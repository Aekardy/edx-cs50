//The header files
#include <cs50.h>
#include <stdio.h>
#include <string.h>      //strlen
#include <ctype.h>      //toupper, tolower, isupper, islower, isalpha

//The function declarations
void substitution(string ptext, string argv[]);

//The main block
int main(int argc, string argv[])
{
    //If user gives no or more than one value for key 
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;                   //Exit with return state 1
    }
    
    //If the key is not made up alphabets 
    for (int i = 0; argv[1][i] != '\0'; i++)
    {
        if (!(isalpha(argv[1][i])))
        {
            printf("Key must contain only alphabetic characters.\n");
            return 1;
        }
    }
    
    //If the key is not 26 characters long
    if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    //If characters are not unique
    for (int i = 0; argv[1][i] != '\0'; i++)
    {
        for (int j = i + 1; argv[1][j] != '\0'; j++)
        {
            if (tolower(argv[1][i]) == tolower(argv[1][j]))
            {
                printf("Key must not contain repeated characters.\n");
                return 1;
            }
        }
    }

    //Get the plaintext from user
    string ptext = get_string("plaintext: ");

    //Convert to ciphertext
    substitution(ptext, argv);
    return 0;                  //No error occurred
}

void substitution(string ptext, string argv[])
{
    int n = strlen(ptext);             //Store the length of the string
    char ctext[n];                    //Make a ctext char array to hold the string ciphertext

    //Check individual characters and apply the cipher
    for (int i = 0; i < n; i++)
    {
        if (islower(ptext[i]))
        {
            ctext[i] = tolower(argv[1][ptext[i] - 97]);      //take the index value of ptext, convert to lower case and store it back 
        }
        else if (isupper(ptext[i]))
        {
            ctext[i] = toupper(argv[1][ptext[i] - 65]);      //same thing as before, only convert to upper case
        }
        else
        {
            ctext[i] = ptext[i];                           //if not alpha character, then store the same thing
        }
    }
    ctext[n] = '\0';                              //For terminating the string
    printf("ciphertext: %s\n", ctext);
}