#include<stdio.h>
#include<cs50.h>
#include<string.h>
#include<stdlib.h>

int main(int argc, char **argv)
{
    string key;
    string plain;
    int dig; //Number of digits of the key
    int j;
    int let[26] = {0}; //Array of integers for evaluating repeated letters of the key

    if (argc < 2 || argc > 2)
        //argc has the amount of arguments given at the command. For ./substitution ,argc is 1. For ./substitution ABC ,argc is 2
    {
        printf("Usage: ./substitution key\n"); //The argument is not valid
        return 1;
    }

    key = argv[1]; //Use of the variable key for taking the argument
    dig = strlen(key); //dig is an int with the length of the key

    for (int i = 0 ; i < dig ; i ++) //check for every char on the key if it is a letter
    {
        if (key[i] < 65 || (key[i] > 90 && key[i] < 97) || key[i] > 122)
            //The letters go from ASCII 65(A) to 90(Z) and 97(a) to 122(z)
        {
            printf("Usage: ./caesar key\n"); //The argument is not valid (it is not a letter)
            return 1;
        }
        //Now evaluate if the key has repeated letters
        if (key[i] > 64 && key[i] < 91) //If letter is uppercase and repeated (flag of that letter ==1), end program
        {
            if (let[(key[i] - 65)] == 1)
            {
                printf("Key must not contain repeated characters.\n");
                return 1;
            }
            let[(key[i] - 65)] = 1; //Flag position of thet letter as used
        }
        if (key[i] > 96 && key[i] < 123) //If letter is lowercase and repeated (flag of that letter ==1), end program
        {
            if (let[(key[i] - 97)] == 1)
            {
                printf("Key must not contain repeated characters.\n");
                return 1;
            }
            let[(key[i] - 97)] = 1; //Flag position of thet letter as used
        }
    }

    if (dig != 26)
    {
        printf("Key must contain 26 characters.\n"); //If the argument is not 26 char long, it is not valid
        return 1;
    }

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
        //If it is a letter 65(A) to 90(Z) and 97(a) to 122(z), print according to the key mantaining the upper or lowercase

        else if (plain[i] > 64 && plain[i] < 91) //If it is an uppercase letter, print the key's uppercase letter
        {

            j = (plain[i] - 65); //Store in j the order of the letter from 0 to 25

            if (key[j] > 96 && key[j] < 123) //If the key is lowercase, print its correspondant uppercase
            {
                printf("%c", (key[j] - 32)); //Shift the letter to the correspondant uppercase one
            }
            else if (key[j] > 64 && key[j] < 91)
            {
                printf("%c", key[j]); //Prints the uppercase character
            }

        }
        else if (plain[i] > 96 && plain[i] < 123) //If it is a lowercase letter, print the key's lowercase letter
        {
            j = (plain[i] - 97);

            if (key[j] > 64 && key[j] < 91) //If the key is uppercase, print its correspondant lowercase
            {
                printf("%c", (key[j] + 32)); //Shift the letter to the correspondant lowercase one
            }
            else if (key[j] > 96 && key[j] < 123)
            {
                printf("%c", key[j]); //Prints the lowercase character
            }

        }

    }


    printf("\n"); //Print a space at last
    return 0;

}