from functools import reduce

f = open("input.txt", "r")
fileLines = f.read().split("\n")
f.close()

part1Lines = list(fileLines)
part2Lines = list(fileLines)

#part - 1
badNumber = None;

preamble = []
for i in range(25):
  line = part1Lines.pop(0).strip()
  number = int(line)
  preamble.append(number)

while len(part1Lines) > 0:
    number = int(part1Lines.pop(0).strip())
    diffList = list(map(lambda x: number - x, preamble))
    diffInPreamble = list(map(lambda x: x in preamble, diffList))
    okNumber = reduce(lambda x,y : x or y, diffInPreamble, False)

    if not okNumber:
      badNumber = number
      print(number)
      print(preamble)
      print(diffList)
      break;
    else:
      preamble.pop(0)
      preamble.append(number)

#part - 2
if badNumber:
  targetNumber = badNumber

  part2Numbers = list(map(lambda x : int(x.strip()), part2Lines))
  _range = [0,0]
  foundRange = False

  while not foundRange:
    sublist = part2Numbers[_range[0]:_range[1]]
    sumSubList = sum(sublist)
    #print(targetNumber - sumSubList)
    if sumSubList < targetNumber:
      _range[1] = _range[1] + 1
      foundRange=False
    elif sumSubList > targetNumber:
      _range[0] = _range[0] + 1
      foundRange=False
    else:
      foundRange=True
    
    _sublist = part2Numbers[_range[0]:_range[1]]
    if len(_sublist) < 2:
      foundRange = False
      _range[1] = _range[1] + 1
  
  actRange = part2Numbers[_range[0]:_range[1]]
  min_rangeNum = min(actRange)
  max_rangeNum = max(actRange)
  
  print(actRange)
  print(min_rangeNum)
  print(max_rangeNum)
  print(min_rangeNum + max_rangeNum)