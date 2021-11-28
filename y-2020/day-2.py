  
def validPassWord(policy, password):
  policy = policy.split(" ",1)
  p_range = policy[0].strip().split("-", 1)
  low_range = int(p_range[0].strip())
  high_range = int(p_range[1].strip())
  p_letter = policy[1].strip()

  actual_number = password.count(p_letter)

  if (actual_number >= low_range and actual_number <= high_range):
    return True
  else:
    return False

def validPassWord2(policy, password):
  policy = policy.split(" ",1)
  p_range = policy[0].strip().split("-", 1)
  low_range = int(p_range[0].strip())
  high_range = int(p_range[1].strip())
  p_letter = policy[1].strip()

  if ((password[low_range-1] == p_letter and not password[high_range-1] == p_letter)or (not password[low_range-1] == p_letter and password[high_range-1] == p_letter)):
    return True
  else:
    return False


# Using readlines() 
file1 = open('input.txt', 'r') 
Lines = file1.readlines() 
  
count = 0;
# Strips the newline character 
for line in Lines: 
  line = line.strip()
  line = line.split(":",1)
  policy = line[0].strip()
  passWord = line[1].strip()
  #print("Pol:{} ; Pass:{}".format(policy,passWord))
  if(validPassWord2(policy,passWord)):
    count = count+1

print(count)