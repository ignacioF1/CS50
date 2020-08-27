from cs50 import get_int

h = 0
while True:
    h = get_int("Height: \n")  # get Height from the user
    if h > 0 and h < 9:  # if h given by the user is NOT between 1 and 8, keep asking
        break

for i in range(h):  # for each line
    for j in range(h - i - 1):  # print the first spaces for each line
        print(" ", end="")
    for k in range(i + 1):  # print the first #'s for each line
        print("#", end="")
    print()  # go to the next line