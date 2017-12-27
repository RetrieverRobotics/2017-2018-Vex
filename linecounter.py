import os

def main():
    path = input("enter directory\n")
    files = os.listdir(path)

    total = 0
    for filename in files:
        file = open(os.path.join(path,filename))
        lines = file.readlines()
        total += len(lines)
        file.close()

    print(total)

main()
