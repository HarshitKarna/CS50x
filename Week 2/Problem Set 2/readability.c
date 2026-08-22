/*
implement a program that calculates the approximate grade level needed to comprehend some text. Your program should print as output “Grade X” where “X” is the grade level computed, rounded to the nearest integer. If the grade level is 16 or higher (equivalent to or greater than a senior undergraduate reading level), your program should output “Grade 16+” instead of giving the exact index number. If the grade level is less than 1, your program should output “Before Grade 1”.

The Coleman-Liau index of a text is designed to output that (U.S.) grade level that is needed to understand some text. The formula is:

index = 0.0588 * L - 0.296 * S - 15.8

where L is the average number of letters per 100 words in the text, and S is the average number of sentences per 100 words in the text.

NOTE: it is also assumed that there aren't any grammatical errors in the program
*/

#include<ctype.h>
#include<math.h>
#include<stdio.h>
#include "../cs50.c"
#include "../cs50.h"

struct text_details
{
    int letter_count;
    int sentence_count;
    int word_count;
};

// turns string to lower case using pointer. Doesnt return
void turn_lower_case(char *str)
{
    for(int i = 0; str[i] != '\0'; i++)
    {
        str[i] = tolower((unsigned char)str[i]);
    }
}

// gets number of letters/actual characters
struct text_details str_details(char *str)
{
    struct text_details r;
    r.letter_count = 0;
    r.sentence_count = 0;
    r.word_count = 1;

    for(int i = 0; str[i] != '\0'; i++)
    {
        if(str[i] >= 'a' && str[i] <= 'z')
        {
            r.letter_count++;
        }
        else if(str[i] == ' ')
        {
            r.word_count++;
        }
        else if(str[i] == '.' || str[i] == '?' || str[i] == '!')
        {
            r.sentence_count++;
        }
    }

    return r;
}


int main(void)
{
    printf("Text: ");
    string input_text = GetString();

    turn_lower_case(input_text);

    struct text_details vals;
    vals = str_details(input_text);

    // index calculation
    double index = (((double) vals.letter_count / vals.word_count) * 100 * 0.0588) - (((double) vals.sentence_count / vals.word_count) * 100 * 0.296) - 15.8;
    printf("Index: %f\n", index);

    // grade check
    int index_rounded = (int)round(index);
    if(index_rounded >= 16)
    {
        printf("Grade 16+");
    }
    else if(index_rounded < 1)
    {
        printf("Before Grade 1");
    }
    else
    {
        printf("Grade %d", index_rounded);
    }
}
