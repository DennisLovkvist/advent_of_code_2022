
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

static char* load_input(char *path)
{
    FILE *f = fopen(path, "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *source_code = malloc(fsize + sizeof(char));
    fread(source_code, fsize, 1, f);
    fclose(f);
    source_code[fsize] = '\0';
    return source_code;
}
void part1()
{
    char* input_raw = load_input("input.txt");
    int length = strlen(input_raw);

    int groups[4];
    int index = 0,sum = 0,count = 0;
    char digits[3] = {'\0','\0','\0'};

    for (size_t i = 0; i < length+1; i++)
    {
        char c = input_raw[i];
        int line_feed = (c == '\n'|| i == length);
        int delimiter = (c == '-' || c == ',');
        
        if(line_feed || delimiter)
        {
            groups[index] = atoi(digits);
            digits[0] = digits[1] = '\0';
            index ++;
            count = 0;
        
            if(line_feed)
            {
                index = 0;
                sum += ((groups[0] <= groups[2]) * (groups[1] >= groups[3])) || ((groups[2] <= groups[0]) * (groups[3] >= groups[1]));
            } 
        }
        else
        {
            digits[count++] = c;                
        }  
    }
    printf("%i%s", sum, " pairs contain eachother");
}
void part2()
{
    char* input_raw = load_input("input.txt");
    int length = strlen(input_raw);

    int groups[4];
    int index = 0,sum = 0,count = 0;
    char digits[3] = {'\0','\0','\0'};

    for (size_t i = 0; i < length+1; i++)
    {
        char c = input_raw[i];
        int line_feed = (c == '\n'|| i == length);
        int delimiter = (c == '-' || c == ',');
        
        if(line_feed || delimiter)
        {
            groups[index] = atoi(digits);
            digits[0] = digits[1] = '\0';
            index ++;
            count = 0;
        
            if(line_feed)
            {
                index = 0;
                sum += ((groups[0] <= groups[2]) * (groups[2] <= groups[1])) || ((groups[2] <= groups[0]) * (groups[0] <= groups[3]));//void part2() is the same as void part1() except this line
            } 
        }
        else
        {
            digits[count++] = c;                
        }  
    }
    printf("%i%s", sum, " pairs overlaps");
}
int main()
{
    //part1();
    part2();
    return 1;
}

