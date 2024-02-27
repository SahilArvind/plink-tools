import os
from functools import cmp_to_key

def sort_genos(a, b):
    ch_a = a[1]
    ch_b = b[1]

    if ch_a == 'X':
        ch_a = 23
    elif ch_a == 'Y':
        ch_a = 24
    elif ch_a == 'XY':
        ch_a = 25
    elif ch_a == 'MT':
        ch_a = 26
    
    if ch_b == 'X':
        ch_b = 23
    elif ch_b == 'Y':
        ch_b = 24
    elif ch_b == 'XY':
        ch_b = 25
    elif ch_b == 'MT':
        ch_b = 26

    ch_a = int(ch_a)
    ch_b = int(ch_b)

    if ch_a < ch_b:
        return -1
    elif ch_a > ch_b:
        return 1

    loc_a = int(a[2])
    loc_b = int(b[2])

    if loc_a < loc_b:
        return -1
    elif loc_a > loc_b:
        return 1
    else:
        return 0

with open('./data.txt', 'r') as infile:
    l = infile.readlines()

l2 = []

for line in l:
    linestrp = line.strip()
    if not linestrp or '#' in linestrp or 'RSID' in linestrp:
        continue
    l2.append(linestrp)

l = l2

genos = []

for line in l:
    if '--' in line:
        pass

    genos.append(line.split())

genos.sort(key=cmp_to_key(sort_genos))

output_filename = input("Enter the output file name: ")

with open(output_filename, 'w') as outfile:
    for geno in genos:
        outfile.write('\t'.join([geno[0], geno[1], geno[2], geno[3]]) + '\n')

# Delete the data.txt file
os.remove('./data.txt')