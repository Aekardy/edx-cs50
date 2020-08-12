def main():
    # Ask user for input, and check if it is in 1-8 (inclusive)
    while True:
        h = input("Height: ")
        # Check if h is a numeric value
        if h.isnumeric():
            if int(h) in range(1, 9):
                # Call d_halfPyramid to print the required double half pyramid
                d_halfPyramid(int(h))
                break


# Function for printing the pyramid
def d_halfPyramid(h):
    j = h
    # Go upto the required height, print spaces and #s accordingly
    for i in range(h):
        print((" " * (j - 1) + "#" * (i + 1)), end="")
        print("  " + "#" * (i + 1))
        j -= 1


# Call main to run the program
main()

