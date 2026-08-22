/*
Take input from two players. Calculate the points obtained by each player according to the character value chart given. Then declare the winner.

A=1, B=3, C=3, D=2, E=1, F=4, G=2, H=4, I=1, J=8, K=5, L=1, M=3, N=1, O=1, P=3, Q=10, R=1, S=1, T=1, U=1, V=4, W=4, X=8, Y=4, Z=10
*/

#include<stdio.h>
#include<string.h>
#include<ctype.h>


// array that contains points for each letter
const int LETTER_POINTS[26] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

// turns string to lower case using pointer. Doesnt return
void turn_lower_case(char *str)
{
    for(int i = 0; str[i] != '\0'; i++)
    {
        str[i] = tolower((unsigned char)str[i]);
    }
}

// calculates the total score of the input "string", tallying with LETTER_POINTS global var
int calc_score(char *str)
{
    int total_score = 0;
    int pos_of_letter = 0;

    for(int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] >= 'a' && str[i] <= 'z')
        {
            pos_of_letter = str[i] - 'a';
            total_score += LETTER_POINTS[pos_of_letter];
        }
    }

    return total_score;
}


int main(void)
{
    // getting the strings
    char p1_input[50];
    char p2_input[50];
    printf("Player 1: ");
    scanf("%49s", p1_input);
    printf("Player 2: ");
    scanf("%49s", p2_input);

    // turning to lowercase
    turn_lower_case(p1_input);
    turn_lower_case(p2_input);

    // getting scores
    int p1_score = calc_score(p1_input);
    int p2_score = calc_score(p2_input);

    // final output
    if(p1_score > p2_score)
    {
        printf("The Winner is Player 1!");
    }
    else if(p2_score > p1_score)
    {
        printf("The Winner is Player 2!");
    }
    else
    {
        printf("It's a Tie!");
    }
}
