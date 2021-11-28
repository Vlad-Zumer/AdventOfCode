f = open("input.txt","r")
fileData = f.read()
f.close()

fileMat = fileData.split("\n")
fileMat = list(map(lambda x: list(x), fileMat))

free = "L"
occupied = "#"
floor = "."

import copy

def iteration(_inmat):
  outmat = copy.deepcopy(_inmat)
  for (line, _line) in enumerate(_inmat):
    for (col, _) in enumerate(_line):
      neighbours = []

      if line > 0:
        neighbours.append(_inmat[line-1][col])
        if col > 0:
          neighbours.append(_inmat[line-1][col-1])
        if col < len(_line)-1:
          neighbours.append(_inmat[line-1][col+1])

      if line < len(_inmat)-1:
        neighbours.append(_inmat[line+1][col])
        if col > 0:
          neighbours.append(_inmat[line+1][col-1])
        if col < len(_line)-1:
          neighbours.append(_inmat[line+1][col+1])
      
      if col > 0:
        neighbours.append(_inmat[line][col-1])
      if col < len(_line)-1:
        neighbours.append(_inmat[line][col+1])

      occupiedNum = sum(list(map(lambda x: 1 if x == occupied else 0, neighbours)))
      # print((line,col))
      # print(neighbours)
      # print(occupiedNum)

      if _inmat[line][col] == free and occupiedNum == 0:
        outmat[line][col] = occupied
      if _inmat[line][col] == occupied and occupiedNum > 3:
        outmat[line][col] = free
      
  return outmat


ite = iteration(fileMat)
changed = not ite == fileMat

while changed:
  newIte = iteration(ite)
  changed = not newIte == ite
  ite = newIte

acc = 0
for line in ite:
  for chair in line:
    if chair == occupied:
      acc = acc +1

print(acc)