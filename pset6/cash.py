from cs50 import get_float

while True:
    change = get_float("Change owed:\n")  # prompts the user for the change owed to the customer
    if change > 0:  # if the number is negative, ask again
        break

change *= 100  # convert dollars to cents
chanr = round(change)  # round the change in cents
# chanr is an int which contains the rounded change
coins = int(chanr / 25)
chanr -= (coins * 25)  # decrement the change with the coins used
if chanr % 25 > 0:  # if the reminder is not 0, add 10c coins
    coins += int(chanr / 10)
    chanr -= (int(chanr / 10) * 10)  # decrement the change with the coins used
if chanr % 10 > 0:  # if the reminder is not 0, add 5c coins
    coins += int(chanr / 5)
    chanr -= (int(chanr / 5) * 5)  # decrement the change with the coins used
if chanr % 5 > 0:  # if the reminder is not 0, add 1c coins
    coins += int(chanr / 1)

print(f"{coins}")  # print the amount of coins needed