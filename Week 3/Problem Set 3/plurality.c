/*
Program that takes in min 2, max 9 candidates names(through the command line, not scanf) and declares the winner based on highest_votes vote wins.

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


struct Candidate
{
    char name[50];
    int votes;
};


/*
merges
left = leftmost index
mid = middle index. i.e left + (right - left) / 2
right = rightmost index
*/
void merge(struct Candidate arr[], int left, int mid, int right)
{
    // finding size of left and right halves
    int left_size = mid - left + 1;
    int right_size = right - mid;
    // def struct with the size of left and right halves
    struct Candidate left_arr[left_size];
    struct Candidate right_arr[right_size];

    for(int i=0; i < left_size; i++)
    {
        left_arr[i] = arr[left + i];
    }
    for(int i=0; i < right_size; i++)
    {
        right_arr[i] = arr[mid + 1 + i];
    }

    int i = 0;
    int j = 0;
    int k = left;

    while(i < left_size && j < right_size)
    {
        if(strcmp(left_arr[i].name, right_arr[j].name) <= 0)
        {
            arr[k] = left_arr[i];
            i++;
        }
        else
        {
            arr[k] = right_arr[j];
            j++;
        }
        k++;
    }

    while(i < left_size)
    {
        arr[k] = left_arr[i];
        i++;
        k++;
    }
    while(j < right_size)
    {
        arr[k] = right_arr[j];
        j++;
        k++;
    }
}


/*
Sorts the struct that is passed into the function.
left = leftmost index (typically 0)
right = last index
*/
void merge_sort_struct(struct Candidate arr[], int left, int right)
{
    if (left >= right)  // >= makes sure that even if arr is empty, the case is handled
    {
        return;
    }
    int mid = left + (right - left) / 2;

    merge_sort_struct(arr, left, mid);
    merge_sort_struct(arr, mid+1, right);

    merge(arr, left, mid, right);
}


/*
count = count of items in the struct
target = search for {target} in struct.name
returns index of {target} in struct if found. -1 if not found
*/
int binary_search_struct(struct Candidate candidates[], int count, char target[])
{
    int left = 0;
    int right = count - 1;

    while(left <= right)
    {
        int mid = left + (right - left) / 2;
        int comparison = strcmp(target, candidates[mid].name);
        // in strcmp, if target comes before candidate: < 0
        // if target comes after candidate: > 0
        // if target is equal to candidate, then it returns 0
        if (comparison == 0)
        {
            return mid;
        }
        else if(comparison < 0)
        {
            right = mid - 1;
        }
        else
        {
            left = mid + 1;
        }
    }

    return -1; // -1 says search didnt find required thing
}


int main(int argc, char *argv[])
{
    // basic initial error cases
    if(argc < 3 || argc > 10)
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

    struct Candidate candidate[argc-1];

    // getting values into the struct/ initialize candidates
    for(int i=0; i < argc-1; i++)
    {
        strcpy(candidate[i].name, argv[i+1]);
        candidate[i].votes = 0;
    }

    // sort candidates alphabetically
    merge_sort_struct(candidate, 0, argc - 2);

    // collecting votes
    for(int i=0; i < no_of_voters; i++)
    {
        // input vote
        printf("Voter %d: ", i+1);
        char vote[50];
        scanf("%49s", vote);
        // adding vote
        int index = binary_search_struct(candidate, argc - 1, vote);
        if (index == -1)
        {
            printf("INVALID VOTE\n");
        }
        else
        {
            candidate[index].votes++;
        }
    }

    // winner declaration
    int winner_index[argc - 1];
    int highest_votes = 0;
    for (int i=0; i < argc - 1; i++)
    {
        if (highest_votes < candidate[i].votes)
        {
            highest_votes = candidate[i].votes;
        }
    }
    printf("Winner: ");
    int first_winner = 1;
    for(int i=0; i < argc - 1; i++)
    {
        if (highest_votes == candidate[i].votes)
        {
            if (!first_winner)
            {
                printf(", ");
            }
            printf("%s", candidate[i].name);
            first_winner = 0;
        }
    }

    return 0;
}
