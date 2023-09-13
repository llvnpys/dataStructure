import sys

# node class
class AVLNode:
  def __init__(self, key, value):
    self.key = key
    self.value = value
    self.left = None
    self.right = None
    self.height = 0

# single rotate(Left)
def single_rotate_with_left(node):
  temp = node.right
  node.right = temp.left
  temp.left = node
  node.height = set_height(node)
  temp.height = set_height(temp)
  return temp

# single rotate(Right)
def single_rotate_with_right(node):
  temp = node.left
  node.left = temp.right
  temp.right = node
  node.height = set_height(node)
  temp.height = set_height(temp)
  return temp

# double rotate(Left-Rgiht)
def double_rotate_with_left(node):
  node.right = single_rotate_with_right(node.right)
  return single_rotate_with_left(node)

# double rotate(Right-Left)
def double_rotate_with_right(node):  
  node.left = single_rotate_with_left(node.left)
  return single_rotate_with_right(node)

def get_balance_factor(node):
  if node.left and node.right :
    return node.left.height - node.right.height
  elif node.left:
    return node.left.height + 1
  elif node.right:
    return -(node.right.height + 1)
  else:
    return 0

def select_rotation(node):
  balance_factor = get_balance_factor(node)
  # left
  if balance_factor < -1:
    balance_factor = get_balance_factor(node.right)
    if balance_factor > 0:
      node = double_rotate_with_left(node)
    else:
      node = single_rotate_with_left(node)
      
  # right
  elif balance_factor > 1:
    balance_factor = get_balance_factor(node.left)
    if balance_factor < 0:
      node = double_rotate_with_right(node)
    else:
      node = single_rotate_with_right(node)
  return node

def set_height(node):
  if node.left and node.right:
    return max(node.left.height, node.right.height) + 1
  elif node.left:
    return node.left.height + 1
  elif node.right:
    return node.right.height + 1
  else:
    return 0

def insert(key, value, t):
  if t is None:
    t = AVLNode(key, value)
  elif t.key > key:
    t.left = insert(key, value , t.left)
  elif t.key < key:
    t.right = insert(key, value, t.right)
    
  t.height = set_height(t)
  t = select_rotation(t)
  return t

def delete(key, t):
  if t == None:
    return t
  elif t.key > key:
    t.left = delete(key, t.left)
  elif t.key < key:
    t.right = delete(key, t.right)
  else:
    temp = None
    if t.left is None:
      temp = t.right
      return temp
    elif t.right is None:
      temp = t.left
      return temp
    else:
      temp = t.right
      while temp.left is not None:
          temp = temp.left
      t.key = temp.key
      t.value = temp.value
      t.right = delete(temp.key, t.right)
  
  t.height = set_height(t)
  t = select_rotation(t)
  return t

def find(key, t):
  if t == None:
    return None
  elif t.key == key:
    return t
  elif t.key > key:
    return find(key, t.left)
  else:
    return find(key, t.right)

# def print_inorder(t, output2):
#   if t is None:
#     return
#   print_inorder(t.left, output2)
#   output2.write(str(t.key)+ "\t" + str(t.value)+ "\n")
#   print_inorder(t.right, output2)

def search(t, fin, fout):
  for line in fin:
    key = int(line.split()[0])
    node = find(key, t)
    if node:
      fout.write(str(node.key)+ "\t" + str(node.value)+ "\n")
    else:
      fout.write(str(key)+ "\tN/A\n")
    
def compare(fin, fout):
  for line1, line2 in zip(fin, fout):
    if line1.split()[0] != line2.split()[0] or line1.split()[1] != line2.split()[1]:
      print("Comparison failed.")
      return
    print("Comparison succeeded.")
    return
  
# main
# python ***.py
tree = None
while(True):
  command = int(input("1.Insertion, 2.Deletion, 3.Quit\n"))
  
  # insertion
  if(command == 1):
    file1 = input("insert file name\n")
    insert_result = input("output file name\n")
    with open(file1, 'r') as fin:
      for line in fin:
        key = int(line.split()[0])
        value = int(line.split()[1])
        tree = insert(key, value, tree)
    with open(file1, 'r') as fin, open(insert_result, 'w') as fout:
      search(tree, fin, fout)
    with open(file1, 'r') as fin, open(insert_result, 'r') as fout:
      compare(fin, fout)
  
  # deletion
  elif(command == 2):
    file2 = input("delete file name\n")
    delete_result = input("output file name\n")
    with open(file2, 'r') as fin:
      for line in fin:
        key = int(line.split()[0])
        tree = delete(key, tree)
    with open(file1, 'r') as fin, open(delete_result, 'w') as fout:
      search(tree, fin, fout)
    compare_filename = input("compare file name\n")
    with open(compare_filename, 'r') as fin, open(delete_result, 'r') as fout:
      compare(fin, fout)
  
  # quit
  elif(command == 3):
    sys.exit()