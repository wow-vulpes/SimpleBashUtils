#!/bin/bash

S21CAT=output_s21_cat
CAT=output_cat
FORTEST=cat_tests/*

FLAGS="b e n s t v"
TESTS_COUNT=0
SUCCESS=0

for FLAG in $FLAGS
do
    ./s21_cat -$FLAG $FORTEST > $S21CAT
    cat -$FLAG $FORTEST > $CAT

    diff $S21CAT $CAT > /dev/null

    if [ $? -eq 0 ]
    then
        SUCCESS=$((SUCCESS + 1))
    else
        echo "\t-$FLAG $FORTEST\n"
    fi

    TESTS_COUNT=$((TESTS_COUNT + 1))
done

rm $S21CAT $CAT

echo "\nSUCCESS $SUCCESS/$TESTS_COUNT\n"

