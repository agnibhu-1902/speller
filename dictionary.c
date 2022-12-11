// Implements a dictionary's functionality

#include <stdbool.h>

#include "dictionary.h"

#include <ctype.h>

#include <string.h>

#include <stdlib.h>

#include <stdio.h>

//Function prototype
unsigned int hash(const char *);

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 5000;

//Counter for words in the dictionary
unsigned int word_count = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    char nword[LENGTH + 1];
    strcpy(nword, word);
    for (int i = 0; nword[i] != '\0'; i++)
    {
        nword[i] = tolower(nword[i]);
    }
    unsigned int pos = hash(nword);
    if (table[pos] == NULL)
    {
        return false;
    }
    else
    {
        node *cursor = table[pos];
        do
        {
            if (strcmp(cursor -> word, nword) == 0)
            {
                return true;
            }
            else
            {
                cursor = cursor -> next;
            }
        }
        while (cursor != NULL);
    }
    return false;
}

// Hashes word to a number (Source: https://cs50.stackexchange.com/questions/37209/pset5-speller-hash-function)
unsigned int hash(const char *word)
{
    // TODO
    unsigned int hash_value = 0;
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        hash_value = (hash_value << 2) ^ word[i];
    }
    return hash_value % N; //N is size of hashtable
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    char word_buffer[LENGTH + 1];
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }
    else
    {
        while (fscanf(dict, "%s", word_buffer) != EOF)
        {
            node *word_node = malloc(sizeof(node));
            if (word_node == NULL)
            {
                fclose(dict);
                return false;
            }
            strcpy(word_node -> word, word_buffer);
            word_count++;
            unsigned int pos = hash(word_node -> word);
            if (table[pos] == NULL)
            {
                table[pos] = word_node;
                word_node -> next = NULL;
            }
            else
            {
                word_node -> next = table[pos];
                table[pos] = word_node;
            }
        }
    }
    if (fscanf(dict, "%s", word_buffer) == EOF)
    {
        fclose(dict);
        return true;
    }
    fclose(dict);
    return false;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    if (word_count > 0)
    {
        return word_count;
    }
    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    unsigned int count = 0;
    for (int i = 0; i < N; i++)
    {
        if (table[i] == NULL)
        {
            count++;
            continue;
        }
        node *cursor = table[i];
        node *tmp = table[i];
        do
        {
            cursor = cursor -> next;
            free(tmp);
            tmp = cursor;
        }
        while (tmp != NULL);
    }
    if (count < N)
    {
        return true;
    }
    return false;
}