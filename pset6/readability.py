from cs50 import get_string
from sys import exit

let = 0
wor = 0
sent = 0
k = 0

text = get_string("Text: ")

i = len(text)  # It also counts spaces

for j in range(i):
    if ord(text[j]) != 32:  # The ASCII code for space is 32
        let += 1  # If the next character is not a space or punctuation mark, increment the letter count

    if (ord(text[j]) >= 33 and ord(text[j]) <= 47) or (ord(text[j]) >= 58 and ord(text[j]) <= 64):
        let -= 1  # If the next character is a punctuation mark, decrement the letter count
        # print(f"pm{text[j]}")  # Uncomment this to print the punctuation marks excluded

    if k == 0 and ord(text[j]) != 32:  # The first non space character means the start of the first word
        wor += 1  # Add 1 to the word count
        k = 1  # Flag to indicate the first word came

    if k == 1 and ord(text[j]) == 32:
        # If the character is space and the first word have already appeared and the previous was not space, add another word
        wor += 1  # Add another word to the count
        k = 2  # Indicate that for this j, there was a space.

    if ord(text[j]) != 32:  # Turn k to 1 in the next non space character
        k = 1  # If in the next j there is no space, turn k to 1 again to continue with the word count

    if ord(text[j]) == 33 or ord(text[j]) == 46 or ord(text[j]) == 63:  # 46 punto, 63 ? y 33 ! delimitan oraciones
        sent += 1  # Add 1 to the sentence count

grade = 0.0588 * (let * 100 / wor) - 0.296 * (sent * 100 / wor) - 15.8

# print(f"{let} letter(s)")  # Uncomment the following to get the number of letter(s), word(s) and sentence(s)
# print(f"{wor} word(s)")
# print(f"{sent} sentence(s)")

# print(f"Grade {grade}")  # Uncomment this to print the unrounded result

grade = round(grade)   # Rounds the result

if grade > 16:  # if grade is bigger than 16, print 16+ and return
    print("Grade 16+")
    exit(0)
elif grade < 1:  # if grade is less than 1, print Before Grade 1 and return
    print("Before Grade 1")
    exit(0)

print(f"Grade {grade}")  # if none of the previous was true, print the grade number