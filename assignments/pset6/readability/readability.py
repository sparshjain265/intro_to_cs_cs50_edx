from cs50 import get_string
import re


def main():
    # Get text
    text = get_string("Text: ")

    # count letters, words, and sentences
    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)

    # print values to test
    print("Letters:", letters)
    print("Words:", words)
    print("Sentences:", sentences)

    # calculate index
    index = get_index(letters, words, sentences)

    # print appropriate result
    if index < 1:
        print("Before Grade 1")
    elif index < 16:
        print("Grade", index)
    else:
        print("Grade 16+")


def count_letters(text):
    return len(re.findall(r"[a-zA-Z]", text))


def count_words(text):
    return len(re.split(r" ", text))


def count_sentences(text):
    return len(re.findall(r"[.!?]", text))


def get_index(letters, words, sentences):
    L = 100.0 * letters / words
    S = 100.0 * sentences / words
    index = round(0.0588 * L - 0.296 * S - 15.8)
    return int(index)


main()