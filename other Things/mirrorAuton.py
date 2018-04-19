import os

def main():
    path = input("enter directory\n")
    files = os.listdir(path)

    total = 0
    for filename in files:
        file = open(os.path.join(path,filename))
        lines = file.readlines()

        for line in lines:
            index = line.find("pointTurnInc(")
            index2 = line.find("swingTurn")
            if(index != -1):
                negate(line[index+:])
            else if(index2 != -1):

            else:
                print(line)


        file.close()

main()

def negate(num):
    if(num[0] == '-'):
        return num[1:]
    else:
        return num
