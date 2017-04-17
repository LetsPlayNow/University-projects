#!python
# open file and parse it
# 1) [| 0 | 1 | 2 |] => [1/2] #=> find currency ratio
# 2) i | i-1 | i-2 | i+1 
import csv
import sys

def main():
  ratio_list = csv_to_ratio_list(sys.argv[1])
  fuzzy_network_data = ratio_list_to_fuzzy_network_data(ratio_list)
  fuzzy_network_data_to_file(sys.argv[2], fuzzy_network_data)


def csv_to_ratio_list(filename):
  ratio_list = []
  with open(filename, 'r') as csvfile:
    csvreader = csv.reader(csvfile, delimiter=';')
    next(csvreader) # ignore the header
    for row in csvreader:
      ratio_value = float(row[1].replace(',', '.')) / float(row[2].replace(',', '.'))
      ratio_list.append(ratio_value)
  return ratio_list[::-1]


def ratio_list_to_fuzzy_network_data(ratio_list):
  fuzzy_network_data = []
  for i in range(2, len(ratio_list)-1):
    fuzzy_network_data.append([ratio_list[i], 
                               ratio_list[i-1],
                               ratio_list[i-2], 
                               ratio_list[i+1]])
  return fuzzy_network_data

def fuzzy_network_data_to_file(filename, fuzzy_network_data):
  with open(filename, 'w', newline='') as csvfile:
    csvwriter = csv.writer(csvfile, delimiter=';')
    csvwriter.writerows(fuzzy_network_data)

main()