import string
import functools as func

f = open("input.txt","r")
fileData = f.read()
f.close()
groups = fileData.split("\n\n")
questions = string.ascii_lowercase;

part = 2
if part == 1:
  tot = 0
  for group in groups:
    group = group.replace("\n","")
    ans = map(lambda x: 1 if x in group else 0, questions)
    ans = list(ans)
    tot = tot + sum(ans)
  print(tot)
elif part == 2 :
  tot = 0
  for group in groups:
    peeps = group.split("\n")
    ans = map(lambda x : list(map( lambda y : 1 if y in x else 0, questions)) , peeps)
    ans = list(ans)
    if len(ans) > 1 :
      ans = func.reduce(lambda x,y : list(map(lambda z: z[0] and z[1] ,zip(x,y))) , ans)
    elif len(ans) == 1:
      ans = ans[0]
    tot = tot + sum(ans)

  print(tot)