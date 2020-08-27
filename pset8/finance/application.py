import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash
import time
from helpers import apology, login_required, lookup, usd, checkPass

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Alert variable for showing messages in the index page
    alert = ""
    # Read the user's actual cash
    actual_cash = float(db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])[0]["cash"])
    # Read the user transactions history
    operations = db.execute("SELECT id, symbol , name, SUM(shares) AS shar FROM history WHERE id = :id GROUP BY symbol HAVING(shar) > 0 ORDER BY shar DESC",id = session["user_id"])
    total_cash = 0
    l = len(operations)
    for i in range (l):
        symb = operations[i]["symbol"]
        operations[i].update({"price" : (lookup(symb)["price"])})
        operations[i].update({"total" : (operations[i]["price"] * operations[i]["shar"])})
        total_cash = total_cash + (operations[i]["total"])
    total_cash = total_cash + actual_cash
    # A dict saved in session stores the 'alert' and its 'status'.
    # If the 'alert' was shown, the next time the page is loaded, 'status' -> 0 and it is not showed again
    if (session.get('alert', None))['status'] == 1:
        alert = (session.get('alert', None))['alert']
        session['alert'] = {"alert" : "", "status" : 0}
    else:
        alert = ""
    return render_template("index.html", actual_cash = usd(actual_cash), operations = operations, total_cash = usd(total_cash), usd = usd, alert = alert)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        # If no symbol provided, return a message
        if not request.form.get("symbol"):
            return apology("must provide a stock symbol", 403)
        get_shares = request.form.get("shares")
        # If no share's quantity provided or it is not numeric, return a message
        if not get_shares or get_shares.isnumeric() == False:
            return apology("must provide a valid share's quantity", 403)
        # If share's quantity provided is <= 0, return a message
        shares = int(get_shares)
        if shares <= 0:
            return apology("must provide a valid share's quantity", 403)
        else:
            result = lookup(request.form.get("symbol"))
            # If stock symbol not found, return a message
            if result == None:
                return apology("no stock with that symbol", 403)
            else:
                # Query database for the user's actual cash
                rows = db.execute("SELECT * FROM users WHERE id = :id", id = session["user_id"])
                cash_actual = rows[0]["cash"]
                user_id = rows[0]["id"]
                symbol = request.form.get("symbol")
                name = result["name"]
                price = result["price"]
                new_cash = cash_actual - (price * shares)
                # Check if the user has enaugh cash for the operation, if not, return a message
                if new_cash < 0:
                    return apology("no cash available for the operation", 403)
                else:
                    # Update cash
                    db.execute("UPDATE users SET cash = :new_cash WHERE id = :user_id", new_cash = new_cash, user_id = user_id)
                    # Insert operation into history table with the time stamp
                    # Get the seconds since epoch
                    secondsSinceEpoch = time.time()
                    # Convert seconds since epoch to struct_time
                    timeObj = time.localtime(secondsSinceEpoch)
                    timestamp = ('%d-%d-%d %d:%d:%d' % (timeObj.tm_year, timeObj.tm_mon, timeObj.tm_mday, timeObj.tm_hour, timeObj.tm_min, timeObj.tm_sec))
                    db.execute("INSERT INTO history (id,symbol,name,shares,price,transacted) VALUES (?,?,?,?,?,?)",
                                user_id, symbol, name, shares, price, timestamp)
                # Change alert to "Bought!"
                session['alert'] = {"alert" : "Bought!", "status" : 1}
                return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # Read the user transactions history
    operations = db.execute("SELECT * FROM history WHERE id = :id",id = session["user_id"])
    return render_template("history.html", operations = operations, usd = usd)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        username = request.form.get("username")
        rows = db.execute("SELECT * FROM users WHERE username = :username", username = username)

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]
        session["user"] = rows[0]["username"]

        # Change alert to "Logged in!"
        session['alert'] = {"alert" : "Logged in!", "status" : 1}

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        # If no symbol provided, return a message
        if not request.form.get("symbol"):
            return apology("must provide a stock symbol", 403)
        else:
            result = lookup(request.form.get("symbol"))
            # If symbol not found, return a message
            if result == None:
                return apology("no stock with that symbol", 403)
            else:
                # The symbol was found, return the result
                return render_template("quoted.html", result=result)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Ensure password has the requiered characters
        elif checkPass(request.form.get("password")) == False:
            return apology("password must contain numbers and letters", 403)

        # Ensure Re-type password was submitted
        elif not request.form.get("rpassword"):
            return apology("must re-type password", 403)

        # Ensure Re-type password is equal to the password
        elif request.form.get("password") != request.form.get("rpassword"):
            return apology("Re-typed password is not equal to the password", 403)

        # Query database for username
        username = request.form.get("username")
        rows = db.execute("SELECT * FROM users WHERE username = :username", username = username)

        # Ensure username does not exist
        if len(rows) == 1:
            return apology("Username already exists", 403)

        # All went OK, so register the new user

        # Insert user and password (hash) into finance.db -> users
        db.execute("INSERT INTO users (username,hash) VALUES (?,?)",
                    (username, generate_password_hash(request.form.get("password"))))

        # Search for the user id
        rows = db.execute("SELECT id FROM users WHERE username = :username", username = username)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]
        session["user"] = username

        # Change alert to "Registered!"
        session['alert'] = {"alert" : "Registered!", "status" : 1}

        # Redirect user to home page
        return redirect("/")
    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        # If no symbol provided, return a message
        if not request.form.get("symbol"):
            return apology("must provide a symbol", 403)

        get_shares = request.form.get("shares")
        # If no share's quantity provided or it is not numeric, return a message
        if not get_shares or get_shares.isnumeric() == False:
            return apology("must provide a valid share's quantity", 403)
        # If share's quantity provided is <= 0, return a message
        shares = int(get_shares)
        if shares <= 0:
            return apology("must provide a valid share's quantity", 403)
        symbol = str.lower(request.form.get("symbol"))
        user_id = session["user_id"]
        portfolio = db.execute("SELECT SUM(shares) AS shar FROM history WHERE id = :id AND symbol = :symbol GROUP BY symbol",id = user_id , symbol = symbol)
        act_shares = portfolio[0]["shar"]
        # Check if the user has the quantity requested to sell
        if shares > act_shares:
            return apology("the share's quantity is more than what you have", 403)
        # Everithing good, update the user's cash and register the transaction
        result = lookup(request.form.get("symbol"))
        name = result["name"]
        price = result["price"]
        # Read the user's actual cash
        actual_cash = float(db.execute("SELECT cash FROM users WHERE id = :id", id = user_id)[0]["cash"])
        new_cash = actual_cash + (price * shares)
        shares_hist = -(shares)
        # Update cash
        db.execute("UPDATE users SET cash = :new_cash WHERE id = :user_id", new_cash = new_cash, user_id = user_id)
        # Insert operation into history table
        # Get the seconds since epoch
        secondsSinceEpoch = time.time()
        # Convert seconds since epoch to struct_time
        timeObj = time.localtime(secondsSinceEpoch)
        timestamp = ('%d-%d-%d %d:%d:%d' % (timeObj.tm_year, timeObj.tm_mon, timeObj.tm_mday, timeObj.tm_hour, timeObj.tm_min, timeObj.tm_sec))
        db.execute("INSERT INTO history (id,symbol,name,shares,price,transacted) VALUES (?,?,?,?,?,?)",user_id, symbol, name, shares_hist, price, timestamp)
        # Change alert to "Sold!"
        session['alert'] = {"alert" : "Sold!", "status" : 1}
        return redirect("/")
    else:
        # GET method
        # Read the user transactions history to show the available stocks
        portfolio = db.execute("SELECT symbol FROM history WHERE id = :id GROUP BY symbol HAVING SUM (shares) > 0",id = session["user_id"])
        return render_template("sell.html", portfolio = portfolio)

@app.route("/config", methods=["GET", "POST"])
@login_required
def config():
    """Show user configuration / to change password"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure password was submitted
        if not request.form.get("password"):
            return apology("must provide password", 403)

        # Ensure password has the requiered characters
        elif checkPass(request.form.get("password")) == False:
            return apology("password must contain numbers and letters", 403)

        # Ensure Re-type password was submitted
        elif not request.form.get("rpassword"):
            return apology("must re-type password", 403)

        # Ensure Re-type password is equal to the password
        elif request.form.get("password") != request.form.get("rpassword"):
            return apology("Re-typed password is not equal to the password", 403)

        # All went OK, so update password
        username = session["user"]
        # Insert user and password (hash) into finance.db -> users
        db.execute("UPDATE users SET hash = :hash WHERE username = :username",
                    hash = generate_password_hash(request.form.get("password")), username = username)

        # Change alert to "Password changed!"
        session['alert'] = {"alert" : "Password changed!", "status" : 1}

        # Redirect user to home page
        return redirect("/")
    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("config.html")

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
