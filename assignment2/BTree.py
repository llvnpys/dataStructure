import math
import sys

class BNode:
  def __init__(self, order):
    self.order = order
    self.child = []
    self.element = []

def insert(parent, node, key, value):
  pos = 0
  while pos < len(node.element) and node.element[pos][0] < key:
    pos += 1

  # non-leaf
  if len(node.child) != 0:
    node = insert(node, node.child[pos], key, value)
    if len(node.element) >= node.order:
      parent = split_node(parent, node)
      
  # leaf
  else:
    node.element.insert(pos, [key, value])
    if len(node.element) >= node.order:
      parent = split_node(parent, node)
      
  return parent

def split_node(parent, node):
  mid = (node.order-1) // 2
  right_child = BNode(node.order)

  # right node split
  for _ in range(mid+1, len(node.element)):
    right_child.element.append(node.element.pop(mid+1))

  if len(node.child) != 0:
    for _ in range(mid+1, len(node.child)):
      right_child.child.append(node.child.pop(mid+1))
    
  if parent != node:  
    parent.element.append(node.element.pop(mid))
    parent.child.append(node)
    parent.child.append(right_child)
    parent.element.sort()
    parent.child = sorted(set(parent.child), key=lambda x: x.element[0])
    return parent
  
  else:
    new_root = BNode(node.order)
    new_root.element.append(node.element.pop(mid))
    new_root.child.append(node)
    new_root.child.append(right_child)
    return new_root

def delete(parent, node, key):
  min_key = math.ceil(parent.order/2) - 1
  pos = 0
  while pos < len(node.element) and node.element[pos][0] < key:
    pos += 1
  
  if pos < len(node.element) and node.element[pos][0] == key:
    # leaf
    if len(node.child) == 0:
      node.element.pop(pos)
      idx = check_underflow(parent)
      if idx != -1:
        return merge(parent, idx)
      
    # internal
    else:
      # predecessor
      if len(node.child[pos].element) > min_key or len(node.child) != 0:
        left_child = node.child[pos]
        while len(left_child.child) != 0:
          left_child = left_child.child[-1]
        ele = left_child.element.pop()
        left_child.element.append(node.element.pop(pos))
        node.element.insert(pos, ele)      
        node = delete(node, node.child[pos], left_child.element[-1][0])
        idx = check_underflow(parent)
        if idx != -1:
          return merge(parent, idx)
        return parent
      
      # successor
      elif len(node.child[pos+1].element) > min_key:
        right_child = node.child[pos+1]
        while len(right_child.child) != 0:
          right_child = right_child.child[0]
        ele = right_child.element.pop()
        right_child.element.insert(0, node.element.pop(pos))
        node.element.insert(pos, ele)      
        node = delete(node, node.child[pos+1], right_child.element[0][0])
        idx = check_underflow(parent)
        if idx != -1:
          return merge(parent, idx)
        return parent
      
      # merge
      node.element.pop(pos)
      node.child[pos].element.extend(node.child[pos+1].element)
      node.child[pos].child.extend(node.child[pos+1].child)
      node.child.pop(pos+1)
      idx = check_underflow(parent)
      if idx != -1:
        return merge(parent, idx)
      
      return parent
    
  else:
    node = delete(node, node.child[pos], key)
    idx = check_underflow(parent)
    if idx != -1:
      return merge(parent, idx)
      
  return parent

# merge
# parent : 삭제가 이루어진 Node의 부모 / pos : parent.child[pos] : 삭제가 이루어진 노드, 그 위치
def merge(parent, pos):
  min_key = math.ceil(parent.order/2) - 1
  
# sibling node
  # predecessor
  if pos > 0 and len(parent.child[pos-1].element) > min_key:
    parent.child[pos].element.insert(0, parent.element.pop(pos-1))
    parent.element.insert(pos-1, parent.child[pos-1].element.pop())
    if len(parent.child[pos].child) != 0:
      parent.child[pos].child.insert(0, parent.child[pos-1].child.pop())
    return parent
    
  # successor
  if pos < len(parent.child) -1 and len(parent.child[pos+1].element) > min_key:
    parent.child[pos].element.append(parent.element.pop(pos))
    parent.element.insert(pos, parent.child[pos+1].element.pop(0))
    if len(parent.child[pos].child) != 0:
      parent.child[pos].child.append(parent.child[pos+1].child.pop(0))
    return parent
  
# parent or else
  if pos < len(parent.child)-1:
    parent.child[pos].element.append(parent.element.pop(pos))
    parent.child[pos].element.extend(parent.child[pos+1].element)
    parent.child[pos].child.extend(parent.child[pos+1].child)
    parent.child.pop(pos+1)
  else:
    parent.child[pos].element.insert(0, parent.element.pop(pos-1))
    parent.child[pos-1].element.extend(parent.child[pos].element)
    parent.child[pos-1].child.extend(parent.child[pos].child)
    parent.child.pop(pos)
  return parent

def check_underflow(parent):
  min_key = math.ceil(parent.order/2) - 1
  for pos in range(len(parent.child)):
    if len(parent.child[pos].element) < min_key:
      return pos
  else:
    return -1

def find(node, key):
  pos = 0
  while pos < len(node.element) and node.element[pos][0] < key:
    pos += 1
  if pos < len(node.element) and node.element[pos][0] == key:
    return node.element[pos]
  if len(node.child) == 0:
    return False
  
  return find(node.child[pos], key)

def search(t, fin, fout):
  for line in fin:
    key = int(line.split()[0])
    element = find(t, key)
    if element:
      fout.write(str(element[0])+ "\t" + str(element[1])+ "\n")
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
    order = input("order\n")
    tree = BNode(int(order))
    file1 = input("insert file name\n")
    insert_result = input("output file name\n")
    with open(file1, 'r') as fin:
      for line in fin:
        key = int(line.split()[0])
        value = int(line.split()[1])
        tree = insert(tree, tree, key, value)
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
        tree = delete(tree, tree, key)
    with open(file1, 'r') as fin, open(delete_result, 'w') as fout:
      search(tree, fin, fout)
    compare_filename = input("compare file name\n")
    with open(compare_filename, 'r') as fin, open(delete_result, 'r') as fout:
      compare(fin, fout)
  
  # quit
  elif(command == 3):
    sys.exit()