#!/bin/bash

SUCCESS=0
FAIL=0
DIFF_RES=""
COUNTER=0
RED="$(tput setaf 1)"
GREEN="$(tput setaf 2)"
BASE="$(tput setaf 7)"

declare -a tests=(
    "for ../grep/grep.c ../grep/Makefile VAR"
    "s ./txt/grep1.txt VAR"
    "-e for -e ^int ../grep/grep.c ../grep/grep.h ../grep/Makefile VAR"
    "for ../grep/grep.c VAR"
    "-e while -e void ../grep/grep.c ../grep/Makefile VAR -f ./txt/pattern_grep.txt"
    "-e for -e ^int ../grep/grep.c VAR"
)

declare -a extra=(
    "-n -e ^\} ../grep/grep.c"
    "-n for ../grep/grep.c"
    "-n for ../grep/grep.c ../cat/cat.c"
    "-e ^int ../grep/grep.c"
    "-ce ^int ../grep/grep.c ../cat/cat.c"
    "-c -e \/ ../grep/grep.c"
    "-ie INT ../cat/infinity_cat_input.c"
    "-e"
    "-nivh = ../grep/grep.c ../cat/cat.c"
    "-iv int ../cat/infinity_cat_input.c"
    "-ne = -e out ../cat/infinity_cat_input.c"
    "-echar ../grep/grep.c ../cat/cat.c"
    "-v ../grep/grep.c -e ank"
    "-c -l aboba ../grep/grep.c ../cat/infinity_cat_input.c"
    "-in int ../cat/infinity_cat_input.c"
    "-e = -e out ../cat/infinity_cat_input.c"
    "-l for ../grep/grep.c ../cat/cat.c"
    "-l for no_file.txt ../cat/cat.c"
    "-noe ) ../cat/infinity_cat_input.c"
    "-e ing -e as -e the -e not -e is  ./txt/grep2.txt"
)

testing()
{
    t=$(echo $@ | sed "s/VAR/$var/")
    # echo "../grep/s21_grep $t "
    ../grep/s21_grep $t > test_s21_grep.log
    grep $t > test_sys_grep.log
    DIFF_RES="$(diff -s test_s21_grep.log test_sys_grep.log)"
    RESULT="SUCCESS"
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "Files test_s21_grep.log and test_sys_grep.log are identical" ]
    then
      (( SUCCESS++ ))
      echo "[${GREEN}${SUCCESS}${BASE}/${RED}${FAIL}${BASE}] ${GREEN}${RESULT}${BASE} grep $t"
    else
      (( FAIL++ ))
      RESULT="FAIL"
      echo "[${GREEN}${SUCCESS}${BASE}/${RED}${FAIL}${BASE}] ${RED}${RESULT}${BASE} grep $t"
    fi
    rm test_s21_grep.log test_sys_grep.log
}

# специфические тесты
for i in "${extra[@]}"
do
    var="-"
    testing $i
done

# 1 параметр
for var1 in v c l n h o
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        testing $i
    done
done

# 2 параметра
for var1 in v c l n h o
do
    for var2 in v c l n h o
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1 -$var2"
                testing $i
            done
        fi
    done
done

# 3 параметра
for var1 in v c l n h o
do
    for var2 in v c l n h o
    do
        for var3 in v c l n h o
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1 -$var2 -$var3"
                    testing $i
                done
            fi
        done
    done
done

# 2 сдвоенных параметра
for var1 in v c l n h o
do
    for var2 in v c l n h o
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1$var2"
                testing $i
            done
        fi
    done
done

# 3 строенных параметра
for var1 in v c l n h o
do
    for var2 in v c l n h o
    do
        for var3 in v c l n h o
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1$var2$var3"
                    testing $i
                done
            fi
        done
    done
done

echo "${RED}FAIL: ${FAIL}${BASE}"
echo "${GREEN}SUCCESS: ${SUCCESS}${BASE}"
echo "ALL: $COUNTER"
