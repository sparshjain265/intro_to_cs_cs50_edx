#include <stdio.h>
#include <cs50.h>

// main function
int main(void)
{
    // Get name from user
    string name = get_string("What is your name?\n");
    // Hello user
    printf("hello, %s\n", name);
}