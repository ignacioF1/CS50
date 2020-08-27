#include <stdio.h>
#include <cs50.h>



int main(void)
{
    string name = get_string("What's your name?\n"); //prompts user for name
    printf("hello, %s\n", name); //prints hello, and the user's input
}
