from cs50 import get_string
import re


def main():
    credit = get_string("Number: ")

    if visa(credit):
        print("VISA")
    elif amex(credit):
        print("AMEX")
    elif mc(credit):
        print("MASTERCARD")
    else:
        print("INVALID")


def visa(credit):
    # 13 or 16 digits
    if len(credit) != 13 and len(credit) != 16:
        return False

    # start with 4
    if not re.search(r"^4", credit):
        return False

    return checksum(credit)


def amex(credit):
    # 15 digits
    if len(credit) != 15:
        return False

    # start with 34 or 37
    if not re.search(r"^3[47]", credit):
        return False

    return checksum(credit)


def mc(credit):
    # 16 digits
    if len(credit) != 16:
        return False

    # start with 51-55
    if not re.search(r"^5[1-5]", credit):
        return False

    return checksum(credit)


def checksum(credit):
    # if not all digits, return false
    if not re.search(r"^[0-9]*$", credit):
        return False

    flag = True

    total = 0

    # for each digit
    for c in credit[::-1]:
        # if flag
        if flag:
            total += int(c)
            flag = not flag
        else:
            d = int(c) * 2
            total += (d % 10) + (d // 10)
            flag = not flag

    return (total % 10) == 0


main()