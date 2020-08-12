//The header files
#include <cs50.h>   //get_float
#include <stdio.h>
#include <math.h>   //round

//The main block
int main(void)
{
    //Get the amount of change owed 
    float n;
    do
    {
        n = get_float("Enter the change owed: ");
    }
    while (n < 0);
    //Declare the various variables required
    int coins, quarters, dimes, nickels;
    //Initialize each quarter, dime, nickel to zero
    quarters = 0;
    dimes = 0;
    nickels = 0;
    int cents = round(n * 100);  //This converts the fractional value to int and rounds it
    //This blocks checks if quarters can satisfy the requirements, and how many cents are left 
    if (cents / 25 != 0)
    {
        quarters = (cents / 25);
        cents -= quarters * 25;
    }
    //This blocks checks if dimes can satisfy the requirements, and how many cents are left 
    if (cents / 10 != 0)
    {
        dimes = (cents / 10);
        cents -= dimes * 10;
    }
    //This blocks checks if nickels can satisfy the requirements, and how many cents are left 
    if (cents / 5 != 0)
    {
        nickels = (cents / 5);
        cents -= nickels * 5;
    }
    //The rest amount left should be paid using pennies
    int pennies = cents;
    //Calculate the total number of coins required
    coins = quarters + dimes + nickels + pennies;
    printf("%i \n", coins);
}