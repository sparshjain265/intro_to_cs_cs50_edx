#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>

bool key_check(string);
string encode(string, string);

// main with command line arguments
int main(int argc, string argv[])
{
    // if more or less arguments, tell the correct usage and return 1
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    // store key and it's length
    string key = argv[1];
    int length = strlen(key);

    // sanity check for key
    if (length != 26)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    // make key case insensitive
    for (int i = 0; i < length; i++)
    {
        key[i] = toupper(key[i]);
    }

    // check for all letters exactly once
    if (!key_check(key))
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    // prompt plaintext
    string plaintext = get_string("plaintext: ");

    // encode the plaintext
    string ciphertext = encode(plaintext, key);

    // print the ciphertext
    printf("ciphertext: %s", ciphertext);

    // print newline
    printf("\n");
    return 0;
}

// check if all letters appear exactly once in the key
bool key_check(string key)
{
    // check presence of each character
    for (char c = 'A'; c <= 'Z'; c++)
    {
        bool flag = false;
        for (int i = 0; i < 26; i++)
        {
            if (key[i] == c)
            {
                flag = true;
                break;
            }
        }
        // if char not present, return false
        if (flag == false)
        {
            return false;
        }
    }
    // if all present, return true
    return true;
}

// encodes a given plaintext using substitution's encryption with the
// given key
string encode(string plaintext, string key)
{
    // get length of plaintext
    int length = strlen(plaintext);

    // initialize ciphertext with plaintext
    string ciphertext = plaintext;

    // loop over each character
    for (int i = 0; i < length; i++)
    {
        // encode if small letter
        if (plaintext[i] >= 'a' && plaintext[i] <= 'z')
        {
            char c = plaintext[i] - 'a';
            c = key[(int) c];
            c = tolower(c);
            ciphertext[i] = c;
        }
        // encode if capital letter
        else if (plaintext[i] >= 'A' && plaintext[i] <= 'Z')
        {
            char c = plaintext[i] - 'A';
            c = key[(int) c];
            ciphertext[i] = c;
        }
    }

    return ciphertext;
}