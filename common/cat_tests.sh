#!/bin/bash

# без флагов
cat ./txt/*.txt > test_cat1.txt
../cat/s21_cat ./txt/*.txt > test_cat2.txt
diff test_cat1.txt test_cat2.txt
rm -rf test_cat1.txt test_cat2.txt

# -b --number-nonblank
cat -b ./txt/*.txt > test_cat1.txt
../cat/s21_cat -b ./txt/*.txt > test_cat2.txt
diff test_cat1.txt test_cat2.txt
rm -rf test_cat1.txt test_cat2.txt

# -e -E

cat -E ./txt/*.txt > test_cat1.txt
../cat/s21_cat -e ./txt/*.txt > test_cat2.txt
diff test_cat1.txt test_cat2.txt
rm -rf test_cat1.txt test_cat2.txt

cat -E ./txt/*.txt > test_cat1.txt
../cat/s21_cat -E ./txt/*.txt > test_cat2.txt
diff test_cat1.txt test_cat2.txt
rm -rf test_cat1.txt test_cat2.txt

# -n --number

cat -n ./txt/*.txt > test_cat1.txt
../cat/s21_cat -n ./txt/*.txt > test_cat2.txt
diff test_cat1.txt test_cat2.txt
rm -rf test_cat1.txt test_cat2.txt

cat --number ./txt/*.txt > test_cat1.txt
../cat/s21_cat --number ./txt/*.txt > test_cat2.txt
diff test_cat1.txt test_cat2.txt
rm -rf test_cat1.txt test_cat2.txt

# -s --squeeze-blank
cat -s ./txt/*.txt > test_cat1.txt
../cat/s21_cat -s ./txt/*.txt > test_cat2.txt
diff test_cat1.txt test_cat2.txt
rm -rf test_cat1.txt test_cat2.txt

cat --squeeze-blank ./txt/*.txt > test_cat1.txt
../cat/s21_cat --squeeze-blank ./txt/*.txt > test_cat2.txt
diff test_cat1.txt test_cat2.txt
rm -rf test_cat1.txt test_cat2.txt

# -t -T
cat -T ./txt/*.txt > test_cat1.txt
../cat/s21_cat -t ./txt/*.txt > test_cat2.txt
diff test_cat1.txt test_cat2.txt
rm -rf test_cat1.txt test_cat2.txt

cat -T ./txt/*.txt > test_cat1.txt
../cat/s21_cat -T ./txt/*.txt > test_cat2.txt
diff test_cat1.txt test_cat2.txt
rm -rf test_cat1.txt test_cat2.txt