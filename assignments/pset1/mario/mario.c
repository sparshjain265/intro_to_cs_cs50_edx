#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Get Height
    int height;
    do
    {
        height = get_int("Height: ");
    } // Prompt until good input
    while (height < 1 || height > 8);

    // Build Pyramid
    for (int i = 0; i < height; i++)
    {
        // Print appropriate space
        for (int j = 1; j < height - i; j++)
        {
            printf(" ");
        }

        // Print appropriate '#'
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }

        // Print two spaces
        for (int j = 0; j < 2; j++)
        {
            printf(" ");
        }

        // Print appropriate '#'
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }

        printf("\n");
    }
}