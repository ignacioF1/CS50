#include <cs50.h>
#include <stdio.h>
int h = 0;
int main(void)
{
    do
    {
        h = get_int("Height: \n"); //get Height from the user 
    }
    while (h <= 0 || h > 8); //if h given by the user is NOT between 1 and 8, keep asking
    for (int i = 1 ; i <= h ; i++) //for each line
    { 
        for (int j = 1 ; j <= (h - i) ; j ++) //print the first spaces for each line
        {
            printf(" ");    
        }
        for (int k = 1 ; k <= i ; k ++) //print the first #'s for each line    
        {
            printf("#");
        }
        printf("  "); //print two spaces

        for (int k = 1 ; k <= i ; k ++) //print the second #'s for each line
        {
            printf("#");
        }

        printf("\n"); //go to the next line
    }

}

