#include "minishell.h"

int argv_len(char **p)
{
    int i;

    i = 0;
    while(p[i] != NULL)
    {
        i++;
    }
    return (i);
}

char *del_spaces(char *str)
{
    int len;
    
    len = 0;
    if(str)
    {
        while(*str)
        {
            if (*str == ' ')
                str++;
            else
                break;
        }
        
        len = ft_strlen(str) - 1;
        while (*(str + len) == ' ')
            len--;
        str = ft_substr(str, 0, len + 1); // malloc
    }
    return (str);
}