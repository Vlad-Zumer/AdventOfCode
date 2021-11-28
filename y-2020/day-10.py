f = open("input.txt","r")
fileData = f.read()
f.close()

fileLines = fileData.split("\n")
fileNumbers = list(map(lambda x: int(x), fileLines))
fileNumbers.append(0)
fileNumbers.append(max(fileNumbers) + 3)
fileNumbers.sort()


numbersZip = list(zip(fileNumbers,fileNumbers[1:]))

numbersDifs = list(map(lambda x: x[1] - x[0], numbersZip))

print("P1:({})".format((numbersDifs.count(1)*numbersDifs.count(3))))

from collections import Counter
# counts the appearances of key in a list
paths = Counter({0:1}) # 1 path in node 1

for jolt in fileNumbers:  
    paths[jolt+1] += paths[jolt] # add the number paths to this node(JOLT) to the next node(JOLT + 1)
    paths[jolt+2] += paths[jolt] # add the number paths to this node(JOLT) to the node(JOLT + 2)
    paths[jolt+3] += paths[jolt] # add the number paths to this node(JOLT) to the node(JOLT + 3)

print("P2:({})".format(paths[max(fileNumbers)]))