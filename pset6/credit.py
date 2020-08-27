from cs50 import get_int
from sys import exit

while True:
    num = get_int("Number: \n")  # ask for credit card number
    if num > 0:  # if not a positive number, ask again
        break

i = 100
temp = 0
while i <= 10000000000000000:  # for (i = 100; i <= 10000000000000000; i *= 100)
    if int(num % i) >= 5:  # the number will have 2 digits when multiplied by 2
        temp += ((int((num % i) / int(i / 10)) * 2) % 10)  # add the unit
        temp += int((((num % i) / int(i / 10)) * 2) / 10)  # add the decimal part
    elif (num % i) < 5:  # the number won't have 2 digits when multiplied by 2
        temp += (int((num % i) / int(i / 10)) * 2)
    i *= 100

i = 10
while i <= 10000000000000000:  # sum of the rest of the numbers (for (i = 10; i <= 10000000000000000; i *= 100))
    temp += int((num % i) / int(i / 10))
    i *= 100
# print(f"temp={temp}")  # FOR TEST
if (temp % 10) == 0:  # if the checksum is 0, it may be a valid number
    if int(num / 1000000000000000) == 4 or int(num / 1000000000000) == 4:
        # if it has 16 numbers and starts with 4 or it has 13 numbers and starts with 4, it is a VISA
        print("VISA")
        exit(0)
numtest = int(num / 100000000000000)
if numtest == 51 or numtest == 52 or numtest == 53 or numtest == 54 or numtest == 55:
    # if it has 16 numbers and starts with 51, 52, 53, 54 or 55, it is a Mastercard
    print("MASTERCARD")
    exit(0)

if int(num / 10000000000000) == 34 or int(num / 10000000000000) == 37:  # if it has 15 numbers and starts with 34 or 37, it is an AMEX
    print("AMEX")
    exit(0)

# if the check sum is not 0 or did not pass the above conditions, it is invalid
print("INVALID")