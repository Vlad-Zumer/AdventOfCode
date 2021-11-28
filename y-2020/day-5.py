
def position(posStr, arrMargins):
  low, high = arrMargins;
  pos = (low, high)
  for letter in posStr:
    nLow, nHigh = pos
    num = nHigh - nLow + 1
    if letter == "H":
      nLow = nHigh - (num//2 - 1)
    elif letter == "L":
      nHigh = nLow + (num//2 - 1)
    pos = (nLow, nHigh)

  return pos

def calcId(row,seat):
  return (row * 8 + seat)

file = open("input.txt","r")
fileLines = file.readlines()
file.close()

fileLines = list(map(lambda x: x.strip(), fileLines))
fileLines = list(map(lambda x: x.replace("F","L").replace("B","H").replace("R","H"), fileLines))

rowRange = (0,127)
seatRange = (0,7)

maxId = -1
ids = []

for line in fileLines:
  row = line[:7]
  seat = line[-3:]
  rowPos = position(row,rowRange)
  seatPos = position(seat,seatRange)

  if rowPos[0] == rowPos[1] and seatPos[0] == seatPos[1] :
    rowPos = rowPos[0]
    seatPos = seatPos[0]
    thisId = calcId(rowPos,seatPos)
    ids.append(thisId)
    if thisId > maxId:
      maxId = thisId
  else:
    print("ERROR (seat({} - {}) - row({} - {}))".format(seatPos[0], seatPos[1],rowPos[0], rowPos[1]))

mySeatIds = []
for sid in ids:
  isBefore =  (not (sid - 1) in ids) and ((sid - 2) in ids)
  isAfter = (not (sid + 1) in ids) and ((sid + 2) in ids)
  if isBefore:
    mySeatIds.append(sid-1)
  elif isAfter:
    mySeatIds.append(sid+1)

mySeatIds = set(mySeatIds)

print("max id:{}".format(maxId))
print("my pos seat id:{}".format(mySeatIds))