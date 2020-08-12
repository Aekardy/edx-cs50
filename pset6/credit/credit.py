from cs50 import get_string


def main():
    # Use a string so that individual digits can be stored in list
    while True:
        n = get_string("Number: ")
        if n.isnumeric():                 # If not numeric string, iterate again
            break
    
    # sumO ,sumE stores the sum (according to rules) at odd and even places
    # num is a list of all the digits of the numbers
    sumO = sumE = 0
    num = [int(num) for num in n]

    for i in (range(len(num))):
        if i % 2 == 0:
            sumO += num[len(num) - i - 1]
        else:
            # For consequent digits, starting from second multiply by 2 and add digits (Luhn's algo) 
            sumE += ((2 * num[len(num) - i - 1]) % 10) + ((2 * num[len(num) - i - 1]) // 10)
            
    # Check according to Luhn's algorithm 
    if (sumE + sumO) % 10 == 0 and len(num) in range(13, 17):
        if len(num) == 15 and num[0] == 3 and (num[1] == 4 or num[1] == 7):
            print("AMEX")
        elif len(num) == 16 and num[0] == 5 and (num[1] in range(1, 6)):
            print("MASTERCARD")
        elif num[0] == 4:
            print("VISA")
        else:
            print("INVALID")
    else:
        print("INVALID")
    
    
main()