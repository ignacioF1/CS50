#include <stdio.h>
#include <cs50.h>
#include <math.h>

int coins = 0;
float change = 0;

int main(void)
{

    do
    {
        change = get_float("Change owed:\n");//prompts the user for the change owed to the customer
    }
    while (change < 0); //if the number is negative, ask again

    change *= 100; //convert dollars to cents
    int chanr = round(change); //round the change in cents
//chanr is an int which contains the rounded change
    coins = chanr / 25;
    chanr -= (coins * 25); //decrement the change with the coins used

    if (chanr % 25 > 0) //if the reminder is not 0, add 10c coins
    {
        coins += (chanr / 10);
        chanr -= ((chanr / 10) * 10); //decrement the change with the coins used
    }
    if (chanr % 10 > 0) //if the reminder is not 0, add 5c coins
    {
        coins += (chanr / 5);
        chanr -= ((chanr / 5) * 5); //decrement the change with the coins used
    }
    if (chanr % 5 > 0) //if the reminder is not 0, add 1c coins
    {
        coins += (chanr / 1);
    }

    printf("%i\n", coins);//print the amount of coins needed

}
