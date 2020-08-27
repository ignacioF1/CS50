#include<stdio.h>
#include<cs50.h>
#include<string.h>
#include<stdlib.h>

string plain;
int dig; //Number of digits of the key
int key = 0; //The key in int
char j;

int main(int argc, char **argv)
{

    if (argc < 2 || argc > 2) 
        //argc has the amount of arguments given at the command. For ./caesar ,argc is 1. For ./caesar 12 ,argc is 2
    {
        printf("Usage: ./caesar key\n"); //The argument is not valid
        return 1;
    }

    //printf("%s\n", argv[1]); //Uncomment to print the argument

    plain = argv[1]; //Use of the variable plain for taking the argument, after I'll use it for taking the plaintext
    dig = strlen(plain); //dig is an int with the length of the key
    for (int i = 0 ; i < dig ; i ++) //check for every char on the key if it is a number
    {
        if (plain[i] < 48 || plain[i] > 57) //The numbers go from ASCII 48(0) to 57(9)
        {
            printf("Usage: ./caesar key\n"); //The argument is not valid
            return 1;
        }
    }
    
    key = atoi(plain); //It is a number, so convert the argument to the int k (key)
    //printf("key(int):%i\n", key); //Uncomment to print the key stored in an integer

    plain = get_string("plaintext:"); //get the text to encrypt from the user
    dig = strlen(plain); //dig is an int with the length of the plaintext
    
    printf("ciphertext:");
    
    for (int i = 0 ; i < dig ; i ++) //analyze every char from the plaintext
    {

        if (plain[i] < 65 || (plain[i] > 90 && plain[i] < 97) || plain[i] > 122)
            //If it is not a letter (space, punctuation mark, etc) print it as is
        {
            printf("%c", plain[i]);
        }
        //If it is a letter 65(A) to 90(Z) and 97(a) to 122(z), shift it k positions mantaining the upper or lowercase
        else if (plain[i] > 64 && plain[i] < 91) //If it is an uppercase letter, print the k's uppercase letter
        {

            j = ((plain[i] - 65 + key) % 26); //The operation remainder A % B = (A if A<B), (remainder if A>=B)
            printf("%c", (j + 65)); //Prints the shifted character

        }
        else if (plain[i] > 96 && plain[i] < 123) //If it is a lowercase letter, print the k's lowercase letter
        {
            j = ((plain[i] - 97 + key) % 26);
            printf("%c", (j + 97)); //Prints the shifted character
        }

    }


    printf("\n"); //Print a space at last
    return 0;

}