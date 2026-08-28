/*
I HATE C
Program to do a runoff voting where ranks is taken from the voters.

In an instant runoff election, voters can rank as many candidates as they wish. If any candidate has a majority (more than 50%) of the first preference votes, that candidate is declared the winner of the election.

If no candidate has more than 50% of the vote, then an “instant runoff” occurrs. The candidate who received the fewest number of votes is eliminated from the election, and anyone who originally chose that candidate as their first preference now has their second preference considered. Why do it this way? Effectively, this simulates what would have happened if the least popular candidate had not been in the election to begin with.

The process repeats: if no candidate has a majority of the votes, the last place candidate is eliminated, and anyone who voted for them will instead vote for their next preference (who hasn’t themselves already been eliminated). Once a candidate has a majority, that candidate is declared the winner.

(do Voter 1:
Rank 1:
Rank 2:
...
Rank n:

Voter 2:
Rank 1:
....)
*/

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#define MAX_CANDIDATES 9

struct Voter
{
    int ranking[MAX_CANDIDATES];
};

struct Candidate
{
    char name[50];
    int votes;
    bool eliminated;
};


void merge(struct Candidate arr[], int left, int mid, int right)
{
    int left_size = mid - left + 1;
    int right_size = right - mid;

    struct Candidate left_arr[left_size];
    struct Candidate right_arr[right_size];

    for (int i = 0; i < left_size; i++)
    {
        left_arr[i] = arr[left + i];
    }
    for (int i = 0; i < right_size; i++)
    {
        right_arr[i] = arr[mid + 1 + i];
    }

    int i = 0;
    int j = 0;
    int k = left;

    while (i < left_size && j < right_size)
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

    while (i < left_size)
    {
        arr[k] = left_arr[i];
        i++;
        k++;
    }
    while (j < right_size)
    {
        arr[k] = right_arr[j];
        j++;
        k++;
    }
    
}


void merge_sort_struct(struct Candidate arr[], int left, int right)
{
    if (left >= right)
    {
        return;
    }

    int mid = left + (right - left) / 2;
    merge_sort_struct(arr, left, mid);
    merge_sort_struct(arr, mid + 1, right);

    merge(arr, left, mid, right);
}


int binary_search_struct(struct Candidate arr[], int count, char target[])
{
    int left = 0;
    int right = count - 1;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        int comparison = strcmp(target, arr[mid].name);

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

    return -1;
}


bool already_ranked(struct Voter *voter, int rank, int candidate)
{
    for (int i = 0; i < rank; i++)
    {
        if (voter->ranking[i] == candidate)
        {
            return true;
        }
    }

    return false;
}


/*
Returns index of the majority winner
Returns -1 if no >50% majority was found
*/
int has_majority(struct Candidate arr[], int candidates_count, int voters_count)
{
    for (int i = 0; i < candidates_count; i++)
    {
        if (!arr[i].eliminated && arr[i].votes > (voters_count / 2))
        {
            return i;
        }
    }
    return -1;
}


int highest_votes(struct Candidate arr[], int count)
{
    int highest = 0;
    for (int i = 0; i < count; i++)
    {
        if (!arr[i].eliminated && arr[i].votes > highest)
        {
            highest = arr[i].votes;
        }
    }

    return highest;
}


int lowest_votes(struct Candidate arr[], int count)
{
    int lowest = INT_MAX;
    for (int i=0; i < count; i++)
    {
        if (!arr[i].eliminated && arr[i].votes < lowest)
        {
            lowest = arr[i].votes;
        }
    }

    return lowest;
}


bool check_majority(struct Candidate arr[], int candidate_count, int voter_count)
{
    int majority_index = has_majority(arr, candidate_count, voter_count);
    if (majority_index != -1)
    {
        printf("Winner: %s", arr[majority_index].name);
        return true;
    }

    return false;

}


bool check_tie(struct Candidate arr[], int candidate_count)
{
    // if highest == lowest then all remaining are winners
    int highest = highest_votes(arr, candidate_count);
    int lowest = lowest_votes(arr, candidate_count);
    if (highest == lowest)
    {
        printf("Winner: ");
        bool first_winner = true;
        for (int i = 0; i < candidate_count; i++)
        {
            if (!arr[i].eliminated)
            {
                if (first_winner)
                {
                    printf("%s", arr[i].name);
                    first_winner = false;
                }
                else
                {
                    printf(", %s", arr[i].name);
                }
            }
        }

        return true;
    }
    return false;
}


void eliminate_minority(struct Candidate candidate[], int candidate_count, struct Voter voter[], int voter_count)
{
    int lowest = lowest_votes(candidate, candidate_count);

    for (int i = 0; i < candidate_count; i++)
    {
        if (candidate[i].eliminated || candidate[i].votes != lowest)
        {
            continue;
        }

        candidate[i].eliminated = true;

        for (int j = 0; j < voter_count; j++)
        {
            // Is this voter's current vote for the eliminated candidate?
            if (voter[j].ranking[0] != i)
            {
                continue;
            }

            // Find the voter's next non-eliminated preference.
            for (int k = 1; k < candidate_count; k++)
            {
                int next = voter[j].ranking[k];

                if (!candidate[next].eliminated)
                {
                    candidate[next].votes++;
                    break;
                }
            }
        }

        candidate[i].votes = 0;
    }
}


void winner_finder(struct Candidate candidate[], int candidate_count, struct Voter voter[], int voter_count)
{
    while (true)
    {
        if (check_majority(candidate, candidate_count, voter_count) || check_tie(candidate, candidate_count))
        {
            return;
        }

        eliminate_minority(candidate, candidate_count, voter, voter_count);
    }
}



int main(int argc, char *argv[])
{
    int no_of_candidates = argc - 1;
    if (no_of_candidates < 2 || no_of_candidates > MAX_CANDIDATES)
    {
        printf("Usage: runoff_l_comfortable {atleast 2 candidates, at most 9}");
        return 1;
    }

    // initializing candidate struct
    struct Candidate candidate[no_of_candidates];
    for(int i=0; i < no_of_candidates; i++)
    {
        strcpy(candidate[i].name, argv[i+1]);
        candidate[i].votes = 0;
        candidate[i].eliminated = false;
    }
    merge_sort_struct(candidate, 0, no_of_candidates - 1);

    int no_of_voters;
    printf("No. of Voters: ");
    if (scanf("%d", &no_of_voters) != 1 || no_of_voters <= 0)
    {
        printf("Invalid number of voters.\n");
        return 1;
    }
    struct Voter voter[no_of_voters];

    // getting valid votes
    for (int i = 0; i < no_of_voters; i++)  // per voter
    {
        printf("Voter %d: ", i+1);
        char voted_for[50];
        printf("\n");

        for (int j = 0; j < no_of_candidates; j++) // per rank
        {
            bool valid = false;
            // only getting valid votes, else enter again
            while (!valid)
            {
                printf("Rank %d: ", j+1);
                scanf("%49s", voted_for);
                int result_index = binary_search_struct(candidate, no_of_candidates, voted_for);

                if (result_index == -1)
                {
                    printf("Invalid Vote. Try again.");
                }
                else if (already_ranked(&voter[i], j, result_index))
                {
                    printf("You already ranked that candidate. Try again.");
                }
                else
                {
                    valid = true;
                    voter[i].ranking[j] = result_index; // recording vote in respective rank

                    if (j == 0)
                    {
                        candidate[result_index].votes++; // giving rank 1 a +1 point
                    }
                }
            }
        }

        printf("\n");
    }

    // winner finding
    winner_finder(candidate, no_of_candidates, voter, no_of_voters);
    
    return 0;
}