// Implements a dictionary's functionality

#include <stdbool.h>

#include "dictionary.h"

#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

int wrdcount = 0; // Variable to store the dictionary's words count
char wordr[LENGTH + 1]; // To store each read word
int indx; // To store the index of the word (the result of the hash function)

// Number of buckets in hash table
const unsigned int N = 10000;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    node *cursor;
    char wordL[LENGTH + 1];

    int len = strlen(word); // strlen calculates length of string except for the null byte
    for (unsigned int i = 0; i <= len; i ++)
    {
        wordL[i] = tolower(word[i]); // if it contains uppercase letters, turn them to lowercase
    }
    indx = hash(wordL); // Hash the word
    cursor = table[indx];
    while (cursor != NULL)
    {
        if (strcmp(wordL, cursor -> word) == 0) // Compare the word
        {
            return true;  // word found
        }
        cursor = cursor -> next;
    }
    return false;   // word not found
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned long hash = 5381;  // INITIAL_VALUE (test with 261 as well)
    // OPTION I
    int len = strlen(word); // strlen calculates length of string except for the null byte
    for (unsigned int i = 0; i < len; i ++)
        // (Test a way to avoid calculating the word length, instead if null terminator, end) Conclusion: Not better
    {
        hash = 33 * hash + word[i];
        // M * hash + word[i] (test if rotating 5 times (hash << 5) plus adding hash, which is the same to multiplying by 33 is faster) Conclusion: Not better
    }
    return (hash % N); // Return the reminder  of hash / TABLE_SIZE
    // OPTION II (Conclusion: Not better)
    //int c;
    //while((c = *word++))
    //hash = (hash << 5) + hash + c;
    //return (hash % N);
    
// Bernstein's function uses INITIAL_VALUE of 5381 and M of 33

// Hash function source: https://www.strchr.com/hash_functions
// Abstract: Benchmark program for hash tables and comparison of 15 popular hash functions.
// Created 12 years ago by Peter Kankowski
// Last changed 8 years ago
// Contributors: Nils, Ace, Won, Andrew M., and Georgi 'Sanmayce'
// Filed under Algorithms
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Open dictionary file
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;   // If there was an error openning dictionary, return false
    }

    // Read strings from dictionary file
    while (fscanf(dict, "%s", wordr) != EOF) // Scan each word (wordr is a character array) until end of file
    {
        wrdcount ++; // Increment the words count
        node *n = malloc(sizeof(node)); // Create a node for each word
        if (n == NULL)
        {
            return false;
        }
        strcpy(n -> word, wordr);
        n -> next = NULL;
        // Call the hash function for the word
        indx = hash(wordr);
        // Insert the node into the hash table
        if (table[indx] != NULL) // check if the first node is present
        {
            n -> next = table[indx];   // point the new node to the index's pointed node
            table[indx] = n;   // point the first element of the linked list to the new node
        }
        else
        {
            table[indx] = n;
        }
    } // End while
    fclose(dict); // close file
    return true; // No errors
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return wrdcount;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    node *cursor;
    node *tmp;
    for (int i = 0 ; i < N ; i ++)
    {
        cursor = table[i];
        while (cursor != NULL)
        {
            tmp = cursor;
            cursor = cursor -> next;
            free(tmp);
        }
    }
    return true;    // Memory freed successfully
}
