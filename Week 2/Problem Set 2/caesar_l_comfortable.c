/*
Basic Caesar cipher. Accept a positive number by the user when calling out the program, use that number as the secret key. If user didnt give a number, say "Usage: {program_name} {key}" ;and also set the program return code to 1.

Can only rotate between alphabets, cant go into the special symbols territory.
Output must be in the format of:
plaintext:
ciphertext:

NOTE: preserve capitalization
*/

#include<ctype.h>
#include<stdio.h>
#include<string.h>
#include "../cs50.c"
#include "../cs50.h"

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Usage: caesar_l_comfortable {some number}");
        return 1;
    }

    int secret_key = (atoi(argv[1])) % 26;
    printf("Plaintext: ");
    string plaintext = GetString();
    char ciphertext[strlen(plaintext)+1];

    for(int i = 0; plaintext[i] != '\0'; i++)
    {
        if(plaintext[i] >= 'a' && plaintext[i] <= 'z')
        {
            ciphertext[i] = 'a' + ((plaintext[i] - 'a' + secret_key) % 26);
        }
        else if(plaintext[i] >= 'A' && plaintext[i] <= 'Z')
        {
            ciphertext[i] = 'A' + ((plaintext[i] - 'A' + secret_key) % 26);
        }
        else
        {
            ciphertext[i] = plaintext[i];
        }
    }

    ciphertext[strlen(plaintext)] = '\0'; // why does C not do this automatically????
    printf("Ciphertext: %s", ciphertext);
    return 0;
}
