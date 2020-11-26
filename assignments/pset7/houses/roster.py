# import required libraries
from sys import argv, exit
import csv
import cs50


# if improper usage, return error
if len(argv) != 2:
    print("Usage: python roster.py <house>")
    exit(1)

# connect to the database
db = cs50.SQL("sqlite:///students.db")

# extract housename
housename = argv[1]

# search query
result = db.execute("SELECT * FROM students WHERE house = ? ORDER BY last, first, middle", housename)

# for each result, print appropriately
for r in result:
    if r['middle']:
        print(r['first'], r['middle'], r['last'] + ',', 'born', r['birth'])
    else:
        print(r['first'], r['last'] + ',', 'born', r['birth'])
