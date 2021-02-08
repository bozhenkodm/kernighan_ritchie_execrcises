#include <stdio.h>
#define MAXLEN 100

int htoi(char s[]);
int lower(int c);

void main(){
    char hex[MAXLEN];
    scanf("%s", hex);
    printf("%d\n", htoi(hex));
}

int lower(int c)
{
    if (c >= 'A' && c <= 'Z')
        return c + 'a' - 'A';
    else
        return c;    
}

int htoi(char s[])
{
    int i, start = 0;
    int n;
    char c;
    n = 0;
    if (s[0] == '0') start++;
    if (lower(s[1]) == 'x') start++;
    for (
        i = start; 
        (s[i] >= '0' && s[i] <= '9') 
        || (s[i] >= 'a' && s[i] <= 'f')  
        || (s[i] >= 'A' && s[i] <= 'F'); 
        ++i
    )
    {
        c = lower(s[i]);
        if (c >= '0' && c <= '9')
            n = 16 * n + (c - '0');
        if (c >= 'a' && c <= 'f')
            n = 16 * n + 10 + (c - 'a');
    }
    return n;
}
