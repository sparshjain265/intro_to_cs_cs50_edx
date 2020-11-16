#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int count_letters(string);
int count_words(string);
int count_sentences(string);
int get_index(int, int, int);

int main(void)
{
    // Get text from user
    string text = get_string("Text: ");

    // count letters, words, and sentences.
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);
    // print values to test
    printf("Letters: %i\n", letters);
    printf("Words: %i\n", words);
    printf("Sentences: %i\n", sentences);

    // calculate index
    int index = get_index(letters, words, sentences);

    // print appropriate result
    if (index < 1)
    {
        // If grade <= 0
        printf("Before Grade 1\n");
    }
    else if (index < 16)
    {
        // If 0 < grade < 16
        printf("Grade %i\n", index);
    }
    else
    {
        // If grade >= 16
        printf("Grade 16+\n");
    }
}

/**
 * Function count_letters
 * ----------------------
 * counts the number of letters in a given text
 *
 * text: string value
 *
 * returns: number of letters
 */
int count_letters(string text)
{
    // get the length of the text
    int length = strlen(text);
    int count = 0;
    for (int i = 0; i < length; i++)
    {
        // small letters increment the count
        if (text[i] >= 'a' && text[i] <= 'z')
        {
            count++;
        }
        // capital letters increment the count
        else if (text[i] >= 'A' && text[i] <= 'Z')
        {
            count++;
        }
    }
    return count;
}

/**
 * Function: count_words
 * ---------------------
 * counts the number of <space> separated words in a given text
 *
 * text: string value
 *
 * returns: number of words
 */
int count_words(string text)
{
    // get the length of the text
    int length = strlen(text);
    int count = 0;
    for (int i = 0; i < length; i++)
    {
        // space increments the count
        if (text[i] == ' ')
        {
            count++;
        }
    }
    // don't forget the last word that does not end in ' '
    count++;
    return count;
}

/**
 * Function: count_sentences
 * -------------------------
 * counts the number of sentences that end in '.' or '!' or '?'
 * in a given text
 *
 * text: string value
 *
 * returns: number of sentences
 */
int count_sentences(string text)
{
    // get the length of the text
    int length = strlen(text);
    int count = 0;
    for (int i = 0; i < length; i++)
    {
        // period increments the count
        if (text[i] == '.')
        {
            count++;
        }
        // exclaimation increments the count
        else if (text[i] == '!')
        {
            count++;
        }
        // question increments the count
        else if (text[i] == '?')
        {
            count++;
        }
    }
    return count;
}

/**
 * Function: get_index
 * -------------------
 * gives readability index of a text given the number of letters,
 * words, and sentences in the text
 *
 * letters: number of letters
 *
 * words: number of words
 *
 * sentences: number of sentences
 *
 * returns: readability index of the text
 */
int get_index(int letters, int words, int sentences)
{
    // get letters per 100 words
    float L = 100.0 * letters / words;
    // get sentences per 100 words
    float S = 100.0 * sentences / words;
    // get index from formula and round it off
    int index = round(0.0588 * L - 0.296 * S - 15.8);

    return index;
}