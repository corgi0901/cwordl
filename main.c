#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "wordlist.h"

#define WORD_LEN    5
#define MAX_TRY     6
#define INPUT_MAX_LEN 256
#define WORD_LIST_LEN (sizeof(wordlist)/sizeof(wordlist[0]))

char status[26] = {0};

void print_green(char c)
{
    printf("\x1b[30m");
    printf("\x1b[42m %c \x1b[m", c);
    printf("\x1b[0m");
}

void print_yellow(char c)
{
    printf("\x1b[30m");
    printf("\x1b[43m %c \x1b[m", c);
    printf("\x1b[0m");
}

void print_gray(char c)
{
    printf("\x1b[30m");
    printf("\x1b[47m %c \x1b[m", c);
    printf("\x1b[0m");
}

void to_upper(char *str)
{
    for(int i=0; str[i] != '\0'; i++)
    {
        if('a' <= str[i] && str[i] <= 'z')
        {
            str[i] = 'A' + (str[i] - 'a');
        }
    }
}

int check(char* ans, char* input)
{
    for(int i=0; i<WORD_LEN; i++)
    {
        char c = input[i];

        if(ans[i] == c)
        {
            print_green(c);
            status[c - 'A'] = 1;
            continue;
        }

        int match = 0;

        for(int j=0; j<WORD_LEN; j++)
        {
            if(ans[j] == c)
            {
                print_yellow(c);
                match = 1;
                if(status[c - 'A'] == 0) status[c - 'A'] = 2;
                break;
            }
        }

        if(match == 0)
        {
            print_gray(c);
            status[c - 'A'] = 3;
        }
    }
    printf("\n");

    return (strcmp(ans, input) == 0) ? 1 : 0;
}

int is_acceptable(char input[INPUT_MAX_LEN])
{
    if(strlen(input) != WORD_LEN)
    {
        printf("Input must be just %d characters\n", WORD_LEN);
        return 0;
    }

    for(int i=0; i<WORD_LEN; i++)
    {
        char c = input[i];
        if(!(('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')))
        {
            printf("You can use only \'a\'~\'z\' and \'A\'~\'Z\'\n");
            return 0;
        }
    }

    to_upper(input);

    for(int i=0; i<WORD_LIST_LEN; i++)
    {
        if(strcmp(wordlist[i], input) == 0)
        {
            return 1;
        }
    }

    printf("\"%s\" is not in word list\n", input);

    return 0;
}

int main(void)
{
    char *answer;
    int count = 0;
    int clear = 0;
    char record[MAX_TRY][INPUT_MAX_LEN] = {0};

    srand((unsigned int)time(NULL));

    int index = rand() % WORD_LIST_LEN;
    answer = wordlist[index];

    while(count < MAX_TRY && !clear)
    {
        char input[INPUT_MAX_LEN] = {0};

        for(int i=0; i<sizeof(status); i++)
        {
            switch(status[i])
            {
                case 0:
                    print_gray('A' + i);
                    break;
                case 1:
                    print_green('A' + i);
                    break;
                case 2:
                    print_yellow('A' + i);
                    break;
                case 3:
                    print_gray(' ');
                    break;
                default:
                    break;
            }
        }
        printf("\n");

        printf("(%d/%d) Your answer : ", count+1, MAX_TRY);
        scanf("%s", input);

        printf("\n");

        if(is_acceptable(input))
        {
            strcpy(record[count], input);

            for(int i=0; i<=count; i++)
            {
                if(check(answer, record[i]))
                {
                    clear = 1;
                    break;
                }
            }
            count++;
        }

        printf("\n");
    }

    if(clear)
    {
        printf("Congratulations!\n");
    }
    else 
    {
        printf("Answer = %s\n", wordlist[index]);
    }

    return 0;
}