//The header files
#include <cs50.h>      //get_string
#include <stdio.h>
#include <math.h>      //round

//The function declarations 
int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
int CLindex(int letter, int sentence, int word);

//The main block
int main(void)
{
    //Get the text in variable text
    string text = get_string("Text : ");
    
    //Count the total letters in the text
    int letter = count_letters(text);

    //Count the total words in text
    int word = count_words(text);

    //Count the number of sentences in text
    int sentence = count_sentences(text);

    //The Coleman-Liau index
    int index = CLindex(letter, sentence, word);

    if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}

int count_letters(string text)
{
    int count = 0;
    //Loop over all the text characters and check for condition, if true increment count by 1 to get total letters
    for (int i = 0; text[i] != '\0'; i++)
    {
        if ((text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z'))
        {
            count++;
        }
        
    }
    return count;
}

int count_words(string text)
{
    int count = 0;
    //Loop over all the characters and check for spaces, if found increment count by 1 to get all words separated by spaces
    //Assuming text don't start or end with space, no multiple spaces among words
    for (int i = 0; text[i] != '\0'; i++)
    {
        if (text[i] == ' ')
        {
            count++;
        }
    }
    return (count + 1);         //The +1 is for the last word
}

int count_sentences(string text)
{
    int count = 0;
    //Loop over all characters, check for '.', '!', '?' for end of sentence, increment counter
    //Assumptions include Mr. Dudley. is counted as two sentences
    for (int i = 0; text[i] != '\0'; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            count++;
        }
    }
    return count;
}

int CLindex(int letter, int sentence, int word)
{
    float L = ((float) letter / word) * 100;           //Number of letters per 100 words
    float S = ((float) sentence / word) * 100;         //Number of sentences per 100 words
    float index = 0.0588 * L - 0.296 * S - 15.8;       //The Coleman-Liau indexdebug
    return round(index);               //Round index to nearest value and return it as an int
}