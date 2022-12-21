
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SYMBOL_EMPTY '.'
#define SYMBOL_SOLID '#'
#define SYMBOL_FLUID 'o'
#define SYMBOL_SOURCE '+'

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
void get_boundaries(ListVector2i **vector_lists, int count,Vector2i *min,Vector2i *max)
{
    for (size_t i = 0; i < count; i++)
    {
       for (size_t j = 0; j < vector_lists[i]->count; j++)
       {
            if(min->x > vector_lists[i]->list[j].x)
            {
                min->x = vector_lists[i]->list[j].x;
            }
            if(min->y > vector_lists[i]->list[j].y)
            {
                min->y = vector_lists[i]->list[j].y;
            }

            if(max->x < vector_lists[i]->list[j].x)
            {
                max->x = vector_lists[i]->list[j].x;
            }
            if(max->y < vector_lists[i]->list[j].y)
            {
                max->y = vector_lists[i]->list[j].y;
            }
       }
    }
    




}
void generate_line_segment(char *map, Vector2i v1, Vector2i v2, int stride)
{
    int width = abs(v1.x-v2.x)+1;
    int height = abs(v1.y-v2.y)+1;
    int start_x = (v1.x < v2.x) ? v1.x:v2.x;
    int start_y = (v1.y < v2.y) ? v1.y:v2.y;


    


    for (size_t x = start_x; x < start_x+width; x++)
    {
        for (size_t y = start_y; y < start_y+height; y++)
        {
            int index = y * stride + x;
            map[index] = SYMBOL_SOLID;
        }
    }
    
}
void generate_terrain(ListVector2i **vector_lists, int count,int stride, char *map)
{
    for (size_t i = 0; i < count; i++)
    {
        ListVector2i *line_list = vector_lists[i];

        for (size_t j = 0; j < line_list->count-1; j++)
        {
            generate_line_segment(map,line_list->list[j],line_list->list[j+1],stride);
        }
    }
    
}
void debug_print(char *map, int w, int h)
{
    for (size_t y = 0; y < h ; y++)
    {
       for (size_t x = 0; x < w; x++)
       {
            int index = y * w  + x;
            printf("%c",map[index]);
       }
       printf("\n");
    }
}
int main()
{
    char* input_raw = load_input("input.txt");
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

    Vector2i min,max;
    min.x = 100000;
    min.y = 100000;
    max.x = 0;
    max.y = 0;

    

    get_boundaries(vector_lists,count,&min,&max);


    for (size_t i = 0; i < count; i++)
    {
       for (size_t j = 0; j < vector_lists[i]->count; j++)
       {
            vector_lists[i]->list[j].x -= min.x;
       }
    }
    max.x -= min.x;
    

   

    int WIDTH =  max.y*max.x;
    int HEIGHT = 2+max.y;

    int area = WIDTH *HEIGHT;

    char *map = malloc(sizeof(char)*area);

    for (size_t i = 0; i < area; i++)
    {
        map[i] = SYMBOL_EMPTY;
    }

    generate_terrain(vector_lists,count,WIDTH ,&map[0]);



    Vector2i opening = {500-min.x,0};
    map[opening.y*WIDTH+opening.x] = SYMBOL_SOURCE;

    
    
    Vector2i particle = {opening.x,opening.y};
    int particle_count = 0;
    while(1)
    {
        //particle.y ++;
        int Y = particle.y;
        int X = particle.y;

        int index = (particle.y + 1) * WIDTH + particle.x;
        
        
        if(particle.y == HEIGHT-1)
        {

        }
        else if(map[index] == SYMBOL_SOLID || map[index] == SYMBOL_FLUID)
        {
            int index_left = (particle.y + 1) * WIDTH + (particle.x-1);
            if(map[index_left] == SYMBOL_SOLID || map[index_left] == SYMBOL_FLUID)
            {
                int index_right = (particle.y + 1) * WIDTH + (particle.x+1);
                if(map[index_right] == SYMBOL_SOLID || map[index_right] == SYMBOL_FLUID)
                {

                }
                else
                {

                    particle.y ++;
                    particle.x ++;
                }
            }
            else
            {
                particle.y ++;
                particle.x --;
            }
        }
        else
        {
            particle.y ++;
        }

        map[particle.y * WIDTH+particle.x] = SYMBOL_FLUID;

        //debug_print(map,WIDTH,HEIGHT);





        if(Y == particle.y)
        {

            particle_count++;
            if(particle.y == opening.y && particle.x == opening.x)
            {
                printf("%i\n",particle_count);
                return 0;
            }
            particle.x = opening.x;
            particle.y = opening.y;
        }
        else
        {
            map[particle.y * WIDTH+particle.x] = SYMBOL_EMPTY;
        }

        

//part1
/*
        if(particle.y > max.y)
        {
            printf("%i\n",particle_count);
            return;
        }*/
    

    }
    

    return 1;
}

