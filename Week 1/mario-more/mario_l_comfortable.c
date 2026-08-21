/*
   #
  ##
 ###
 till height n 
*/

#include<stdio.h>


int get_height(void) // gets height. checks if input is not a number, retries until number is obtained
{
    int n;
    printf("Enter the height of the 'staircase': ");

    while((scanf("%d", &n)) != 1)
    {
        printf("That's not a number. Enter again: ");
    }

    return n;
}


int main(void)
{
    int input;

    do
    {
        input = get_height();
    }while (input<0);

    for(int i=0; i<input; i++)
    {
        //spaces
        int j= (input - (1+i));
        while(j>0)
        {
            printf(" ");
            j--;
        }

        //actual 'blocks'
        int count = 0;
        while(count != (i+1))
        {
            printf("#");
            count++;
        }
        printf("\n");
    }
}


//can be made better by using for instead of whiles int the main printing block, but my target was to include all the looping structures/methods
