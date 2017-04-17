import random
import csv
import math

def main():
  dimension_count = 2
  selection_size = 200
  selection = [] # selection of points
  random.seed(1) # first part
  for i in range(0, int(selection_size)):
    x = random.random()
    y = random.randint(-100, 100)
    selection.append([x, y * abs(math.cos(x))])

  write_data(selection, 'fcmdata.dat')

def write_data(data, filename='output.csv'):
  with open(filename, 'w', newline='') as csvfile:
    csvwriter = csv.writer(csvfile, delimiter=' ')
    csvwriter.writerows(data)

main()