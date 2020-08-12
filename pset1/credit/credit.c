//The header files
#include <cs50.h>    //get_long
#include <stdio.h>
#include <math.h>  //pow

//The main block
int main(void)
{
    long int num;                 //To store the number
    do
    {
        num = get_long("Number: ");
    }
    while (num < 0);            //The loop repeats until a valid number is entered
    int sum_even = 0;         //Store the sum of individual digit in even's place after multiplying by 2
    int sum_odd = 0;          //Store the sum of digits in odd places
    int digit_even = 0;       //Find the number of digits in even places
    int digit_odd = 0;        //Find the number of digits in the odd places
    long int num_copy = num;    //Make a copy of the original number, so that the original remains intact  
    num_copy /= 10;             //Go to the second position to begin the even loop
    //The even loop
    while (num_copy != 0)
    {
        int num_even = (2 * (num_copy % 10));        //This takes the digit in the even's place and multiplies by 2
        while (num_even != 0)
        {
            sum_even += (num_even % 10);         //This loop finds the sum if the number multiplied by 2 is more than one digit long
            num_even /= 10;
        }
        num_copy /= 100;           //This makes the number smaller by 2 digits, for getting to the next even's place
        digit_even++;              //This increments the even digit's count
    }
    num_copy = num;              //This again makes a copy of num, as the previous was destroyed
    //The odd loop
    while (num_copy != 0)
    {
        sum_odd += (num_copy % 10);        //This sums the digits of the odd's place
        num_copy /= 100;                 //This makes the number smaller by 2 digits, to go to the next odd's place
        digit_odd++;                //This increments the odd digit's count
    }
    int total_sum = sum_odd + sum_even;                     //Finds the total sum from odd and even calculated above
    int total_digit = digit_odd + digit_even;                //Finds the length (total number of digits) of the input number
    int first_digits = (num / pow(10, (total_digit - 2)));      //This stores the first two digits of the number 
    int first_digit = (num / pow(10, (total_digit - 1)));        //This stores the first digit of the number
    //The checking as per given instruction
    //If any conditions fail, the card is stated INVALID
    if (((total_sum % 10) == 0) && ((total_digit >= 13) && (total_digit <= 16))) 
    {
        if ((total_digit == 15) && ((first_digits == 34) || (first_digits == 37)))
        {
            printf("AMEX\n");
        }
        else if ((total_digit == 16) && ((first_digits >= 51) && (first_digits <= 55)))
        {
            printf("MASTERCARD\n");
        }
        else if (((total_digit >= 13) && (total_digit <= 16)) && (first_digit == 4))
        {
            printf("VISA\n");
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