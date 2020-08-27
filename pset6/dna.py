import csv
import re
from sys import argv, exit

strcount = []  # List to store the STRs appearance counts

# --------------------- command-line arguments ------------------------------
# Check for the correct command-line arguments
if len(argv) != 3:
    print("Program was executed with the incorrect number of command-line arguments")
    exit(1)
# --------------------- command-line arguments ------------------------------

# ------------------------ Search function ------------------------------
# Search function (search for a STR in the ADN secuence)
# STR "Short Tandem Repeat" is a sequence of DNA which repeats consecutively
# Compute the longest STR


def search(STR):  # function to search for str in the adn
    finalcount = 0
    count = 0
    j = 0
    inilist = [m.start() for m in re.finditer(STR, adn)]  # import "re" module for this method to work
    for i in range(len(inilist)):  # inilist is a list with the positions of the first letter of the STR found in the adn string
        # print(inilist[i])  # FOR TESTING!!!!!!
        if (i + 1) < len(inilist):  # Check if this "i" is the last item in the inilist list
            if inilist[i] + len(STR) == inilist[i + 1]:
                # if the str found in the ith position + the str length equals the next appearence, they are consecuents
                count += 1  # count the appearances
                if j == 0:  # if it is the first time, add the first appearance
                    j = 1
                    count += 1
            else:                       # if the next appearance is not consecutive
                if finalcount < count:  # if count is greater than finalcount, update it
                    finalcount = count  # update finalcount with the highest value
                count = 0  # reset counter
                j = 0
        else:                       # if it is the last repetition in the list
            if finalcount < count:  # if count is greater than finalcount, update it
                finalcount = count  # update finalcount with the highest value
            count = 0  # reset counter
            j = 0
    if len(inilist) != 0 and finalcount == 0:  # If there is only one appearance, count it
        finalcount = 1  # count one appearance
    strcount.append(finalcount)  # Update the STR count list
# --------------------- END search function ------------------------------


# ------------------------ Open DNA sequence ------------------------------
# Open the DNA sequence TXT and read its contents into memory
with open(argv[2], 'r') as txtfile:
    adn = txtfile.read()
# print(f"{adn}")
# --------------------- END Open DNA sequence ------------------------------

# Open the CSV file and read its contents into memory
with open(argv[1], newline="") as csvfile:
    people = csv.DictReader(csvfile)
    columcount = len(people.fieldnames)  # columns count
    # print(len(people.fieldnames))  # columns count
    # print(people.fieldnames[1])  # second column of first row (1st STR)
    # print(people.fieldnames[2])  # third column of first row (2nd STR)
    # print(people.fieldnames[3])  # fourth column of first row (3rd STR)
    # print(f"{columcount - 1}")  # columns count

    rowcount = 0
    names = []  # List of names
    numbers = []  # List of STR lists for each person. numbers[Person][STR] For example numbers[1][0] first STR of the second person

    for row in people:
        rowcount += 1
        names.append(row["name"])

        filas = []  # Creates a temporary list for storing the STR numbers for each person
        for i in range(1, columcount):
            # print(f"{row[people.fieldnames[i]]}")  # Print the STR numbers for each person
            filas.append(row[people.fieldnames[i]])
            # numbers.append(row[people.fieldnames[i]])
            # print(numbers)  # Print the numbers for each candidate in one unique list
        numbers.append(filas)

    str = []
    for i in range(1, columcount):
        str.append(people.fieldnames[i])
        # print(people.fieldnames[i])  # STRs to search for in the txt
    # print(f"{str[0]}")  # first str
    # print(f"{str[columcount - 2]}")  # last str


for i in range(0, columcount - 1):  # Go through all of the STRs of the csv file
    # print(f"{str[i]}")  # Print each STR
    search(str[i])  # Search for the STR in the DNA sequence and save them into the list "strcount[]"
    # print(f"{strcount[i]}")  # Prints each STR count FOR TESTING !!!!!!!!!!!!!!!!


# If each of the DNA STRs matches, print the person's name
for i in range(rowcount):  # Go through all of the people in the csv file
    count = 0
    for j in range(0, columcount - 1):  # Go through all of the STRs of the csv file
        if int(strcount[j]) == int(numbers[i][j]):
            count += 1
        # print(strcount[j])  # FOR TESTING !!!!!!!!!!!!!!!!
        # print(numbers[i][j])  # FOR TESTING !!!!!!!!!!!!!!!!
    if count == (columcount - 1):
        print(names[i])
        exit(0)
print("No match")