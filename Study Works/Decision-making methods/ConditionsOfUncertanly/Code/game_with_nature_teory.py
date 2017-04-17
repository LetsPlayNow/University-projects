#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Script created by Titov Alex [IVT-360]
# Game with nature teory

import csv


def main():
  gurvitz_alpha    = input("Введите коэффициент для критерия Гурвица [0,1]: ")
  hojj_leman_alpha = input("Введите коэффициент для критерия Ходжа Лемана [0;1]: ")

  input_data = matrix_to_float(read_data())
  print(input_data)
  probability_vector = input_data[0]
  payoff_matrix      = input_data[1:]

  
  # Calculations
  vald         = Vald_criterium(payoff_matrix)
  savidge      = Savidge_criterium(payoff_matrix)
  laplas_baies = Laplas_Baies_criterium(probability_vector, payoff_matrix)
  ghurvitz     = Ghurvitz_criterium(gurvitz_alpha, payoff_matrix)
  ghermeyer    = Ghermeyer_criterium(probability_vector, payoff_matrix)
  hojj_leman   = Hojj_Leman_criterium(hojj_leman_alpha, probability_vector, payoff_matrix)

  # Output
  with open('output.csv', 'w') as csvfile:
    csvwriter = csv.writer(csvfile, delimiter=';')
    # Write header
    csvwriter.writerow([probability_vector, 'Vald', 'Savidge', 'Laplas-Baies', 'Ghurvitz', 'Ghermeyer', 'Hojj-Leman'])

    # Write data
    for i in range(0, len(payoff_matrix)):
      csvwriter.writerow([payoff_matrix[i], vald[i], savidge[i], laplas_baies[i], ghurvitz[i], ghermeyer[i], hojj_leman[i]])


# read input from csv
def read_data():
  data = []
  with open('input.csv', 'r') as csvfile:
    csvreader = csv.reader(csvfile, delimiter=';')
    for row in csvreader:
      data.append(row)

  return data


# Variuos criteries of "game with nature" teory
# Max(Mins)
def Vald_criterium(payoff_matrix):
  criterium_row = []
  for row in payoff_matrix:
    criterium_row.append(min(row))

  value_of_best = max(criterium_row)
  index_of_best = criterium_row.index(value_of_best)
  return highlight_best_value_row(criterium_row, index_of_best, value_of_best)

# Min(Max - Min)
def Savidge_criterium(payoff_matrix):
  criterium_row = []
  for row in payoff_matrix:
    criterium_row.append(max(row) - min(row))

  value_of_best = min(criterium_row)
  index_of_best = criterium_row.index(value_of_best)
  return highlight_best_value_row(criterium_row, index_of_best, value_of_best)

# Max(summ(a_i * q_i))
def Laplas_Baies_criterium(probability_vector, payoff_matrix):
  criterium_row = []
  for row in payoff_matrix:
    criterium_for_row = 0.0
    for i in range(0, len(row)):
      criterium_for_row += row[i] * probability_vector[i]  

    criterium_row.append(criterium_for_row)

  value_of_best = max(criterium_row)
  index_of_best = criterium_row.index(value_of_best)
  return highlight_best_value_row(criterium_row, index_of_best, value_of_best)

# Max(alpha * min(row) + (1-alpha) * max(row))
def Ghurvitz_criterium(alpha, payoff_matrix):
  criterium_row = []
  for row in payoff_matrix:
    criterium_row.append(alpha * min(row) + (1-alpha) * max(row))

  value_of_best = max(criterium_row)
  index_of_best = criterium_row.index(value_of_best)
  return highlight_best_value_row(criterium_row, index_of_best, value_of_best)

# Min(Max(a_i*q_i of each in row))
def Ghermeyer_criterium(probability_vector, payoff_matrix):
  criterium_row = []
  for row in payoff_matrix:
    inefficiency_row = []
    for i in range(0, len(row)):
      inefficiency_row.append(row[i] * probability_vector[i])

    criterium_for_row = max(inefficiency_row)
    criterium_row.append(criterium_for_row)

  value_of_best = min(criterium_row)
  index_of_best = criterium_row.index(value_of_best)
  return highlight_best_value_row(criterium_row, index_of_best, value_of_best)

# Max(alpha * (a_i * q_i) + (1-alpha) * min(row))
def Hojj_Leman_criterium(alpha, probability_vector, payoff_matrix):
  criterium_row = []
  for row in payoff_matrix:
    criterium_for_row = 0.0
    for i in range(0, len(row)):
      criterium_for_row += (alpha * (row[i] * probability_vector[i]) +
                           (1-alpha) * min(row))

    criterium_row.append(criterium_for_row)

  value_of_best = max(criterium_row)
  index_of_best = criterium_row.index(value_of_best)
  return highlight_best_value_row(criterium_row, index_of_best, value_of_best)


# Helpers 
# highlight best value in criterium row
def highlight_best_value_row(criterium_row, index_of_best, value_of_best):
  criterium_row[index_of_best] = "| {} |".format(str(round(value_of_best, 2)))
  return criterium_row

# Convert matrix os strings to matrix of floats
def matrix_to_float(matrix):
  matrix_height = len(matrix)
  matrix_width  = len(matrix[0])
  for row in range(0, matrix_height):
    for col in range(0, matrix_width):
      matrix[row][col] = float(matrix[row][col])

  return matrix



# ===============================================================
# Execution
# ===============================================================
main()


