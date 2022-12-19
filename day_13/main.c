
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct Node Node;
struct Node
{
    Node *parent;
    int index;
    

};

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
int find_closing(char *line, int length, int index)
{
    for (size_t i = index; i < length; i++)
    {
        if(line[i] == ']')
        {
            return i;
        }
        if(line[i] == '[')
        {
            int closing = find_closing(line,length,i);
            int *list = parse
        }
    }


    return 0;
    
}
int parse_digit(char *line, int s, int e)
{
    int length = e-s;
    char *digit = alloca(sizeof(char)*(length+1));
    memcpy(digit,&line[s],sizeof(char)*length);
    digit[length] = '\0';
    return atoi(digit);
}
void append_digit(Node *parent, int digit)
{

}
int find_children(Node *parent,char *line)
{
    int index = 0;
    char c = line[index];
    int digit_start = 0,digit_end = 0;
    unsigned int digit_begun = 0;
    while(c != '\0')
    {
        if(c == '[')
        {
             Node *node = malloc(sizeof(Node));
            find_children(node,&line[index]);
        }
        else if(c == ']')
        {
            if(digit_begun)
            {
                digit_end = index;
                int digit = parse_digit(line,digit_start,digit_end);
                append_digit(parent,digit);
                digit_begun = 0;
            }
            return index;
        }
        else if(c == ',')
        {
            digit_end = index;
            int digit = parse_digit(line,digit_start,digit_end);
            append_digit(parent,digit);
            digit_begun = 0;
        }
        else
        {
            if(!digit_begun)
            {
                digit_begun = 1;
                digit_start = index;
            }
        }


        index++;
        c = line[index];
    }
    return -1;
}
void parse(char *line, int length)
{
    
    Node *root = malloc(sizeof(Node));

    find_children(root,&line[1]);

    
}
int main()
{
    srand(time(NULL));

    char* input_raw = load_input("input(example).txt");
    int length = strlen(input_raw);

    int index = 0;
    int start = 0;
    while(index <= length)
    {
        char c = input_raw[index];
        if(c == '\n' || c == '\0')
        {
            int len = index -start;
            char *line = alloca(sizeof(char)*(len+1));
            memcpy(line,&input_raw[start],sizeof(char)*len);
            line[len] = '\0';

            parse(line,len);


            start = index +1;
        }
        index++;
    }

    return 1;
}

