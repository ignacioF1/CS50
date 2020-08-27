#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    //Verify if the voted candidate is in the list
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i].name) == 0)
        {
            candidates[i].votes += 1;
            return true;
        }
    }
    //The candidate was not in the list
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    int win[MAX] = {0}; //Variable to store the winner(s)
    int most = 0; //Variable to store the highest vote count
    int i = 0;
    for (i = 0 ; i < candidate_count ; i++)
    {
        if (i == 0)
        {
            win[i] = candidates[i].votes; //Store the first to compare
            most = candidates[i].votes; //The first vote quantity will be the larger one for now
        }
        if (candidates[i].votes > most) //If this candidate has more votes than the previous
        {
            most = candidates[i].votes; //Save most with the new value
            win[i] = most; //Store the candidate's votes on the win array
            for (int j = 0 ; j < i ; j ++)
            {
                win[j] = 0; //Erase the previous less voted candidates as there is a more voted one
            }
        }
        if (candidates[i].votes == most) //If the candidate has the same votes as the previos most voted one, store it in the win array
        {
            win[i] = candidates[i].votes;
        }
        if (candidates[i].votes < most)
        {
            win[i] = 0;
        }
    }
    for (i = 0 ; i < candidate_count; i++) //Print the winner(s) and a new line
    {
        if (win[i] > 0)
        {
            printf("%s\n", candidates[i].name);
        }
    }
    return;
}