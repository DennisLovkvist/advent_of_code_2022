
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct Vector2i Vector2i;
struct Vector2i
{
    int x,y;
};
typedef struct ListVector2i ListVector2i;
struct ListVector2i
{
    Vector2i *list;
    int count;
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
int parse_number(char *line, int s, int e)
{
    int length = e-s;
    char *digit = alloca(sizeof(char)*(length+1));
    memcpy(digit,&line[s],sizeof(char)*length);
    digit[length] = '\0';
    return atoi(digit);
}
void list_append(ListVector2i *vector_list,int x, int y)
{
    vector_list->list = realloc(&vector_list->list[0],sizeof(Vector2i*)*(vector_list->count+1));
    vector_list->list[vector_list->count].x = x;
    vector_list->list[vector_list->count].y = y;
    vector_list->count ++;
}
ListVector2i *parse_line(char *line)
{
    int index = 0;
    char c = line[index];

    int start = 0;
    int x = 0,y= 0;

    ListVector2i *vecotrs = malloc(sizeof(ListVector2i));
    vecotrs->list = malloc(0);
    vecotrs->count = 0;

    while (c != '\0')
    {
        if(c == ',')
        {
            x = parse_number(line,start,index);
            start = index+1;
        }
        else if(c == ' ' && line[index+1] == '-')
        {
            y = parse_number(line,start,index);
            list_append(vecotrs,x,y);
            start = index+3;
        }
        index++;
        c = line[index];
    }

    y = parse_number(line,start,index);
    list_append(vecotrs,x,y);
    return vecotrs;
}
void normalize_x(ListVector2i **vector_lists, int count)
{
    int smallest_x = vector_lists[0]->list[0].x;

    for (size_t i = 0; i < count; i++)
    {
       for (size_t j = 0; j < vector_lists[i]->count; j++)
       {
            if(smallest_x > vector_lists[i]->list[j].x)
            {
                smallest_x = vector_lists[i]->list[j].x;
            }
       }
    }
    for (size_t i = 0; i < count; i++)
    {
       for (size_t j = 0; j < vector_lists[i]->count; j++)
       {
            vector_lists[i]->list[j].x -= smallest_x;
       }
    }
}
int main()
{
    char* input_raw = load_input("input(example).txt");
    int length = strlen(input_raw);

    int index = 0;
    int start = 0;

    int count = 0;

    while(index <= length)
    {
        char c = input_raw[index];
        if(c == '\n' || c == '\0')
        {
            count ++;
        }
        index++;
    }

    ListVector2i **vector_lists = malloc(sizeof(ListVector2i*)*count);
    int v_index = 0;
    index = 0;
    while(index <= length)
    {
        char c = input_raw[index];
        if(c == '\n' || c == '\0')
        {
            int len = index -start;
            if(len > 0)
            {
                char *line = alloca(sizeof(char)*(len+1));
                memcpy(line,&input_raw[start],sizeof(char)*len);
                line[len] = '\0';

                vector_lists[v_index] = parse_line(line);
                v_index++;
            }

            start = index +1;
        }
        
        index++;
    }

    normalize_x(vector_lists,count);

    for (size_t i = 0; i < count; i++)
    {
       for (size_t j = 0; j < vector_lists[i]->count; j++)
       {
            printf("%i,%i -> ",vector_lists[i]->list[j].x,vector_lists[i]->list[j].y);
       }
       printf("\n");
    }
    
    

    return 1;
}

