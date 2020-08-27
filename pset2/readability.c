#include<stdio.h>
#include<cs50.h>
#include<string.h>
#include<math.h>

float grade;
int i;
int j;
int k = 0;
int let = 0;
int wor = 0;
int sent = 0;
string temp;

int main(void)
{

    string text = get_string("Text: ");


    i = strlen(text); //It also counts spaces

    for (j = 0 ; j < i ; j ++)
    {
        if (text[j] != 32) //The ASCII code for space is 32
        {
            let ++; //If the next character is not a space or punctuation mark, increment the letter count
        }

        if ((text[j] >= 33 &&  text[j] <= 47) || (text[j] >= 58 &&  text[j] <= 64))
        {
            let --; //If the next character is a punctuation mark, decrement the letter count
            // printf("pm%c",text[j]); //Uncomment this to print the punctuation marks excluded
        }

        if (k == 0 && text[j] != 32) //The first non space character means the start of the first word
        {
            wor ++; //Add 1 to the word count
            k = 1; //Flag to indicate the first word came
        }

        if (k == 1 && text[j] == 32)
            //If the character is space and the first word have already appeared and the previous was not space, add another word
        {
            wor ++; //Add another word to the count
            k = 2; //Indicate that for this j, there was a space.
        }
        if (text[j] != 32) //Turn k to 1 in the next non space character
        {
            k = 1; //If in the next j there is no space, turn k to 1 again to continue with the word count
        }

        if (text[j] == 33 || text[j] == 46 || text[j] == 63) // 46 punto, 63 ? y 33 ! delimitan oraciones
        {
            sent ++; //Add 1 to the sentence count
        }

    }
    
    float let1 = let; //Move the integer variables into float ones to get the decimal part in the formula
    float wor1 = wor;
    float sent1 = sent;

    grade = 0.0588 * (let1 * 100 / wor1) - 0.296 * (sent1 * 100 / wor1) - 15.8 ; //

    //printf("%i letter(s)\n", let); //Uncomment the following to get the number of letter(s), word(s) and sentence(s)
    //printf("%i word(s)\n", wor);
    //printf("%i sentence(s)\n", sent);

    //printf("Grade %f\n", grade); //Uncomment this to print the unrounded result
    
    grade = round(grade);  //Rounds the result

    if (grade > 16) //if grade is bigger than 16, print 16+ and return
    {
        printf("Grade 16+\n");
        return 0;
    }
    else if (grade < 1) //if grade is less than 1, print Before Grade 1 and return
    {
        printf("Before Grade 1\n");
        return 0;
    }
    printf("Grade %.0f\n", grade); //if none of the previous was true, print the grade number

}