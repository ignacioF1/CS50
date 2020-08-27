import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session, url_for
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash
from time import strptime, mktime, gmtime, localtime
import time
from helpers import apology, login_required, checkPass
import ast # To convert from string to dict
#from flask_talisman import Talisman
#from flask_sslify import SSLify
#import functools
#url_for = functools.partial(url_for, _scheme='https')

# Configure application
app = Flask(__name__)
#Talisman(app, content_security_policy=None)
#sslify = SSLify(app)

app.config['PREFERRED_URL_SCHEME'] = 'https'

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///GIA.db")

@app.route("/", methods=["GET", "POST"])
@login_required
def index():

    """Show Donating and Receiving tables"""
    # Erase Alert (variable for showing messages in the index page)
    alert = ""

    # Read the table's values from the db, ordering the pending first and secondly by date
    donating = db.execute("SELECT * FROM donating ORDER BY receiverID NOT NULL ASC, date DESC")
    # For each row, find the username of the donator and receiver (if any) with the user ID
    memdel=[] # List to store the rows which will not be deleted and not shown because their date plus the PUdays is smaller than the actual time.
    lend = len(donating)
    for i in range(lend):
        username = db.execute("SELECT username FROM users WHERE id = :id", id = donating[i]['donatorID'])
        if username != None:
            donating[i].update({"donatorID" : username[0]['username']})
        if donating[i]['receiverID'] != None:
            username = db.execute("SELECT username FROM users WHERE id = :id", id = donating[i]['receiverID'])
            if username != None:
                donating[i].update({"receiverID" : username[0]['username']})
        # Check if the PUdays have passed and in that case, remove the row from the list to show
        if donating[i]['date'] != None:
            PUdays = donating[i]['PUdays']
            date_str = donating[i]['date']
            target_timestamp = strptime(date_str, '%Y/%m/%d')
            time_epoch = int(mktime(target_timestamp))
            act_time_epoch = int(time.time())
            if act_time_epoch > (time_epoch + (PUdays * 86400)):
                memdel.append(i)
    # Delete the rows stored in memdel from the dict
    memdel.reverse() # reverse the list for deleting the last first
    for i in memdel:
        del donating[i]

    # Read the table's values from the db, ordering the pending first and secondly by date
    receiving = db.execute("SELECT * FROM receiving ORDER BY donatorID NOT NULL ASC, date DESC")
    # For each row, find the username of the receiver and donator (if any) with the user ID
    memdel.clear() # Empty the list
    lenr = len(receiving)
    for i in range(lenr):
        username = db.execute("SELECT username FROM users WHERE id = :id", id = receiving[i]['receiverID'])
        if username != None:
            receiving[i].update({"receiverID" : username[0]['username']})
        if receiving[i]['donatorID'] != None:
            username = db.execute("SELECT username FROM users WHERE id = :id", id = receiving[i]['donatorID'])
            if username != None:
                receiving[i].update({"donatorID" : username[0]['username']})

        # Check if the PUdays have passed and in that case, remove the row from the list to show
        if receiving[i]['date'] != None:
            PUdays = receiving[i]['PUdays']
            date_str = receiving[i]['date']
            target_timestamp = strptime(date_str, '%Y/%m/%d')
            time_epoch = int(mktime(target_timestamp))
            act_time_epoch = int(time.time())
            if act_time_epoch > (time_epoch + (PUdays * 86400)):
                memdel.append(i)
    # Delete the rows stored in memdel from the dict
    memdel.reverse() # reverse the list for deleting the last first
    for i in memdel:
        del receiving[i]

    total_donations = lend
    total_receivings = lenr

    # If the action is POST, send the corresponding row
    if request.method == "POST":
        userID = session["user_id"]
        if 'donate' in request.form:
            #if request.form['donate'] != None:
            # Transform string to dictionary
            #selec = ast.literal_eval(request.form['donate'])
            opID = int(request.form['donate'])
            # Add the current operation to the status register
            #session['oper'] = {"oper" : "donate", "opID" : opID}
            session["opID"] = opID
            # Read the table's values from the db for the selected operation
            selec = db.execute("SELECT * FROM receiving WHERE opID = :opID", opID = opID)
            receiver = selec[0]['receiverID']
            if receiver == userID:
                return apology("The donator can't be the same as the receiver", 403)

            # For the selection, find the username of the receiver with the ID
            username = db.execute("SELECT username FROM users WHERE id = :id", id = receiver)
            if username != None:
                selec[0].update({"receiverID" : username[0]['username']})

            return render_template("donate.html", _external=True, _scheme='https', selec = selec)

        else:
        #elif request.form['receive'] != None:
            opID = int(request.form['receive'])
            session["opID"] = opID
            # Read the table's values from the db for the selected operation
            selec = db.execute("SELECT * FROM donating WHERE opID = :opID", opID = opID)
            donator = selec[0]['donatorID']
            if donator == userID:
                return apology("The receiver can't be the same as the donator", 403)
            # For the selection, find the username of the donator with the ID
            username = db.execute("SELECT username FROM users WHERE id = :id", id = donator)
            if username != None:
                selec[0].update({"donatorID" : username[0]['username']})
            return render_template("receive.html", _external=True, _scheme='https', selec = selec)

    # A dict saved in session stores the 'alert' and its 'status'.
    # If the 'alert' was shown, the next time the page is loaded, 'status' -> 0 and it is not showed again
    if (session.get('alert', None))['status'] == 1:
        alert = (session.get('alert', None))['alert']
        session['alert'] = {"alert" : "", "status" : 0}
    else:
        alert = ""
    return render_template("index.html", _external=True, _scheme='https', donating=donating, receiving=receiving, total_donations=total_donations, total_receivings=total_receivings,alert = alert)

@app.route("/donate", methods=["GET", "POST"])
@login_required
def donate():
    """Donate"""
    if request.method == "POST":
        # If no message provided, return a message
        if not request.form.get("message"):
            return apology("You must provide a message to the receiver", 403)
        userID = session["user_id"]
        message = request.form.get("message")
        opID = session["opID"]
        # Update the list and history
        db.execute("UPDATE receiving SET donatorID = :donator WHERE opID = :opID", donator = userID, opID = opID)
        # Insert the operation into the history record
        # Get the seconds since epoch
        secondsSinceEpoch = time.time()
        # Convert seconds since epoch to struct_time
        timeObj = time.localtime(secondsSinceEpoch)
        resdate = ('%d/%02d/%02d' % (timeObj.tm_year, timeObj.tm_mon, timeObj.tm_mday))
        # Look for the operation in the db
        selec = db.execute("SELECT * FROM receiving WHERE opID = :opID", opID = opID)
        date = selec[0]['date']
        operation = "Accept to donate"
        quantity = selec[0]['quantity']
        product = selec[0]['product']
        description = selec[0]['description']
        location = selec[0]['location']
        providerID = selec[0]['receiverID']
        PUdays = selec[0]['PUdays']
        locatFull = selec[0]['locatFull']
        # Register the operation for the donator's history record
        db.execute("INSERT INTO history (id, date,operation,quantity,product,description,location,resdate,providerID,PUdays,locatFull,message) VALUES (?,?,?,?,?,?,?,?,?,?,?,?)",
                    (userID,date,operation,quantity,product,description,location,resdate,providerID,PUdays,locatFull,message))

        # Update the history record with the opID of the original operation and update the provider and date
        db.execute(" UPDATE history SET providerID = :userID,resdate = :date WHERE opID = :opID", userID = userID,date = date,opID = opID)

        # The operation was submitted correctly
        # Change alert to "Donation made!"
        session['alert'] = {"alert" : "Donation made!", "status" : 1}
        return redirect(url_for('index', _external=True, _scheme='https'))
    else:
        return render_template("donate.html", _external=True, _scheme='https')


@app.route("/receive", methods=["GET", "POST"])
@login_required
def receive():
    """Receive"""
    if request.method == "POST":
        # If no message provided, return a message
        if not request.form.get("message"):
            return apology("You must provide a message to the donator", 403)
        userID = session["user_id"]
        message = request.form.get("message")
        opID = session["opID"]
        # Update the list and history
        db.execute("UPDATE donating SET receiverID = :receiver WHERE opID = :opID", receiver = userID, opID = opID)
        # Insert the operation into the history record
        # Get the seconds since epoch
        secondsSinceEpoch = time.time()
        # Convert seconds since epoch to struct_time
        timeObj = time.localtime(secondsSinceEpoch)
        resdate = ('%d/%02d/%02d' % (timeObj.tm_year, timeObj.tm_mon, timeObj.tm_mday))
        # Look for the operation in the db
        selec = db.execute("SELECT * FROM donating WHERE opID = :opID", opID = opID)
        date = selec[0]['date']
        operation = "Accept to receive"
        quantity = selec[0]['quantity']
        product = selec[0]['product']
        description = selec[0]['description']
        location = selec[0]['location']
        providerID = selec[0]['donatorID']
        PUdays = selec[0]['PUdays']
        locatFull = selec[0]['locatFull']
        # Register the operation for the receiver's history record
        db.execute("INSERT INTO history (id, date,operation,quantity,product,description,location,resdate,providerID,PUdays,locatFull,message) VALUES (?,?,?,?,?,?,?,?,?,?,?,?)",
                    (userID,date,operation,quantity,product,description,location,resdate,providerID,PUdays,locatFull,message))

        # Update the history record with the opID of the original operation and update the provider and date
        db.execute(" UPDATE history SET providerID = :userID,resdate = :date WHERE opID = :opID", userID = userID,date = date,opID = opID)

        # The operation was submitted correctly
        # Change alert to "Donation made!"
        session['alert'] = {"alert" : "Receiving made!", "status" : 1}
        return redirect(url_for('index', _external=True, _scheme='https'))
    else:
        return render_template("receive.html", _external=True, _scheme='https')




@app.route("/history")
@login_required
def history():
    """Show history of operations"""
    # Read the user transactions history
    operations = db.execute("SELECT * FROM history WHERE id = :id",id = session["user_id"])
    # For each row, find the username of the provider (if any) with the ID
    leno = len(operations)
    for i in range(leno):
        if operations[i]['providerID'] != None:
            user = db.execute("SELECT * FROM users WHERE id = :id", id = operations[i]['providerID'])
            if user != None:
                operations[i].update({"providerID" : user[0]['username']})
                # Also add email and phone to the dict
                operations[i].update({"email" : user[0]['email']})
                operations[i].update({"phone" : user[0]['phone']})
    return render_template("history.html", _external=True, _scheme='https', operations = operations)


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
        return redirect(url_for('index', _external=True, _scheme='https'))

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html", _external=True, _scheme='https')


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect(url_for('index', _external=True, _scheme='https'))


@app.route("/post", methods=["GET", "POST"])
@login_required
def post():
    """Post in the donating or receiving list"""
    if request.method == "POST":
        # If any field not completed, return a message
        operation = request.form.get("operation")
        if operation == "Donate":
            operationD = "Post in Donating list"
        else:
            operationD = "Post in Receiving list"
        quantity = request.form.get("quantity")
        product = request.form.get("product")
        description = request.form.get("description")
        location = request.form.get("location")
        days = request.form.get("days")
        if not (operation and quantity and product and description and location):
            return apology("Must complete the form", 403)
        # Check for numeric and positive numbers in quantity
        if quantity.isnumeric() == False:
            return apology("The quantity must be in numbers", 403)
        if int(quantity) <= 0 or int(quantity) > 10000:
            return apology("The quantity must be between 1 and 10000", 403)
        # Insert the operation into the table
        # Get the seconds since epoch
        secondsSinceEpoch = time.time()
        # Convert seconds since epoch to struct_time
        timeObj = time.localtime(secondsSinceEpoch)
        #timestamp = ('%d-%d-%d %d:%d:%d' % (timeObj.tm_year, timeObj.tm_mon, timeObj.tm_mday, timeObj.tm_hour, timeObj.tm_min, timeObj.tm_sec))
        date = ('%d/%02d/%02d' % (timeObj.tm_year, timeObj.tm_mon, timeObj.tm_mday))
        if request.form.get("enableLOC") != None:
            locatFull = request.form.get("locationFull")
        else:
            locatFull = None
        userID = session["user_id"]
        if operation == "Donate":
            db.execute("INSERT INTO donating (date,donatorID,quantity,product,description,location,PUdays,locatFull) VALUES (?,?,?,?,?,?,?,?)",
                    (date,userID,quantity,product,description,location,days,locatFull))
            # Ask for the opID generated automatically for the previous insertion
            last = db.execute("SELECT last_insert_rowid()")
            opID = last[0]['last_insert_rowid()']
            #opID = db.execute("SELECT opID FROM donating WHERE date=:date,donatorID=:donatorID,quantity=:quantity,product=:product,description=:description,location=:location,PUdays=:PUdays,locatFull=:locatFull",
                    #date=date,donatorID=userID,quantity=quantity,product=product,description=description,location=location,PUdays=days,locatFull=locatFull)
        elif operation == "Receive":
            db.execute("INSERT INTO receiving (date,receiverID,quantity,product,description,location,PUdays,locatFull) VALUES (?,?,?,?,?,?,?,?)",
                    (date,userID,quantity,product,description,location,days,locatFull))
            # Ask for the opID generated automatically for the previous insertion
            last = db.execute("SELECT last_insert_rowid()")
            opID = last[0]['last_insert_rowid()']
        # Insert the operation into the history record
        #print(opID) #test
        db.execute("INSERT INTO history (id, date,operation,quantity,product,description,location,PUdays,locatFull,opID) VALUES (?,?,?,?,?,?,?,?,?,?)",
                    (userID,date,operationD,quantity,product,description,location,days,locatFull,opID))
        # The operation was submitted correctly
        # Change alert to "Posted!"
        session['alert'] = {"alert" : "Posted!", "status" : 1}

        return redirect(url_for('index', _external=True, _scheme='https'))
    else:
        return render_template("post.html", _external=True, _scheme='https')


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure email was submitted
        elif not request.form.get("email"):
            return apology("must provide E-mail", 403)

        # Ensure phone was submitted
        elif not request.form.get("phone"):
            return apology("must provide Phone number", 403)

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
        email = request.form.get("email")
        phone = request.form.get("phone")
        # Insert user, email, phone and password (hash) into GIA.db -> users
        db.execute("INSERT INTO users (username,email,phone,hash) VALUES (?,?,?,?)",
                    (username,email,phone,generate_password_hash(request.form.get("password"))))

        # Search for the user id
        rows = db.execute("SELECT id FROM users WHERE username = :username", username = username)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]
        session["user"] = username

        # Change alert to "Registered!"
        session['alert'] = {"alert" : "Registered!", "status" : 1}

        # Redirect user to home page
        return redirect(url_for('index', _external=True, _scheme='https'))
    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html", _external=True, _scheme='https')


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
        # Insert user and password (hash) into GIA.db -> users
        db.execute("UPDATE users SET hash = :hash WHERE username = :username",
                    hash = generate_password_hash(request.form.get("password")), username = username)

        # Change alert to "Password changed!"
        session['alert'] = {"alert" : "Password changed!", "status" : 1}

        # Redirect user to home page
        return redirect(url_for('index', _external=True, _scheme='https'))
    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("config.html", _external=True, _scheme='https')

@app.route("/terms")
@login_required
def terms():
    """Show the 'Terms and conditions'"""
    return render_template("terms.html", _external=True, _scheme='https')

@app.route("/how")
def how():
    """Show the 'How to use page'"""
    return render_template("how.html", _external=True, _scheme='https')

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
