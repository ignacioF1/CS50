#include <cs50.h>
#include <stdio.h>

long num;
int temp = 0;
long i;

int main(void)
{


    do
    {
        num = get_long("Number:\n");//ask for credit card number
    }
    while (num <= 0); //if not a positive number, ask again

    for (i = 100; i <= 10000000000000000; i *= 100)
    {
        if ((num % i) >= 5) //the number will have 2 digits when multiplied by 2
        {
            temp += ((((num % i) / (i / 10)) * 2) % 10);//add the unit
            temp += ((((num % i) / (i / 10)) * 2) / 10);//add the decimal part
        }
        else if ((num % i) < 5)//the number won't have 2 digits when multiplied by 2
        {
            temp += (((num % i) / (i / 10)) * 2);   
        }
    }

    for (i = 10; i <= 10000000000000000; i *= 100) //sum of the rest of the numbers
    {
        temp += ((num % i) / (i / 10));   
    }

    if ((temp % 10) == 0) //if the checksum is 0, it may be a valid number
    {
        if ((num / 1000000000000000) == 4 || (num / 1000000000000) == 4) 
            //if it has 16 numbers and starts with 4 or it has 13 numbers and starts with 4, it is a VISA
        {
            printf("VISA\n");
            return 0;
        }
        if ((num / 100000000000000) == 51 
            || (num / 100000000000000) == 52 
            || (num / 100000000000000) == 53
            || (num / 100000000000000) == 54 
            || (num / 100000000000000) == 55) 
            //if it has 16 numbers and starts with 51, 52, 53, 54 or 55, it is a Mastercard
        {
            printf("MASTERCARD\n");
            return 0;
        }
        if ((num / 10000000000000) == 34 || (num / 10000000000000) == 37) //if it has 15 numbers and starts with 34 or 37, it is an AMEX
        {
            printf("AMEX\n");
            return 0;
        }
    }
    //if the check sum is not 0 or did not pass the above conditions, it is invalid
    printf("INVALID\n"); 
}
