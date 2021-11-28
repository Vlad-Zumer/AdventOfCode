f = open("input.txt","r")
fileData = f.read()
f.close

fileLines = fileData.split("\n")
fileLinesWithLineNumbers = list(enumerate(fileLines))

startInstruction = fileLinesWithLineNumbers[0]

def interpreter(node,accumulator,linesExecuted,instructionSet):
  line = node[0]
  code = node[1]

  instruction = code[:3]
  argument = int(code[4:])

  nextInstructionLine = line + 1

  if instruction == "acc":
    accumulator = accumulator + argument
  elif instruction == "jmp":
    nextInstructionLine = line + argument
  elif instruction == "nop":
    pass
  else:
    raise Exception("Invalid Instruction '{}''".format(instruction)) 

  if nextInstructionLine in linesExecuted:
    return (False, accumulator)
  else:
    if nextInstructionLine >= len(instructionSet):
      return (True, accumulator)
    else:
      newNode = instructionSet[nextInstructionLine]
      linesExecuted.append(line)
      return interpreter(newNode,accumulator,linesExecuted,instructionSet)



print(interpreter(startInstruction,0,[],fileLinesWithLineNumbers))

retOk = interpreter(startInstruction,0,[],fileLinesWithLineNumbers)[0]
possibleFaulty = list(filter(lambda x : "nop" in x[1] or "jmp" in x[1], fileLinesWithLineNumbers))

while (not retOk) and len(possibleFaulty) > 0: 
  faultyLine , command = possibleFaulty.pop()
  
  if "nop" in command:
    aux = fileLinesWithLineNumbers[faultyLine][1].replace("nop", "jmp")
    fileLinesWithLineNumbers[faultyLine] = (faultyLine, aux)
  elif "jmp" in command:
    aux = fileLinesWithLineNumbers[faultyLine][1].replace("jmp", "nop")
    fileLinesWithLineNumbers[faultyLine] = (faultyLine, aux)

  retOk, acc = interpreter(startInstruction,0,[],fileLinesWithLineNumbers)

  if not retOk:
    fileLinesWithLineNumbers[faultyLine] = (faultyLine, command)
  else:
    print((acc,faultyLine,command))
    break;