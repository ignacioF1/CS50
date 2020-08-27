#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;
int strength[MAX * (MAX - 1) / 2];
//strength is an array with the same ordering as pairs, which contains the strength between winner and loser

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
void merge_sort(int low, int high);
void merge(int low, int mid, int high);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();

    sort_pairs();

    lock_pairs();

    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    //If name is equal to the name of a valid candidate, update ranks and return true
    for (int i = 0 ; i < candidate_count ; i ++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    //Update the preferences array with the ranks array
    for (int i = 0 ; i < (candidate_count - 1) ; i ++)
    {
        for (int j = 1 ; (i + j) < candidate_count ; j ++)
        {
            preferences[ranks[i]][ranks[i + j]] += 1;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0 ; i < (candidate_count - 1) ; i ++)
    {
        for (int j = 1 ; (i + j) < candidate_count ; j ++)
        {
            if (preferences[i][i + j] > preferences[i + j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = (i + j);
                pair_count ++;
            }
            if (preferences[i][i + j] < preferences[i + j][i])
            {
                pairs[pair_count].loser = i;
                pairs[pair_count].winner = (i + j);
                pair_count ++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int k = -1;
    for (int i = 0 ; i < (candidate_count - 1) ; i ++)
    {
        for (int j = 1 ; (i + j) < candidate_count ; j ++)
        {
            if (preferences[i][i + j] > preferences[i + j][i])
            {
                k ++;
                strength[k] = (preferences[i][i + j] - preferences[i + j][i]);
            }
            if (preferences[i][i + j] < preferences[i + j][i])
            {
                k ++;
                strength[k] = (preferences[i + j][i] - preferences[i][i + j]);
            }
        }
    }
// Now we sort strength and pairs from high to low
    if (pair_count > 1) // Only if there are 2 ore more pairs, sort them
    {
        merge_sort(0, (pair_count - 1));
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    bool row[candidate_count];  // Row for storing the result of the rows ORed
    bool col[candidate_count];  // Column for storing the result of the columns ORed
    bool rowb[candidate_count];   // To store a copy of row before its modification
    bool colb[candidate_count];   // To store a copy of col before its modification
    int locked_count = 0;   // Count the number of locked candidates. If all are locked, a circle is formed
    int count = 0;
    int f;
    int g;
    bool candi_locked[candidate_count];  // To store if the candidate has already been locked

    for (int k = 0 ; k < candidate_count ; k ++)    // Fill the arrays with False
    {
        candi_locked[k] = false;
        row[k] = false;
        rowb[k] = false;
        col[k] = false;
        colb[k] = false;
    }                                           //

    for (int i = 0; i < pair_count ; i++)   // For all the pairs
    {
        f = pairs[i].winner;
        g = pairs[i].loser;

        row[f] = true;  // Rows ORed
        col[g] = true;  // Columns ORed

        count = 0;
        // Check for a middle circle between the pairs.
        for (int d = 0 ; d < candidate_count ; d ++) // For all of the rows and columns
        {
            if (row[d] == true && col[d] == true)
            {
                count ++;
            }
        }

        if (count < 3)  // If the third pivot was not reached
        {
            locked[f][g] = true;    // Lock the loser in the locked array
            rowb[f] = row[f];   // Update row copy
            colb[g] = col[g];   // Update col copy
            if (candi_locked[(pairs[i].loser)] == false)   // If the candidate has not been locked already, add it to the count
            {
                locked_count ++;
                candi_locked[(pairs[i].loser)] = true;
            }
        }

        row[f] = rowb[f];   // Restore row copy
        col[g] = colb[g];   // Restore col copy
    }
    return;
}

//   0 1 2 3 4 5  col
// 0 x 0 1 1 0 0   1
// 1 1 x 0 0 1 0   1
// 2 0 0 x 0 0 0   0
// 3 0 1 0 x 0 0   1
// 4 0 0 0 0 x 0   0
// 5 0 0 0 0 0 x   0

//   1 1 1 1 1 0 row

// As row[0] = row[0], row[1] = row[1], row[3] = row[3] (three locks in the same rows and cols' order), we know that they form a circle

// Print the winner of the election
void print_winner(void)
{
    int count;

    for (int j = 0 ; j < candidate_count ; j ++)
    {
        count = 0;

        for (int i = 0 ; i < candidate_count ; i ++)
        {
            if (locked[i][j] == false)  // If candidate is not blocked, add one to count
            {
                count ++;
            }
            if (count == candidate_count)
            {
                printf("%s\n", candidates[j]); // print the winner's name
            }
        }
    }
    return;
}

void merge_sort(int low, int high) // low order of the array, high order of the array
{
    int mid;
    if (low != high) // Until the array has only one element
    {
        mid = (low + high) / 2; // Compute the middle of the array
        merge_sort(low, mid);   // Sort the left part
        merge_sort(mid + 1, high); // Sort the right part
        merge(low, mid, high); // Merge the sorted right half with the sorted left half
    }
}

void merge(low, mid, high)
{
    pair temp1[pair_count];
    int temp[pair_count];
    int i = low;
    int j = mid + 1 ;
    int k = low ;

    while ((i <= mid) && (j <= high)) // While both halves have one or more elements
    {
        if (strength[i] >= strength[j]) // If the low end of the left half is >= low end of the right half
        {
            temp[k] = strength[i]; // Store the low end of the left half in the low end of temp
            temp1[k] = pairs[i];
            k ++;
            i ++;
        }
        else
        {
            temp[k] = strength[j] ; // If not, store the low end of the right part in the low end of temp
            temp1[k] = pairs[j];
            k ++;
            j ++;
        }
    }

    while (i <= mid)    // While i is in the left half
    {
        temp[k] = strength[i];  // Copy num to temp
        temp1[k] = pairs[i];
        k ++;
        i ++;
    }
    while (j <= high)   // While j is in the right part
    {
        temp[k] = strength[j];  // Copy num to temp
        temp1[k] = pairs[j];
        k ++;
        j ++;
    }
    for (i = low; i <= high ; i++)  // Copy temp to num
    {
        strength[i] = temp[i];
        pairs[i] = temp1[i];
    }

}