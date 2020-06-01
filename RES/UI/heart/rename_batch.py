import os
import sys

def main(orginalName, newName, startIndex = 1, reserved = False):
    orginalName.replace("#", r"{}")
    newName.replace("#", r"{}")
    length = len(os.listdir())
    for index in range(length):
        src = orginalName.format(startIndex + index)
        dst = newName.format(startIndex + length - index - 1) if reserved else newName.format(startIndex + index)
        #dst = f"heart ({length + startIndex - index})" if reserved else f"heart ({index})"
        os.rename(src, dst)

if __name__ == "__main__":
    length = len(sys.argv)
    parameter = sys.argv
    if parameter[0] == "-h":
        print("")
    elif parameter[0] == "-i" and parameter[2] == "-o":
        if (length == 4):
            main(parameter[1], parameter[3])
        else:
            main(parameter[1], parameter[3], int(parameter[4]), bool(parameter[5]))
    else:
        print("Invalid input")