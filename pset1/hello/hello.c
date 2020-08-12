//These are the header files 
#include <stdio.h>
#include <cs50.h>

//Main function starts
int main(void)
{
    //get_string, string is included in cs50.h
    //stores the name in 'name' variable
    string name = get_string("Tell me your name. \n");   
    //prints the concatenated message along with name
    printf("Hello, %s \n", name);                         
}