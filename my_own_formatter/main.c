// Online C compiler to run C program online
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char uint8;
typedef unsigned int uint32;
typedef unsigned short uint16;

 /* reverse:  reverse string s in place */
 void reverse(char s[])
 {
     int i, j;
     char c;
 
     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }

/* itoa:  convert n to characters in s */
 void itoa(int n, char s[])
 {
     int i, sign;
 
     if ((sign = n) < 0)  /* record sign */
         n = -n;          /* make n positive */
     i = 0;
     do {       /* generate digits in reverse order */
         s[i++] = n % 10 + '0';   /* get next digit */
     } while ((n /= 10) > 0);     /* delete it */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
 }

static uint8 *format(uint8 *formatter, size_t args, uint8 **list)
{
    /* Keep current string so far. */
    uint8 current_string[1024];
    size_t cs_index = 0;
    
    /* Common variables. */
    size_t index = 0;
    size_t list_index = 0;
    
    /* Set all indexes in `current_string` to zero. */
    memset(current_string, 0, 1024);
    
    /* Checks. */
redo:
    if(!(formatter))
    {
        return NULL;
    }
    
    if(formatter[index] == '%')
    {
        /* Skip % */
        index++;
        
        /* If `list_index` is greater than `args`, error. */
        if(list_index > args)
        {
            fprintf(stderr, "Too many formatters.\n");
            exit(1);
        }
        
        /* Get the character after `%`. */
        switch(formatter[index])
        {
            case 's':
            {
                /* Concatenate, advance and continue. */
                strcat(current_string, (uint8 *)list[list_index]);
                cs_index+=strlen(list[list_index]);
                list_index++;
                break;
            }
            case 'c':
            {
                /* Concatenate, advance and continue. */
                strcat(current_string, (uint8 *)list[list_index]);
                cs_index++;
                list_index++;
                break;
            }
            case 'd':
            {
                /* Convert from integer to string. */
                uint8 buff[10];
                itoa(list[list_index], buff);
                
                /* Concatenate, advance and continue. */
                strcat(current_string, (uint8 *)buff);
                cs_index+=strlen(buff);
                list_index++;
                break;
            }
            default: return NULL;
        }
        
        /* Skip the formatter type. */
        index++;
    }
    
    if(index+1 >= strlen(formatter)) goto end;
    
    /* Append character in static-memory `current_string`. */
    current_string[cs_index] = formatter[index];
    cs_index++;
    index++;
    
    /* Are we at the end? */
    if(!(formatter[index] == '\0')) goto redo;
    
    /* Reassign the formatter to the string that has been getting
     * manipulated throughout the lifetime of the function.
     * `current_string` now stores the implied string.
     */
end:
    formatter = current_string;
    
    return formatter;
}

int main() {
    // Write C code here
    uint16 **list = calloc(2, sizeof(*list));
    list[0] = calloc(1, sizeof(*list[0]));
    list[0] = (uint16 *)"a";
    list[1] = calloc(1, sizeof(*list[1]));
    list[1] = (uint16)32;
    
    uint8 *f = format("This is `a`: %c\nThis is 32: %d", 2, list);
    printf("%s", f);

    return 0;
}
