def trees_for_slope(treeMat,slope):
  position = (0,0)
  tree_count = 0

  while (position[0] + slope[0]) < mat_h:
    # go to next position and test
    new_position = (position[0] + slope[0], (position[1] + slope[1]) % mat_w)
    #print(new_position)
    if(treeMat[new_position[0]][new_position[1]] == 1):
      tree_count = tree_count + 1
    position = new_position

  return tree_count







file1 = open('input.txt', 'r') 
Lines = file1.readlines() 

mat_h = len(Lines)
mat_w = len(Lines[0].strip())
treeMat = [ [ 0 for y in range( mat_w ) ] for x in range( mat_h ) ]
slope = (1,3)

for (indx, line) in enumerate(Lines): 
  line = line.strip()
  aux = -1
  aux = line.find("#")
  while aux > -1:
    treeMat[indx][aux] = 1
    aux = line.find("#", aux+1)



print(trees_for_slope(treeMat,slope) * 
      trees_for_slope(treeMat,(1,1)) *
      trees_for_slope(treeMat,(1,5)) *
      trees_for_slope(treeMat,(1,7)) *
      trees_for_slope(treeMat,(2,1)))