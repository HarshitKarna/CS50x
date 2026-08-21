/*
This program checks whether a credit card number is valid using Luhn’s algorithm and verifies whether it matches the format of an American Express, MasterCard, or Visa card. It reads the card number, applies the checksum, checks its length and starting digits, and prints AMEX, MASTERCARD, VISA, or INVALID.

CONTEXT

luns algorithm says: 
Multiply every other digit by 2, starting with the number’s second-to-last digit, and then add those products’ digits together.

Add the sum to the sum of the digits that weren’t multiplied by 2.

If the total’s last digit is 0 (or, put more formally, if the total modulo 10 is congruent to 0), the number is valid!

AMEX: starts with 34, 37. 15 digits
MASTERCARD: starts with 51, 52, 53, 54, 55. 16 digits
VISA: starts with 4. 13 and 16 digits
*/

#include<stdio.h>
#include<math.h>
#include<stdbool.h>
#include<stdlib.h>


int get_length(long long n)
{
    int length = 0;
    while(n != 0)
    {
        n /= 10;
        length++;
    }
    return length;
}


int first_digit(long long n)
{
    n = llabs(n);
    while (n >= 10)
    {
        n /= 10;
    }

    return n;
}


int first_two_digits(long long n)
{
    n = llabs(n);
    while (n >= 100)
    {
        n /= 10;
    }

    return n;
}


bool validate_card(long long n)
{
    int sum = 0;
    int loop_counter = 0;
    int curr_digit;

    while(n != 0)
    {
        loop_counter++;
        curr_digit = n % 10;
        if((loop_counter % 2) == 1)
        {
            sum += curr_digit;
        }
        else
        {
            curr_digit *= 2;
            while(curr_digit != 0)
            {
                sum += (curr_digit % 10);
                curr_digit /= 10;
            }
        }
        n /= 10;
    }

    return ((sum % 10) == 0);
}


int main(void)
{
    long long input_card_no;
    printf("Enter a card number: ");
    while((scanf("%lld", &input_card_no)) != 1 || input_card_no == 0)
    {
        printf("Thats not a real number. Enter again: ");
        // there is an issue where if letters are entered, it asks again, but the letters arent "cleared" from the memory. if abc is entered, 'a' still persists instead of giving a clear empty input "box"
    }

    if(validate_card(input_card_no) == false)
    {
        printf("INVALID\n");
        exit(EXIT_SUCCESS);
    }

    // storing values that are required multiple times
    int card_length = get_length(input_card_no);
    int frst_dgt = first_digit(input_card_no);
    int frst_2_dgt = first_two_digits(input_card_no);

    if(card_length == 16)
    {
        if(frst_2_dgt >= 51 && frst_2_dgt <= 55)
        {
            printf("MASTERCARD\n");
        }
        else if (frst_dgt == 4)
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else if (card_length == 15)
    {
        if (frst_2_dgt == 34 || frst_2_dgt == 37)
        {
            printf("AMEX\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else if (card_length == 13 && frst_dgt == 4)
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }
    
}
