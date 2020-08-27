# This program prints a list of students for a given house in alphabetical order.
# Excecution: $ python roster.py Gryffindor
import cs50
from cs50 import SQL
from sys import argv, exit

# --------------------- command-line arguments ------------------------------
# Check for the correct command-line arguments
if len(argv) != 2:
    print("Program was executed with the incorrect number of command-line arguments")
    exit(1)
# --------------------- command-line arguments ------------------------------

#
house = argv[1]

# Query the students table in the students.db database for all of the students in the specified house.
# Open students.db
db = cs50.SQL("sqlite:///students.db")

rows = db.execute("SELECT first, middle, last, birth FROM students WHERE house = ? ORDER BY last, first", house)

# print(rows[0]['first'])
for row in rows:
    if row['middle'] == None:
        print(f"{row['first']} {row['last']}, born {row['birth']}")
    else:
        print(f"{row['first']} {row['middle']} {row['last']}, born {row['birth']}")

# Print out each student’s full name and birth year (formatted as, e.g., Harry James Potter, born 1980)
# Each student should be printed on their own line.
# Students should be ordered by last name. For students with the same last name, they should be ordered by first name.