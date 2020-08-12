//The header files 
#include <cs50.h>
#include <stdio.h>

//The main block
int main(void)
{
    //Get the input number between 1 and 8, a do-while loop is used since we always start with a prompt
    //get_int is part of cs50.h
    int n;
    do
    {
        n = get_int("Enter the height (integers between 1 and 8 inclusive): ");
    }
    while (n < 1 || n > 8);
    //The for loop builds the required right-aligned pyramid
    for (int i = 0; i < n; i++)
    {
        //This for loop is for printing the extra spaces
        for (int j = 0; j < n - i - 1; j++)
        {
            printf(" ");
        }
        //This is for the #
        for (int j = 0; j <= i; j++)
        {
            printf("#"); 
        }
        //This prints the gap between two pyramids, of 2 spaces
        printf("  ");
        //This prints the right side pyramid
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }
        printf("\n");    //This new line is made after each row completion
        
    }
    
}