import re
#separate ID from list of IDs
idListSep = r"contain"
remBag = r"\ bags?"
remDot = r"\."
remNoOther = r"no other"

#read doc
file = open("input.txt", "r")
docStr = file.read()
file.close()

# You can manually specify the number of replacements by changing the 4th argument
docStr = re.sub(idListSep, "-", docStr, 0, re.MULTILINE)
docStr = re.sub(remBag, "", docStr, 0, re.MULTILINE)
docStr = re.sub(remDot, "", docStr, 0, re.MULTILINE)
docStr = re.sub(remNoOther, "", docStr, 0, re.MULTILINE)

docLines = docStr.split("\n")
bagsDic = {}
containedIn = {}
for line in docLines:
    line = line.strip()
    listSepIdx = line.find("-")
    bagName = line[:listSepIdx - 1]
    bagList = line[listSepIdx + 2:]
    bagsDic[bagName] = []
    if not bagName in containedIn:
        containedIn[bagName] = []
    if bagList:
        bagList = bagList.split(",")
        for bag in bagList:
            bag = bag.strip()
            _bagNumber = int(bag[0])
            _bagName = bag[1:].strip()
            bagsDic[bagName].append((_bagNumber, _bagName))
            if not _bagName in containedIn:
                containedIn[_bagName] = []
            containedIn[_bagName].append(bagName)

# task 1
print()

visited = []
toVisit = containedIn["shiny gold"]

# print(visited)
# print(toVisit)
# print()

for node in toVisit:
    for nodeLink in containedIn[node]:
        if not nodeLink in toVisit:
            toVisit.append(nodeLink)
    visited.append(node)

print("T1:({})".format(len(visited)))

#task2
print()

#print(bagsDic)

start = (1, "shiny gold")


def recFun(bag):
    _bagName = bag[1]
    _bagNumber = bag[0]
    sub_bags = bagsDic[_bagName]
    if len(sub_bags) > 0:
        sumBagNum = 0
        for sub_bag in sub_bags:
            _sub_bag = recFun(sub_bag)
            _sub_bagNum = _bagNumber * _sub_bag[0]
            sumBagNum = sumBagNum + _sub_bagNum
        sumBagNum = sumBagNum + _bagNumber
        return (sumBagNum, _bagName)

    else:
        return bag


print(recFun(start)[0] - 1)

# if(idx > 3):
#   break

# print(toVisit)
# justBags = list(map(lambda x: x[0], toVisit))
# print("T2:({})".format(sum(justBags)))
