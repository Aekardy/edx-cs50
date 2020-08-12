def main():
    # Ask user for input, and check if it is in 1-8 (inclusive)
    while True:
        h = input("Height: ")
        # Check if h is a numeric value
        if h.isnumeric():
            if int(h) in range(1, 9):
                # Call halfPyramid to print the required pyramid
                halfPyramid(int(h))
                break
    
    
# Function for printing the pyramid
def halfPyramid(h):
    j = h
    # Go upto the required height, print spaces and #s accordingly
    for i in range(h):
        print(" " * (j - 1) + "#" * (i + 1))
        j -= 1


# Call main to run the program
main()