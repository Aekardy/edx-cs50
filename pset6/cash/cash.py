# Get the get_float function to read floating point numbers
from cs50 import get_float


def main():
    # Get the positive input from user
    while True:
        change = get_float("Change Owed: ")
        if change >= 0:
            break
    
    # Round up the fractional value to int
    cents = round(change * 100)
    coins = 0
    
    # For each, check for the greatest denomination, and accordingly count the coins
    if cents // 25:
        quarters = cents // 25
        cents -= quarters * 25
        coins += quarters
    if cents // 10:
        dimes = cents // 10
        cents -= dimes * 10
        coins += dimes
    if cents // 5:
        nickels = cents // 5
        cents -= nickels * 5
        coins += nickels
    pennies = cents
    coins += pennies
    
    # Print the coins 
    print(f"{coins}")
    
    
# Call the main function to run the program    
main()