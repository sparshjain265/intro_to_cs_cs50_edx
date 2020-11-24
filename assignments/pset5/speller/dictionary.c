// Implements a dictionary's functionality

#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 5381 * 26;

// Number of words in hash
unsigned int _count = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    // get index
    unsigned int index = hash(word);
    // check in the list at the index
    node *current = table[index];
    while (current)
    {
        if (strcasecmp(word, current->word) == 0)
        {
            return true;
        }
        current = current->next;
    }
    return false;
}

// Hashes word to a number
// using hash djb2, found online at http://www.cse.yorku.ca/~oz/hash.html
unsigned int hash(const char *word)
{
    // TODO
    unsigned int h = 5381;
    const char *c = word;
    while (*c)
    {
        h = ((h << 5) + h) + tolower(*c);
        c++;
    }
    return h % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // TODO
    // open file
    FILE *file = fopen(dictionary, "r");

    // if not, return false
    if (file == NULL)
    {
        return false;
    }

    // to read the word
    char word[LENGTH + 1];

    // while able to read the file
    while (fscanf(file, " %s", word) != EOF)
    {
        // get space for new word
        node *p = malloc(sizeof(node));
        // confirm memory
        if (p == NULL)
        {
            fclose(file);
            return false;
        }
        // copy the word in the node
        strcpy(p->word, word);
        // get index of the word
        unsigned int index = hash(word);
        // insert in the beginning (fast)
        p->next = table[index];
        table[index] = p;
        // increment count
        _count++;
    }

    // close file
    fclose(file);

    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return _count;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    // free all memory
    for (int i = 0; i < N; i++)
    {
        node *current = table[i];
        while (current)
        {
            node *p = current->next;
            free(current);
            current = p;
        }
    }
    return true;
}
