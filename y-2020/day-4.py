import functools 


def tryParseInt(strn):
  try:
    val = int(strn)
    return (True, val)
  except Exception:
    return (False, 0)


f = open("input.txt", "r")
fileData = f.read().strip()
f.close()


# byr (Birth Year)
# iyr (Issue Year)
# eyr (Expiration Year)
# hgt (Height)
# hcl (Hair Color)
# ecl (Eye Color)
# pid (Passport ID)
# cid (Country ID)
reqFields = ["byr","iyr","eyr","hgt","hcl","ecl","pid"]

problem = 2
if problem == 1 :
  
  count = 0
  passportsData = fileData.split("\n\n")
  for passportData in passportsData:

    res = functools.reduce(lambda x,y : x and y, list(map(lambda x : x in passportData , reqFields)))
    if(res):
      count = count + 1
    
  print("Sol1:{}".format(count))

elif problem == 2:

  count = 0
  passportsData = fileData.split("\n\n")
  for passportData in passportsData:
    passportData = passportData.replace("\n", " ");

    fieldOk = True;

    for fieldName in reqFields:
      fieldStart = passportData.find(fieldName)
      if not fieldStart == -1:
        fieldEnd = passportData[fieldStart:].find(" ")
        if fieldEnd == -1:
          fieldEnd = len(passportData)
        else:
          fieldEnd = fieldStart + fieldEnd
        
        #print(passportData[fieldStart:fieldEnd])
        fullField = passportData[fieldStart:fieldEnd]
        fieldData = passportData[(fullField.find(":") + 1 + fieldStart):fieldEnd]
        if(fieldName == "byr"):
          year = tryParseInt(fieldData)
          year = year[1] if year[0] else -1
          fieldOk = fieldOk and year >= 1920 and year <= 2002 and str(year) == fieldData
        elif(fieldName == "iyr"):
          year = tryParseInt(fieldData)
          year = year[1] if year[0] else -1
          fieldOk = fieldOk and year >= 2010 and year <= 2020 and str(year) == fieldData
        elif(fieldName == "eyr"):
          year = tryParseInt(fieldData)
          year = year[1] if year[0] else -1
          fieldOk = fieldOk and year >= 2020 and year <= 2030 and str(year) == fieldData
        elif(fieldName == "hgt"):
          cmStart = fieldData.find("cm")
          inStart = fieldData.find("in")
          unitStart = cmStart if cmStart > -1 and inStart == -1 else (inStart if inStart > -1 and cmStart == -1 else -1)
          # print("({})-({})-({})".format(cmStart, inStart, unitStart))
          if unitStart > -1:
            number = fieldData[:unitStart]
            number = tryParseInt(number)
            number = number[1] if number[0] else -1
            number = number if str(number) == fieldData[:unitStart] else -1
            if cmStart > -1:
              fieldOk = fieldOk and 150 <= number and number <= 193
            elif inStart > -1:
              fieldOk = fieldOk and 59 <= number and number <= 76
            else:
              fieldOk = False
          else:
            fieldOk = False

        elif(fieldName == "hcl"):
          res = functools.reduce(lambda x,y : x and y, list(map(lambda x : ((x >= "a" and x <= "f") or (x >= "0" and x <= "9")) , fieldData[1:])),True)
          res = res and (len(fieldData) == 7)
          res = res and (fieldData[0] == "#")
          fieldOk = fieldOk and res
        elif(fieldName == "ecl"):
          res = fieldData in ["amb","blu","brn","gry","grn","hzl","oth"]
          fieldOk = fieldOk and res
        elif(fieldName == "pid"):
          num = tryParseInt(fieldData)
          res = num[0] and len(fieldData) == 9
          fieldOk = fieldOk and res

        print("--{}-{} - {}".format(fieldName,fieldData,fieldOk))
      else:
        fieldOk = False
    
    print("{}\n----{}".format(passportData,fieldOk))
    print()

    if(fieldOk):
      count = count + 1

  print("Sol2:{}".format(count))
