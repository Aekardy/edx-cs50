//The header files
#include <cs50.h>      
#include <stdio.h>
#include <ctype.h>     //isdigit, isupper, islower
#include <string.h>    //strlen
#include <stdlib.h>    //atoi

//Function declarations
void caesar(string ptext, int k);

//The main block
int main(int argc, string argv[])
{
    //If user gives no or more than one value for key
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;                   //Exit with return state 1
    }
    
    //If the key is not made up digits
    for (int i = 0, n = strlen(argv[1]); argv[1][i] != '\0'; i++)
    {
        if (!(isdigit(argv[1][i])))
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }
    
    //Convert a valid key to int
    int k = atoi(argv[1]);
    
    //Get the plaintext from user
    string ptext = get_string("plaintext: ");
    
    //Convert to ciphertext according to the key
    caesar(ptext, k);
    return 0;                  //No error occurred
} 

void caesar(string ptext, int k)
{
    int n = strlen(ptext);             //Store the length of the string
    char ctext[n];                    //Make a ctext char array to hold the string ciphertext
    
    //Check individual character, then applies the caesar formula to letters
    for (int i = 0; i < n; i++)
    {
        if (isupper(ptext[i]))
        {
            ctext[i] = ((ptext[i] - 65 + k) % 26) + 65;            //'A' starts at 65, so index it to zero, then reset the index
        }
        else if (islower(ptext[i]))
        {
            ctext[i] = ((ptext[i] - 97 + k) % 26) + 97;           //'a' starts at 97, so index it to zero, then reset the index
        }
        else 
        {
            ctext[i] = ptext[i];                                 //If not alphabetical letters then do not change anything
        }
    }
    ctext[n] = '\0';                                   //This marks the end of the text
    printf("ciphertext: %s\n", ctext);  
}