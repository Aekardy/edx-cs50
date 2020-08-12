# Prints list of students for a given house in alphabetical order
from sys import argv, exit
from cs50 import SQL

# Check if correct command line arguments given
if len(argv) != 2:
    print("Usage: python roster.py House")
    exit(1)
    
# Read the database into a variable to execute sql commands
db = SQL("sqlite:///students.db")

roster_dict = db.execute("SELECT * FROM students WHERE House = ? ORDER BY last, first;",
                         argv[1])         # This gives a dict containing list of rows

# Check if the house exists or not (if database returns nothing)
if len(roster_dict) == 0:
    print("Sorry, the mentioned house doesn't exist!")
    exit(1)

for items in roster_dict:
    # Check if middle name exists or not, print accordingly
    if items["middle"] == None:
        print("{0} {1}, born {2}".format(items["first"], items["last"], items["birth"]))
    else:
        print("{0} {1} {2}, born {3}".format(items["first"], items["middle"], items["last"], items["birth"]))
