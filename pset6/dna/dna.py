from sys import exit, argv
import csv


def checkDNA():
    reader_val = {}        # A dict to store the max STR from the sequence file
    f = 0                  # A flag value to check if someone has been found or not
    
    # Open the CSV file, as well as the sequence file, and check for DNA matches
    with open(argv[1]) as csvfile:
        # DictReader creates a dict from the keys provided at the first row of CSV file
        reader = csv.DictReader(csvfile)                 
        # Read the sequence text as string, replacing any trailing newline
        sqn = open(argv[2]).read().replace('\n', '')     
        
        # This iterates over every row of CSV file
        for row in reader:
            fl = 0            # This checks that if any column doesn't match, go to the next row 
            
            for key, val in row.items():            
                c_list = []                   # This list stores all the counts of consequtive STR found from sequence
                if key == "name":         # If key is name, skip 
                    continue
                for i in range(len(sqn)):      
                    c = 0                  # This is the counter that counts all the matched strings
                    if sqn[i:len(key) + i] == key:
                        i_sqn = i           
                        while sqn[i_sqn:len(key) + i_sqn] == key:            # Check chunks of strings as per length of key,
                            c += 1                                           # if equal, increment c by 1
                            i_sqn += len(key)
                    # Append the total number of matched strings found to the list for a particular index    
                    c_list.append(c)
                # Take the max count from the list, and store that corresponding to the current key
                reader_val[key] = max(c_list)         
                # Check if that value matches the value (converted to int) from database, if yes continue for next key, else break out
                if reader_val[key] != int(val):
                    fl = 1                  # Set fl to 1 as this row didn't match the database
                    break
            if fl != 1:          # If all  keys were successfully matched with database
                print(row["name"])
                f = 1           # Set f to 1 as match was found
        if f == 0:           
            print("No Match")
    

def main():
    # Check the correct usage format 
    if (len(argv) != 3):
        print("Usage: python dna.py databases sequences")
        exit(1)

    # The function checks for DNA matches
    checkDNA()
        
    
main()