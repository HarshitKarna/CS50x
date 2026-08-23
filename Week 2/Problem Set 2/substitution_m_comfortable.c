/*
Same rules as caesar_l_comfortable, but accept a 26 character key from the user and replace the corresponding ABC... with the given key.

accept each letter only once.
the given key is assumed to correspond to normal lower-case letters.
IF the plaintext has capital letters, turn its equivalent "key" letter into the reverse of what was entered.
i.e. say Hello was entered and the corresponding key of hello is KSbUN. then, since H was actually capital, reverse K to be k instead.
*/

#include<ctype.h>
#include<stdio.h>
#include<string.h>
#include "../cs50.c"
#include "../cs50.h"


// checks if key ascii array has repeats using frequency tracking method
// also checks if it only has alphabets. didnt use another function cause it would make same looping through array repetitive
bool arr_has_repeats_OR_non_alphabets(char arr[26])
{
    int frequency_tracker[123] = {0};

    for(int i=0; i<26; i++)
    {
        // non-alphabets checker
        if(arr[i] < 'A' || (arr[i] > 'Z' && arr[i] < 'a') || arr[i] > 'z')
        {
            return true;
        }
        // frequency checker
        frequency_tracker[arr[i]] += 1;
        if (frequency_tracker[arr[i]] > 1)
        {
            return true;
        }
    }

    return false;
}


int main(int argc, char *argv[])
{
    if(argc != 2 || strlen(argv[1]) != 26)
    {
        printf("Usage: substitution_m_comfortable {26 character long text, no repeats}");
        return 1;
    }

    char secret_key_arr[26];
    strcpy(secret_key_arr, argv[1]);
    bool has_repeats = arr_has_repeats_OR_non_alphabets(secret_key_arr);
    if(has_repeats)
    {
        printf("The Key didnt quite match the requirements. Please run the program again with a different key.");
        return 2;
    }

    // getting input text
    printf("Plaintext: ");
    string plaintext = GetString();
    char ciphertext[strlen(plaintext) + 1];

    // encryption block
    for (int i=0; plaintext[i] != '\0'; i++)
    {
        if (!isalpha(plaintext[i]))
        {
            ciphertext[i] = plaintext[i];
        }
        else if(plaintext[i] >= 'a')
        {
            ciphertext[i] = secret_key_arr[plaintext[i] - 'a'];
        }
        else
        {
            char capital_ignored_val = secret_key_arr[plaintext[i]- 'A'];
            if(capital_ignored_val >= 'a')
            {
                // capitalize it by -32 ascii. can use toupper(), but not taught get so i didnt.
                ciphertext[i] = (char)(capital_ignored_val - ' ');
            }
            else
            {
                // "lowerize" it by +32 ascii
                ciphertext[i] = (char)(capital_ignored_val + ' ');
            }
        }
    }

    ciphertext[strlen(plaintext)] = '\0';
    printf("Ciphertext: %s", ciphertext);
    return 0;
}
