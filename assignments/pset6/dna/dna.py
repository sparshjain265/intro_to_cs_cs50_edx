from sys import argv, exit
import csv
import re


def main():
    if len(argv) != 3:
        print("Wrong Usage")
        exit(1)

    # get database file name
    db = argv[1]
    # get sequence file name
    sq = argv[2]

    # open database
    with open(db, "r") as database:
        reader = csv.DictReader(database)
        # get list of STRs
        STR = reader.fieldnames[1:]
        # str_re = map(lambda x: re.compile(x + '*'), STR)
        # code of person from sequence
        code = {}

        # open dna sequence
        with open(sq, "r") as sequence:
            # get dna sequence
            seq = sequence.__next__().strip()

            # get longest repeat for all sequences
            for s in STR:
                r = re.compile(r'(?:%s)+' % s)
                l = r.findall(seq)
                l = map(len, l)
                code[s] = max(list(l) + [0]) // len(s)

        # check with each entry
        for row in reader:
            flag = True
            for s in STR:
                if int(row[s]) != code[s]:
                    flag = False
                    break
            if flag:
                print(row['name'])
                exit(0)

        # if no name
        print("No match")


main()