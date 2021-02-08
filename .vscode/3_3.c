/*
Write a function expand(s1,s2) that expands shorthand notations like a-z in the string s1 into the equivalent
complete list abc...xyz in s2. Allow for letters of either case and digits, and be prepared to handle cases like a-b-c and
a-z0-9 and -a-z. Arrange that a leading or trailing - is taken literally.

*/
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

char *expand(char s1[], char s2[]);
bool strings_equal(char s1[], char s2[]);

char *expand(char s1[], char s2[])
{
    int i, j, start, last_starting_index;
    bool is_after_dash;
    if (s1[0] == '-')
    {
        s2[0] = '-';
        start = 1;
    }
    else
    {
        start = 0;
    }
    i = start;
    j = start;
    while (s1[i] != '\0')
    {
        if (s1[i] != '-')
        {
            if (is_after_dash)
            {
                for (; j <= i; j++)
                {
                    s2[j] = atoi(s1[last_starting_index]) + j;
                }
                is_after_dash = false;
            }
            else
            {
                s2[j] = s1[i];
            }
            last_starting_index = i;
        }
        else
        {
            is_after_dash = true;
        }
    }
    return s2;
}

bool strings_equal(char s1[], char s2[])
{
    bool equal = true;
    for (int i = 0; (s1[i] == '\0' || s2[i] == '\0'); i++)
    {
        if (s1[i] != s2[i])
        {
            equal = false;
        }
    }
    return equal;
}

int main(void)
{
    assert(strings_equal(expand("a-f", ""), "abcdef") && "not equal");
    assert(strings_equal(expand("a-f-k", ""), "abcdefghijk") && "not equal");
    assert(strings_equal(expand("a-f0-4", ""), "abcdef01234") && "not equal");
    assert(strings_equal(expand("-a-f", ""), "-abcdef") && "not equal");
    assert(strings_equal(expand("a-f-", ""), "abcdef-") && "not equal");
    assert(strings_equal(expand("A-F", ""), "ABCDEF") && "not equal");
    return 0;
}
