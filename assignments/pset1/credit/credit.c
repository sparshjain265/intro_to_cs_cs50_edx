#include <cs50.h>
#include <stdio.h>
#include <math.h>

void visa(long, int);
void amex(long);
void mc(long);
bool checksum(long, int);

int main(void)
{
    // get user input
    long credit = get_long("Number: ");

    // invalid for negative numbers
    if (credit < 0)
    {
        printf("INVALID\n");
    }

    // get number of digits
    int digits = 17;
    long check = powl(10, digits - 1);
    while ((credit / check) == 0)
    {
        digits--;
        check = powl(10, digits - 1);
    }
    // Print number of digits
    printf("Digits: %i\n", digits);

    // 13 digits = visa
    if (digits == 13)
    {
        visa(credit, digits);
    }
    // 15 digits = amex
    else if (digits == 15)
    {
        amex(credit);
    }
    // 16 digits can be visa or mc
    else if (digits == 16)
    {
        // check for visa or mc
        int sanity = credit / powl(10, digits - 1);
        printf("Sanity: %i\n", sanity);
        if (sanity == 4)
        {
            visa(credit, digits);
        }
        else
        {
            mc(credit);
        }
    }
    // else invalid
    else
    {
        printf("INVALID\n");
    }
}

// for visa cards
void visa(long credit, int digits)
{
    printf("visa\n");

    // Sanity Check
    int sanity = credit / powl(10, digits - 1);
    if (sanity != 4)
    {
        printf("INVALID\n");
    }
    // Checksum
    else if (checksum(credit, digits))
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }
}

// for american express
void amex(long credit)
{
    printf("amex\n");

    int digits = 15;

    // Sanity Check
    int sanity = credit / powl(10, digits - 2);
    if (sanity == 34 || sanity == 37)
    {
        if (checksum(credit, digits))
        {
            printf("AMEX\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}

// for mastercard
void mc(long credit)
{
    printf("mc\n");

    int digits = 16;

    // Sanity Check
    int sanity = credit / powl(10, digits - 2);
    if (sanity >= 51 && sanity <= 55)
    {
        if (checksum(credit, digits))
        {
            printf("MASTERCARD\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}

// checksum
bool checksum(long credit, int digits)
{
    // Initialize sum with 0
    int sum = 0;

    // for each digit
    for (int i = 0; i < digits; i++)
    {
        // starting from right, if digit is 1,3,5,...
        if (i % 2 == 0)
        {
            // add the digit to sum
            sum += (credit % 10);
        }
        // otherwise from right, if digit is 2,4,6,...
        else
        {
            // add both the digits after doubling
            int d = (credit % 10) * 2;
            sum += (d % 10);
            sum += (d / 10);
        }
        // delete the last digit
        credit = credit / 10;
    }

    return (sum % 10) == 0;
}