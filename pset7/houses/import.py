# This program imports data from a CSV spreadsheet and loads students.db database.
# Excecution: $ python import.py characters.csv
import cs50
import csv
from sys import argv, exit
n0 = []
n1 = []
n2 = []
count = 0
counts = {}
i = 0

# --------------------- command-line arguments ------------------------------
# Check for the correct command-line arguments
if len(argv) != 2:
    print("Program was executed with the incorrect number of command-line arguments")
    exit(1)
# --------------------- command-line arguments ------------------------------

# Open the CSV file and read its contents into memory
with open(argv[1], newline="") as csvfile:
    characters = csv.DictReader(csvfile)
    for row in characters:
        name = row["name"]
        house = row["house"]
        birth = row["birth"]
        counts[i] = name, house, birth  # Dictionary count[A][B] where A is order, B =0 is name, =1 is house =2 is birth
        i += 1
# print(counts[3][2])  # print birth of the fourth item

# --------------------- Separate name into first, middle and last ------------------------------
for j in range(i):
    words = (counts[j][0]).split()
    if len(words) == 3:
        n0.append(words[0])
        n1.append(words[1])
        n2.append(words[2])
    elif len(words) == 2:
        n0.append(words[0])
        n1.append(None)  # If no middle name, leave middle name field as NULL in the table. "NULL" in Python is "None"
        n2.append(words[1])
#        print(words[0])
    count += 1
# print(n0[11],n1[11],n2[11])
# --------------------- Separate name into first, middle and last ------------------------------

# --------------------- Insert student into the students table in the students.db database ------------------------------
# Open students.db
db = cs50.SQL("sqlite:///students.db")
# Create table students in students.db
#db.execute("CREATE TABLE students(first TEXT, middle TEXT, last TEXT, house TEXT, birth NUMERIC)")
# Insert students into table
for j in range(0, count):
    first = n0[j]
    middle = n1[j]
    last = n2[j]
    house = counts[j][1]
    birth = counts[j][2]
    db.execute("INSERT INTO students(first, middle, last, house, birth)VALUES(?, ?, ?, ?, ?)", first, middle, last, house, birth)
# --------------------- Insert student into the students table in the students.db database ------------------------------