#!/bin/bash

sum_arr()
{
    ARR=("$@");
    SUM=0
    for NUM in "${ARR[@]}"; do
        SUM=$(($SUM + $NUM));
    done

};

parse_num()
{
    PARSED_VAL=0;
    case "${1,,}" in
        "one"|"1") PARSED_VAL=1;;
        "two"|"2") PARSED_VAL=2;;
        "three"|"3") PARSED_VAL=3;;
        "four"|"4") PARSED_VAL=4;;
        "five"|"5") PARSED_VAL=5;;
        "six"|"6") PARSED_VAL=6;;
        "seven"|"7") PARSED_VAL=7;;
        "eight"|"8") PARSED_VAL=8;;
        "nine"|"9") PARSED_VAL=9;;
    esac
};

# INPUT_FILE="./tst_input.txt";
INPUT_FILE="./input.txt"

NUMS1=();
NUMS2=();

FD_PATTERN="(one|two|three|four|five|six|seven|eight|nine|[0-9]).*$"
LD_PATTERN="^.*(one|two|three|four|five|six|seven|eight|nine|[0-9])"

###########################################################################
#                                                                         #
#       DOCUMENT MUST END WITH A NEWLINE FOR READ TO WORK CORRECTLY       #
#                ^^^^            ^^^^^^^                                  #
###########################################################################

while read LINE; do
    # echo "Read: $LINE"

    if [[ $LINE =~ [0-9] ]]; then
        FD1=${BASH_REMATCH[0]};
        # echo "  [1]Firts digit: $FD1";
    fi
    if [[ $LINE =~ .*([0-9]) ]]; then
        LD1=${BASH_REMATCH[1]};
        # echo "  [1]Last digit: $LD1";
    fi
    
    if [[ ${LINE,,} =~ $FD_PATTERN ]]; then
        FD2_MATCH=${BASH_REMATCH[1]};
        parse_num "$FD2_MATCH"
        FD2=$PARSED_VAL
        # echo "  [2]Firts digit: $FD2_MATCH - $FD2";
    fi
    if [[ ${LINE,,} =~ $LD_PATTERN ]]; then
        LD2_MATCH=${BASH_REMATCH[1]};
        parse_num "$LD2_MATCH"
        LD2=$PARSED_VAL
        # echo "  [2]Last digit: $LD2_MATCH - $LD2";
    fi

    NUM1=$(($FD1*10+$LD1));
    # echo "  [1]Number: $NUM1";
    NUMS1+=($NUM1);

    NUM2=$(($FD2*10+$LD2));
    # echo "  [2]Number: $NUM2";
    NUMS2+=($NUM2);
done < $INPUT_FILE

sum_arr "${NUMS1[@]}"
SUM1=$SUM;

sum_arr "${NUMS2[@]}"
SUM2=$SUM;

echo "Part 1: $SUM1"
echo "Part 2: $SUM2"
