#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <ctype.h>
#include "wordlist.h"

#define WORD_LEN    5
#define MAX_TRY     6
#define INPUT_MAX_LEN 256
#define WORD_LIST_LEN (sizeof(wordlist)/sizeof(wordlist[0]))

#define RECORD_LINE 1
#define CHAR_LINE   8
#define INPUT_LINE  9
#define MSG_LINE    11

char status[26] = {0};
char record[MAX_TRY][INPUT_MAX_LEN] = {0};

enum {
    GREEN = 1,
    YELLOW,
    WHITE,
};

void printc_with_color(int color, char c)
{
    attron(COLOR_PAIR(color));
    printw(" %c ", c);
    attroff(COLOR_PAIR(color));
    refresh();
}

void clear_line(int line)
{
    move(line, 0);
    deleteln();
    insertln();
}

void to_upper(char *str)
{
    for(int i=0; str[i] != '\0'; i++)
    {
        str[i] = toupper(str[i]);
    }
}

int check(char* ans, char* input)
{
    for(int i=0; i<WORD_LEN; i++)
    {
        char c = input[i];

        if(ans[i] == c)
        {
            status[c - 'A'] = 1;
            continue;
        }

        int match = 0;

        for(int j=0; j<WORD_LEN; j++)
        {
            if(ans[j] == c)
            {
                match = 1;
                if(status[c - 'A'] == 0) status[c - 'A'] = 2;
                break;
            }
        }

        if(match == 0)
        {
            status[c - 'A'] = 3;
        }
    }

    return (strcmp(ans, input) == 0) ? 1 : 0;
}

int is_acceptable(char input[INPUT_MAX_LEN])
{
    if(strlen(input) != WORD_LEN)
    {
        mvprintw(MSG_LINE, 0, "Input must be just %d characters\n", WORD_LEN);
        return 0;
    }

    for(int i=0; i<WORD_LEN; i++)
    {
        char c = input[i];
        if(!(('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')))
        {
            mvprintw(MSG_LINE, 0, "You can use only \'a\'~\'z\' and \'A\'~\'Z\'\n");
            return 0;
        }
    }

    to_upper(input);

    for(int i=0; i<WORD_LIST_LEN; i++)
    {
        if(strcmp(wordlist[i], input) == 0)
        {
            clear_line(MSG_LINE);
            return 1;
        }
    }

    mvprintw(MSG_LINE, 0, "\"%s\" is not in word list\n", input);

    return 0;
}

void print_character(void)
{
    move(CHAR_LINE, 0);
    for(int i=0; i<sizeof(status); i++)
    {
        switch(status[i])
        {
            case 0:
                printc_with_color(WHITE, 'A' + i);
                break;
            case 1:
                printc_with_color(GREEN, 'A' + i);
                break;
            case 2:
                printc_with_color(YELLOW, 'A' + i);
                break;
            case 3:
                printc_with_color(WHITE, ' ');
                break;
            default:
                break;
        }
    }
    refresh();
}

void print_word(char* ans, char* input)
{
    for(int i=0; i<WORD_LEN; i++)
    {
        int match = 0;
        char c = input[i];

        if(ans[i] == c)
        {
            printc_with_color(GREEN, c);
            continue;
        }

        for(int j=0; j<WORD_LEN; j++)
        {
            if(ans[j] == c)
            {
                match = 1;
                printc_with_color(YELLOW, c);
                break;
            }
        }

        if(match == 0) printc_with_color(WHITE, c);
    }

    printw("\n");
}

void print_record(char* answer, int count)
{
    move(RECORD_LINE, 0);

    for(int i=0; i<count; i++)
    {
        print_word(answer, record[i]);
    }

    for(int i=count; i<MAX_TRY; i++)
    {
        print_word(answer, "_____");
    }
}

int main(void)
{
    char *answer;
    int count = 0;
    int clear = 0;

    /* init screen */
    initscr();
    erase();

    /* init color */
    start_color();
    init_pair(GREEN, COLOR_BLACK, COLOR_GREEN);
    init_pair(YELLOW, COLOR_BLACK, COLOR_YELLOW);
    init_pair(WHITE, COLOR_BLACK, COLOR_WHITE);

    /* select answer */
    srand((unsigned int)time(NULL));
    answer = wordlist[rand() % WORD_LIST_LEN];

    while(count < MAX_TRY && !clear)
    {
        char input[INPUT_MAX_LEN] = {0};

        print_record(answer, count);
        print_character();

        clear_line(INPUT_LINE);
        mvprintw(INPUT_LINE, 0, "(%d/%d) Your answer : ", count+1, MAX_TRY);
        refresh();

        scanw("%s", input);

        if(strcmp("exit", input) == 0)
        {
            endwin();
            return 0;
        }

        if(is_acceptable(input))
        {
            strcpy(record[count++], input);
            if(check(answer, input)) clear = 1;
        }
    }

    print_record(answer, count);
    print_character();

    if(clear)
    {
        mvprintw(MSG_LINE, 0, "Congratulations!\n");
    }
    else 
    {
        mvprintw(MSG_LINE, 0, "Answer = %s\n", answer);
    }

    refresh();
    getch();
    endwin();

    return 0;
}