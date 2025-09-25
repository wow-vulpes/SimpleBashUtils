#!/bin/bash

S21GREP=output_s21_grep
GREP=output_grep

FORTEST=grep_tests/*

FLAGS="i v c l n h s o"

PATTERNS="my is yeah"
PATTERNS_FOR_e="a water"
PATTERNS_FOR_f="grep_tests/grep_patterns.txt grep_tests/grep_a.txt"

TESTS_COUNT=0
SUCCESS=0

#без флагов
for PATTERN in $PATTERNS
do
    ./s21_grep $PATTERN $FORTEST > $S21GREP
    grep $PATTERN $FORTEST > $GREP

    diff $S21GREP $GREP > /dev/null

    if [ $? -eq 0 ]
    then
        SUCCESS=$((SUCCESS + 1))
    else
        echo "\tno flags $FORTEST\n"
    fi

    TESTS_COUNT=$((TESTS_COUNT + 1))
done

#флаги i v c l n h s o
for FLAG in $FLAGS
do
    for PATTERN in $PATTERNS
    do
        ./s21_grep $PATTERN -$FLAG $FORTEST > $S21GREP
        grep $PATTERN -$FLAG $FORTEST > $GREP

        diff $S21GREP $GREP > /dev/null

        if [ $? -eq 0 ]
        then
            SUCCESS=$((SUCCESS + 1))
        else
            echo "\t-$FLAG $FORTEST\n"
        fi

        TESTS_COUNT=$((TESTS_COUNT + 1))
    done

done

#флаги i v c l n h s o парами
for FLAG1 in $FLAGS
do
    for FLAG2 in $FLAGS
    do
        for PATTERN in $PATTERNS
        do

                ./s21_grep $PATTERN -$FLAG1 -$FLAG2 $FORTEST > $S21GREP
                grep $PATTERN -$FLAG1 -$FLAG2 $FORTEST > $GREP

                diff $S21GREP $GREP > /dev/null

                if [ $? -eq 0 ]
                then
                    SUCCESS=$((SUCCESS + 1))
                else
                    echo "\t-$FLAG1 -$FLAG2 $FORTEST\n"
                fi
            

            TESTS_COUNT=$((TESTS_COUNT + 1))
        done

    done
done

#флаг е 1 шаблон
    for PATTERN in $PATTERNS_FOR_e
    do
        ./s21_grep -e $PATTERN $FORTEST > $S21GREP
        grep -e $PATTERN $FORTEST > $GREP

        diff $S21GREP $GREP > /dev/null

        if [ $? -eq 0 ]
        then
            SUCCESS=$((SUCCESS + 1))
        else
            echo "\t-e $FORTEST\n"
        fi

        TESTS_COUNT=$((TESTS_COUNT + 1))
    done

#флаг f 1 шаблон
    for PATTERN in $PATTERNS_FOR_f
    do
        ./s21_grep -f $PATTERN $FORTEST > $S21GREP
        grep -f $PATTERN $FORTEST > $GREP

        diff $S21GREP $GREP > /dev/null

        if [ $? -eq 0 ]
        then
            SUCCESS=$((SUCCESS + 1))
        else
            echo "\t-f $FORTEST\n"
        fi

        TESTS_COUNT=$((TESTS_COUNT + 1))
    done

#флаг е несколько шаблонов
for PATTERN1 in $PATTERNS
do
    for PATTERN2 in $PATTERNS_FOR_e
    do
        ./s21_grep -e $PATTERN1 -e $PATTERN2 $FORTEST > $S21GREP
        grep -e $PATTERN1 -e $PATTERN2 $FORTEST > $GREP

        diff $S21GREP $GREP > /dev/null

        if [ $? -eq 0 ]
        then
            SUCCESS=$((SUCCESS + 1))
        else
            echo "\t-e $FORTEST\n"
        fi

        TESTS_COUNT=$((TESTS_COUNT + 1))
    done
done

#флаг f несколько шаблонов
for PATTERN1 in $PATTERNS_FOR_f
do
    for PATTERN2 in $PATTERNS_FOR_f
    do
        ./s21_grep -f $PATTERN1 -f $PATTERN2 $FORTEST > $S21GREP
        grep -f $PATTERN1 -f $PATTERN2 $FORTEST > $GREP

        diff $S21GREP $GREP > /dev/null

        if [ $? -eq 0 ]
        then
            SUCCESS=$((SUCCESS + 1))
        else
            echo "\t-f $FORTEST\n"
        fi

        TESTS_COUNT=$((TESTS_COUNT + 1))
    done
done

#флаги е и f
for PATTERN1 in $PATTERNS
do
    for PATTERN2 in $PATTERNS_FOR_f
    do
        ./s21_grep -e $PATTERN1 -f $PATTERN2 $FORTEST > $S21GREP
        grep -e $PATTERN1 -f $PATTERN2 $FORTEST > $GREP

        diff $S21GREP $GREP > /dev/null

        if [ $? -eq 0 ]
        then
            SUCCESS=$((SUCCESS + 1))
        else
            echo "\t-e -f $FORTEST\n"
        fi

        TESTS_COUNT=$((TESTS_COUNT + 1))
    done
done

#флаг е и др.
for FLAG in $FLAGS
do
    for PATTERN in $PATTERNS_FOR_e
    do

            ./s21_grep -$FLAG -e $PATTERN $FORTEST > $S21GREP
            grep -$FLAG -e $PATTERN $FORTEST > $GREP

            diff $S21GREP $GREP > /dev/null

            if [ $? -eq 0 ]
            then
                SUCCESS=$((SUCCESS + 1))
            else
                echo "\t-$FLAG -e $FORTEST\n"
            fi
            

            TESTS_COUNT=$((TESTS_COUNT + 1))
    done
done

#флаг f и др.
for FLAG in $FLAGS
do
    for PATTERN in $PATTERNS_FOR_f
    do

            ./s21_grep -$FLAG -f $PATTERN $FORTEST > $S21GREP
            grep -$FLAG -f $PATTERN $FORTEST > $GREP

            diff $S21GREP $GREP > /dev/null

            if [ $? -eq 0 ]
            then
                SUCCESS=$((SUCCESS + 1))
            else
                echo "\t -$FLAG -f $FORTEST\n"
            fi
            

            TESTS_COUNT=$((TESTS_COUNT + 1))
    done
done

rm $S21GREP $GREP

echo "\nSUCCESS $SUCCESS/$TESTS_COUNT\n"