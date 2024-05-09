#!/bin/bash

# без флагов
grep q ./txt/*.txt > test_grep1.txt
../grep/s21_grep q ./txt/*.txt > test_grep2.txt
diff test_grep1.txt test_grep2.txt
rm -rf test_grep1.txt test_grep2.txt

# -e
grep -e q ./txt/*.txt > test_grep1.txt
../grep/s21_grep -e q ./txt/*.txt > test_grep2.txt
diff test_grep1.txt test_grep2.txt
rm -rf test_grep1.txt test_grep2.txt

# -i

grep -i q ./txt/*.txt > test_grep1.txt
../grep/s21_grep -i q ./txt/*.txt > test_grep2.txt
diff test_grep1.txt test_grep2.txt
rm -rf test_grep1.txt test_grep2.txt

# -v

grep -v q ./txt/*.txt > test_grep1.txt
../grep/s21_grep -v q ./txt/*.txt > test_grep2.txt
diff test_grep1.txt test_grep2.txt
rm -rf test_grep1.txt test_grep2.txt

# -c
grep -c q ./txt/*.txt > test_grep1.txt
../grep/s21_grep -c q ./txt/*.txt > test_grep2.txt
diff test_grep1.txt test_grep2.txt
rm -rf test_grep1.txt test_grep2.txt

# -l
grep -l q ./txt/*.txt > test_grep1.txt
../grep/s21_grep -l q ./txt/*.txt > test_grep2.txt
diff test_grep1.txt test_grep2.txt
rm -rf test_grep1.txt test_grep2.txt

# -n
grep -n q ./txt/*.txt > test_grep1.txt
../grep/s21_grep -n q ./txt/*.txt > test_grep2.txt
diff test_grep1.txt test_grep2.txt
rm -rf test_grep1.txt test_grep2.txt

# -h
grep -h q ./txt/*.txt > test_grep1.txt
../grep/s21_grep -h q ./txt/*.txt > test_grep2.txt
diff test_grep1.txt test_grep2.txt
rm -rf test_grep1.txt test_grep2.txt

# -s
grep -s q ./txt/*.txt > test_grep1.txt
../grep/s21_grep -s q ./txt/*.txt > test_grep2.txt
diff test_grep1.txt test_grep2.txt
rm -rf test_grep1.txt test_grep2.txt

# -f
echo q > patern.txt
set timeout 1s
grep -f patern.txt ./txt/*.txt > test_grep1.txt
../grep/s21_grep -f patern.txt ./txt/*.txt > test_grep2.txt
diff test_grep1.txt test_grep2.txt
rm -rf test_grep1.txt test_grep2.txt patern.txt

# -o
grep -o q ./txt/*.txt > test_grep1.txt
../grep/s21_grep -o q ./txt/*.txt > test_grep2.txt
diff test_grep1.txt test_grep2.txt
rm -rf test_grep1.txt test_grep2.txt