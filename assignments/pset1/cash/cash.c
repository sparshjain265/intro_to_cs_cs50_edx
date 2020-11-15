#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    // get input from user
    float dollars;
    do
    {
        dollars = get_float("How much do I owe?\n");
    }
    while (dollars < 0);

    // convert to cents
    int cents = round(dollars * 100);

    // variable to store minimum number of coins
    int coins = 0;

    // add coins till needed
    while (cents > 0)
    {
        if (cents >= 25)
        {
            cents -= 25;
            coins++;
        }
        else if (cents >= 10)
        {
            cents -= 10;
            coins++;
        }
        else if (cents >= 5)
        {
            cents -= 5;
            coins++;
        }
        else
        {
            cents--;
            coins++;
        }
    }

    // print result
    printf("%i\n", coins);
}