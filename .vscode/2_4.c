#include <stdio.h>
#define MAXLEN 100

void squeeze(char s1[], char s2[]);

void main(){
    char s1[MAXLEN], s2[MAXLEN];
    scanf("%s", s1);
    scanf("%s", s2);
    squeeze(s1, s2);

    printf("%s\n", s1);
}


void squeeze(char s1[], char s2[])
{
    int i, j, k;
    int is_matches = 0;
    for (i = j = 0; s1[i] != '\0'; i++)
    {
        for (k = 0; s2[k] != '\0'; k++)
            if (s1[i] == s2[k])
            {
                is_matches = 1;
                break;
            }

        if (!is_matches)
            s1[j++] = s1[i];
            is_matches = 0;
    }
    s1[j] = '\0';
}
