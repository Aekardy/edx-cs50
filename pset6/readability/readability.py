from cs50 import get_string


def main():
    # Get the input string
    text = get_string("Text: ")
    # Variables for storing letters, words, sentences count
    letter = words = sentences = 0
    
    for word in text.split():                      # The spilt just allows word to take in a full word (separated by spaces)
        for char in word:
            # Check if char is alphabet
            if char.isalpha():
                letter += 1
            # Check for end of sentences (only checking !,.,? (assumptions specified in questions)
            if char == '!' or char == '.' or char == '?':
                sentences += 1
        words += 1
    
    # The Coleman-Liau index  
    CLindex = round(0.0588 * ((letter / words) * 100) - 0.296 * ((sentences / words) * 100) - 15.8)
    if CLindex >= 16:
        print("Grade 16+")
    elif CLindex < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {CLindex}")
    
    
main()