/*
Program that takes in min 2, max 9 candidates names(through the command line, not scanf) and declares the winner based on highest vote wins.

EG:> program.exe Bob Alice Michael
No. of voters: 3
Voter 1: Bob
Voter 2: Bob
Voter 3: RandoGUY
INVALID VOTE
Winner: Bob
(if tie, show both)
*/

#include<stdio.h>
#include<string.h>


typedef struct Candidates
{
    char name[50];
    int votes;
};


// void merge_sort_struct(struct Candidates arr[], int left, int right)
// {
//     int mid = left + (right - left) / 2;
// }


int main(int argc, char *argv[])
{
    // basic initial error cases
    if(argc <= 3 || argc > 10)
    {
        printf("Usage: plurality {atleast 2 candidates, max 9}");
        return 1;
    }
    printf("No. of Voters: ");
    int no_of_voters = 0;
    scanf("%d", &no_of_voters);
    if(no_of_voters < 1)
    {
        printf("There cant be that many voters");
    }

    struct Candidates candidate[argc-1];

    // getting values into the struct
    for(int i=0; i < argc-1; i++)
    {
        strcpy(candidate[i].name, argv[i+1]);
        candidate[i].votes = 0;
    }

    for(int i=0; i < argc-1; i++)
    {
        printf("Voter %d", i+1);
        char vote[50];
        scanf("%c", vote[50]);

        // going through each candidate
    }
}
