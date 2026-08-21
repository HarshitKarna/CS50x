//    #  #
//   ##  ##
//  ###  ###
// ####  ####
// till height n

#include<stdio.h>


int get_height(void) // gets height. checks if height is not a number, retries until number is obtained
{
    int n;
    printf("Enter the height of the 'staircase': ");

    while((scanf("%d", &n)) != 1)
    {
        printf("That's not a number. Enter again: ");
    }

    return n;
}


void print_blocks(int n)   //prints the number of blocks as passed
{
    for(int i=0; i<n; i++)
    {
        printf("#");
    }
}


int main(void)
{
    int height;
    do
    {
        height = get_height();
    } while (height<0);

    for(int row=0; row<height; row++)
    {
        for(int spaces=0; spaces<(height-(row+1)); spaces++)
        {
            printf(" ");
        }

        print_blocks(row+1);
        printf("  ");
        print_blocks(row+1); //i found it better to use this small line twice rather than use a whole loop, define new var, just for 2 loops
        printf("\n");
    }
}
