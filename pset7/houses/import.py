# Reads a CSV file into a database
from cs50 import SQL
from sys import argv, exit
import csv


# Check if correct command line arguments given
if len(argv) != 2:
    print("Usage: python import.py file.csv")
    exit(1)

# Read the database into a variable to execute sql commands
db = SQL("sqlite:///students.db")

with open(argv[1]) as csvfile:
    file = csv.DictReader(csvfile)
    for row in file:
        # Take in the name, split it into individual words separated by spaces
        name = row["name"].split()
        if len(name) == 3:
            pass                  # Do nothing
        else:
            name.append(None)                       
            name[2], name[1] = name[1], name[2]    # Swap the last None/NULL value to the middle position 
        house = row["house"]
        birth = row["birth"]
        # Command to insert into table
        db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?);",
                   name[0], name[1], name[2], house, birth)