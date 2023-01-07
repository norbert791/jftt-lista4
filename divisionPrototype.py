def div(a : int, b : int):
  r = a
  b_ = b
  q = 0
  while r >= b_:
    tempB = b_
    tempQ = 1
    while r >= tempB:
      tempB = tempB + tempB
      tempQ = tempQ + tempQ
    tempB //= 2
    tempQ //= 2
    q += tempQ
    r = r - tempB
  return q, r

def main():
  print(div(10,2))

if __name__ == "__main__":
  main()