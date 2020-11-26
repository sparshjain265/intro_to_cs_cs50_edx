# import required libraries
from sys import argv, exit
import csv
import cs50


# if improper usage, return error
if len(argv) != 2:
    print("Usage: python import.py <csv_filename>")
    exit(1)

# connect to the database
db = cs50.SQL("sqlite:///students.db")

# extract filename
filename = argv[1]

# open file
with open(filename, 'r') as file:
    reader = csv.DictReader(file)

    # for each row in the file
    for row in reader:
        # extract details
        name = row['name'].split()
        house = row['house']
        birth = row['birth']

        # get first and last name
        first = name[0]
        last = name[-1]

        # insert into database based on existance of middle name
        if len(name) == 3:
            middle = name[1]
            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)",
                       first, middle, last, house, birth)
        else:
            db.execute("INSERT INTO students (first, last, house, birth) VALUES (?, ?, ?, ?)",
                       first, last, house, birth)

