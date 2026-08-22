/*
Suppose you work at a store and a customer gives you $1.00 (100 cents) for candy that costs $0.50 (50 cents). You’ll need to pay them their “change,” the amount leftover after paying for the cost of the candy. When making change, odds are you want to minimize the number of coins you’re dispensing for each customer, lest you run out (or annoy the customer!). Implement a program in C that prints the minimum coins(per type) needed to make the given amount of change, in cents. (1, 5, 10, 20 cent coins)
*/

#include<stdio.h>

int main(void)
{
    int change_owed;
    printf("Enter the change owed in cents: ");
    while ((scanf("%u", &change_owed)) != 1 || change_owed == 0)
    {
        printf("Thats not a valid number. Enter again: ");
    }
    
    int no_of_20_owed = change_owed / 20;
    change_owed %= 20;
    int no_of_10_owed = change_owed / 10;
    change_owed %= 10;
    int no_of_5_owed = change_owed / 5;
    change_owed %= 5;
    int no_of_1_owed = change_owed;

    printf("Number of coins per type owed:\n");
    printf("20: %d\n", no_of_20_owed);
    printf("10: %d\n", no_of_10_owed);
    printf("5: %d\n", no_of_5_owed);
    printf("1: %d", no_of_1_owed);
}
