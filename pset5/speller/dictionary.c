// Implements a dictionary's functionality
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 65536;        //Have to make this smaller for space efficiency :(

//Keep track of number of words in dictionary
unsigned int cnt = 0;

// Hash table
node *table[N] = { NULL };

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    //Firstly hash the word and then check all nodes on that hash number in the hash table for possible match
    unsigned int x = hash(word);
    node *check = table[x];
    
    while (check != NULL)
    {
        if (check->word[1] == tolower(word[1]))
        {
            if (strcasecmp(check->word, word) == 0)
            {
                return true;
            }
        }
        check = check->next;
    }
    return false;
}

//I don't know, most people seem to prefer this ...
// unsigned int hash(const char *word)
// {
//     unsigned int hash = 0;
//     for (int i=0, n=strlen(word); i<n; i++) {
//         hash = (hash << 2) ^ tolower(word[i]);
//     }
//     return hash % N;    
// }

//Source: https://sunilsavanur.wordpress.com/2012/08/14/polynomial-hash-function-for-dictionary-words/
// unsigned int hash(const char *word)
// {
//     float hashInd = 0.0, sum = 0.0;
//     int j = 0, len = strlen(word);

//     for (j = 0; j < len; j++)
//     {
//         sum += (int) tolower(word[j]);
//         hashInd += (float) ((int) tolower(word[j])) * pow(2, j);
//     }
//     unsigned int indexofKey = (((int) fmod(hashInd, sum) * len) % N);
//     return indexofKey;
// }

//Jenkins hash function (https://en.wikipedia.org/wiki/Jenkins_hash_function)
// unsigned int hash(const char *word)   ///rank 1496
// {
//     unsigned int hash = 1;
//     for(int i = 0, len = strlen(word); i < len; ++i)
//     {
//         hash += tolower(word[i]);
//         hash += (hash << 10);
//         hash ^= (hash >> 6);
//         //hash += (hash << 15);
//     }
//     hash += (hash << 3);
//     hash ^= (hash >> 11);
//     hash += (hash << 15);
//     return hash % N;
// }

//Hashes word to a number (LOWERCASE THE WORD WHICH IS BEING HASHED, VERY IMP!!)
//The algo used is FNV-1a hash (https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function)
unsigned int hash(const char *word)  //rank 1127 1346  BEST(165)!!!
{
    uint64_t indHash = 14695981039346656037U; //14695981039346656037;
    for (int i = 0, len = strlen(word); i < len; i++)
    {
        indHash ^= tolower(word[i]);                               
        indHash *= 1099511628211U;  //1099511628211 ;
    }
    return indHash % (N);
}

//Daniel Lamire's blog (https://lemire.me/blog/2015/10/22/faster-hashing-without-effort/)
// unsigned int hash(const char *word)  
// {
//     unsigned int h = 1;
//     int i = 0, len = strlen(word);
//     for (; i + 3 < len; i += 4) 
//     {
//         h = 31 * 31 * 31 * 31 * h + 31 * 31 * 31 * tolower(word[i]) + 31 * 31
//                 * tolower(word[i + 1]) + 31 * tolower(word[i + 2]) + tolower(word[i + 3]);
//     }
//     for (; i < len; i++) 
//     {
//         h = 31 * h + tolower(word[i]);
//     }
//     return h % N;
// }

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Open the file for reading, and check if it was opened correctly
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Sorry cannot open file!\n");
        return false;
    }

    //Create an array to store a scanned word from file
    char fword[LENGTH + 1];
    
    //Read the whole file, storing the words into a hash table
    while (fscanf(file, "%s", fword) != EOF)
    {
        //Create a node to store the word scanned 
        node *n = malloc(sizeof(node));
        memset(n, 0, sizeof(node));      //This is used to initialize the node to 0 or NULL, accordingly (VALGRIND ERROR)
        if (n == NULL)
        {
            return false;
        }

        //Store the word in node then get the hash value 
        strcpy(n->word, fword);
        unsigned int x = hash(fword);
        
        //Insert the word accordingly to the hash table (SPENT A LOT OF TIME HERE!)
        //Basically inserts every new word to starting of list
        n->next = table[x];
        table[x] = n;
        
        //This value keeps track of total number of words read successfully
        cnt++;
    }
    //Close the file and return true for successful reaad
    fclose(file);

    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // Return the value of cnt, which was storing the total number of words in dictionary
    return cnt;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    //tmp points to the same node as cursor, then cursor moves forward by 1, while tmp is freed
    //This is done until the end of list
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];

        while (cursor != NULL)
        {
            node *tmp;
            tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    return true;
}
