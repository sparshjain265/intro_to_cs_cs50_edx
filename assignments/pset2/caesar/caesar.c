#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>

int get_key(string);
string encode(string, int);

// main with command line arguments
int main(int argc, string argv[])
{
    // if more or less arguments, tell the correct usage and return 1
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // store key as string and it's length
    string key_string = argv[1];
    int key_length = strlen(key_string);

    // sanity check for non-negative integer input
    for (int i = 0; i < key_length; i++)
    {
        // return for any non-valid digit
        if (key_string[i] < '0' || key_string[i] > '9')
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }

    // convert key to int
    int key = get_key(key_string);

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

// gives an integer key from a string representing key
int get_key(string key_string)
{
    // return remainder by 26 to handle overflow challenges
    return atoi(key_string) % 26;
}

// encodes a given plaintext using caesar's encryption with the
// given key
string encode(string plaintext, int key)
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
            c = (c + key) % 26;
            c += 'a';
            ciphertext[i] = c;
        }
        // encode if capital letter
        else if (plaintext[i] >= 'A' && plaintext[i] <= 'Z')
        {
            char c = plaintext[i] - 'A';
            c = (c + key) % 26;
            c += 'A';
            ciphertext[i] = c;
        }
    }

    return ciphertext;
}