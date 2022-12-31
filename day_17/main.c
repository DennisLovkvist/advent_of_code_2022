
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct Shape Shape;
struct Shape
{
    int x, y,w,h;
    int type;
    unsigned int resting;
};

typedef struct Snapshot Snapshot;
struct Snapshot
{
    int instruction_index;
    int type,id, highest;
    int delta[32];
};

static char *load_input(char *path)
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
    int length = e - s;
    char *digit = alloca(sizeof(char) * (length + 1));
    memcpy(digit, &line[s], sizeof(char) * length);
    digit[length] = '\0';
    return atoi(digit);
}
unsigned int shape_to_shape_collision(Shape *shape,Shape *resting_shape,char *shape_data[5])
{
    if(shape->x + shape->w - 1 >= resting_shape->x && shape->x <= resting_shape->x + resting_shape->w-1)
    {
        if(shape->y + shape->h >= resting_shape->y)
        {

            for (int i = 0; i < shape->w; i++)
            {
                for (int j = 0; j < shape->h; j++)
                {
                    int coord1 = j * shape->w + i;
                    char c1 = shape_data[shape->type][coord1];

                    if(c1 == '#')
                    {
                        int x1 = shape->x + i;
                        int y1 = shape->y + j;

                        for (int k = 0; k < resting_shape->w; k++)
                        {
                            for (int l = 0; l < resting_shape->h; l++)
                            {
                                int coord2 = l * resting_shape->w + k;
                                char c2 = shape_data[resting_shape->type][coord2];

                                if(c2 == '#')
                                {
                                    int x2 = resting_shape->x + k;
                                    int y2 = resting_shape->y + l;

                                    if(x2 == x1 && y2 == y1)
                                    {
                                        return 1;
                                    }
                                }
                            }
                        }
                    }
                }   
            }
        }
    }
    return 0;
}
unsigned int collision_h(Shape **shape_resting, int shapes_resting_count, Shape *shape,int x,char *shape_data[5])
{
    if(shape->x + x < 1)
    {
        return 1;
    }
    else if(shape->x + x + shape->w > 8)
    {
        return 1;
    }
    for (size_t i = 0; i < shapes_resting_count; i++)
    {
        Shape *resting_shape = shape_resting[i];
        shape->x +=x;
        int ssc = shape_to_shape_collision(shape,resting_shape,shape_data);
        shape->x -=x;
        if(ssc)
        {
            return 1;
        }
    }
    return 0;
}
unsigned int collision_v(Shape **shape_resting, int shapes_resting_count, Shape *shape,int y,char *shape_data[5])
{
    if(shape->y + y +shape->h-1 >= 0)
    {
        return 1;
    }
    for (size_t i = 0; i < shapes_resting_count; i++)
    {
        Shape *resting_shape = shape_resting[i];
        shape->y ++;
        int ssc = shape_to_shape_collision(shape,resting_shape,shape_data);
        shape->y --;
        if(ssc)
        {
            return 1;
        }
    }
    return 0;
}
void shape_spawn(int strides[5],int lengths[5],Shape *shape,int spawn_x,int spawn_y,int type)
{
    shape->type = type;
    shape->w = strides[type];
    shape->h = lengths[type] / strides[type];
    shape->x = spawn_x;
    shape->y = spawn_y - (lengths[type]/strides[type]);
    shape->resting = 0;
}
void part1()
{
    char shape_0[4] = {'#','#','#','#'};

    char shape_1[9] = 
    {
        '.','#','.',
        '#','#','#',
        '.','#','.'
    };

    char shape_2[9] = 
    {
        '.','.','#',
        '.','.','#',
        '#','#','#'
    };

    int strides[5] = {4,3,3,1,2};
    int lengths[5] = {4,9,9,4,4};
    char *shape_data[5] = {&shape_0[0],&shape_1[0],&shape_2[0],&shape_0[0],&shape_0[0]};
    
    char *input_raw = load_input("input.txt");
    int length = strlen(input_raw);


    int index = 0;
    int count = 0;
    while (index <= length)
    {
        char c = input_raw[index];
        if (c == '<' || c == '>')
        {
           count ++;
        }
        index++;
    }
    
    int instruction_count = count;
    char *instructions = malloc(sizeof(char)*instruction_count);
    index = 0;
    count = 0;
    while (index <= length)
    {
        char c = input_raw[index];

        if (c == '<' ||  c == '>')
        {
           instructions[count] = c;
           count ++;
        }
        index++;
    }

    Shape *shape_pool = malloc(sizeof(Shape)*2022);
    int current_shape = 0;

    Shape **shapes_resting = malloc(sizeof(Shape*)*2022);
    int shapes_resting_count = 0;
    
    Shape *shape = &shape_pool[current_shape];

    int current_type = 0;

    shape_spawn(strides,lengths,shape,3,-4,current_type);

    unsigned int gravity = 1;
    int rocks = 0;

    int instruction = 0;
    int highest = 0;
    while(1)
    {
        
        if(shape->resting)
        {
            if(highest > shape->y)
            {
                highest = shape->y;
            }
            rocks ++;


            if(rocks < 2022)
            {
                current_type ++;
                if(current_type > 4)
                {
                    current_type = 0;
                }
                int spawn_y = highest - 3;

                current_shape ++;
                shape = &shape_pool[current_shape];

                shape_spawn(strides,lengths,shape,3,spawn_y - shape->h,current_type);
            }
            else
            {
                goto end;
            }
            
        }
        else
        {
            if(gravity)
            {
                if(!collision_v(&shapes_resting[0],shapes_resting_count,shape,1,shape_data))
                {
                    shape->y++;
                }
                else
                {
                    shape->resting = 1;
                    shapes_resting[shapes_resting_count] = shape;
                    shapes_resting_count++;

                    if(highest > shape->y)
                    {
                        highest = shape->y;
                    }

                }
            }
            else
            {
                int tx = (instructions[instruction] == '<' ? -1:1);

                
                instruction = instruction +1 < instruction_count ? instruction + 1:0; 

                if(!collision_h(&shapes_resting[0],shapes_resting_count,shape,tx,shape_data))
                {
                    shape->x+=tx;
                }

            }

            gravity = !gravity;
        }
        
        //sleep(1);
/*
        for (int y = -20; y <= 0; y++)
        {
            char scan_line[10] = {'|','.','.','.','.','.','.','.','|','\0'};

            if(y != 0)
            {
                for (int x = 0; x < 9; x++)
                {
                    for (size_t i = 0; i < shapes_resting_count; i++)
                    {
                        Shape *s = shapes_resting[i];
                        int type = s->type;
                        int stride = strides[type];
                        int height = lengths[type] / stride;
                        if(x >= s->x && x < s->x + stride)
                        {
                            if(y >= s->y && y < s->y + height)
                            {
                                int local_x = x - s->x;
                                int local_y = y - s->y;
                                int coord = local_y * stride + local_x;

                                if(shape_data[type][coord] == '#')
                                { 
                                    scan_line[x] = '#';
                                }
                            }
                        }
                    }
                    
                    
                    if(!shape->resting)
                    {
                        int type = shape->type;
                        int stride = strides[type];
                        int height = lengths[type] / stride;
                        if(x >= shape->x && x < shape->x + stride)
                        {
                            if(y >= shape->y && y < shape->y + height)
                            {
                                int local_x = x - shape->x;
                                int local_y = y - shape->y;
                                int coord = local_y * stride + local_x;
                                if(shape_data[type][coord] == '#')
                                { 
                                    scan_line[x] = '@';
                                }
                            }
                        }
                    }
                        
                        
                    
                }
                printf("%s\n",scan_line);
            }
            else
            {
                printf("%s\n","---------");
            }
        }

        */


        

        



        
        
    }

    end:
       



    
    printf("%i\n",highest);
}
#define MAX 30000

void part2()
{
    char shape_0[4] = {'#','#','#','#'};

    char shape_1[9] = 
    {
        '.','#','.',
        '#','#','#',
        '.','#','.'
    };

    char shape_2[9] = 
    {
        '.','.','#',
        '.','.','#',
        '#','#','#'
    };

    int strides[5] = {4,3,3,1,2};
    int lengths[5] = {4,9,9,4,4};
    char *shape_data[5] = {&shape_0[0],&shape_1[0],&shape_2[0],&shape_0[0],&shape_0[0]};
    
    char *input_raw = load_input("input.txt");
    int length = strlen(input_raw);

    int index = 0;
    int count = 0;
    while (index <= length)
    {
        char c = input_raw[index];
        if (c == '<' || c == '>')
        {
           count ++;
        }
        index++;
    }
    
    int instruction_count = count;
    char *instructions = malloc(sizeof(char)*instruction_count);
    index = 0;
    count = 0;
    while (index <= length)
    {
        char c = input_raw[index];

        if (c == '<' ||  c == '>')
        {
           instructions[count] = c;
           count ++;
        }
        index++;
    }

    Shape *shape_pool = malloc(sizeof(Shape)*MAX);
    int current_shape = 0;

    Shape **shapes_resting = malloc(sizeof(Shape*)*MAX);
    int shapes_resting_count = 0;
    
    Shape *shape = &shape_pool[current_shape];

    int current_type = 0;

    shape_spawn(strides,lengths,shape,3,-4,current_type);

    unsigned int gravity = 1;
    int rocks = 0;

    int instruction = 0;
    int highest = 0;
    int highest_prev = 0;

    Snapshot *snapshots = malloc(sizeof(Snapshot)*MAX);
    int snapshot_index = 0;

    int *deltas = malloc(sizeof(int)*MAX);
    int delta_index = 0;

    while(rocks < MAX)
    {
        
        if(shape->resting)
        {
            if(highest > shape->y)
            {
                highest_prev = highest;
                highest = shape->y;

                deltas[delta_index++] = abs(highest - highest_prev);
            }

            if(rocks > 1000)
            {
                Snapshot *snapshot = &snapshots[snapshot_index];
                snapshot->id = rocks;
                snapshot->highest = abs(highest);
                snapshot->instruction_index = instruction;
                snapshot->type = current_type;
                memcpy(&snapshot->delta[0],&deltas[delta_index-32],sizeof(int)*32);
                snapshot_index++;
            }

            current_type ++;
            if(current_type > 4)
            {
                current_type = 0;
            }
            int spawn_y = highest - 3;

            current_shape ++;
            shape = &shape_pool[current_shape];

            shape_spawn(strides,lengths,shape,3,spawn_y - shape->h,current_type);

            rocks ++;
            
        }
        else
        {
            if(gravity)
            {
                if(!collision_v(&shapes_resting[0],shapes_resting_count,shape,1,shape_data))
                {
                    shape->y++;
                }
                else
                {
                    shape->resting = 1;
                    shapes_resting[shapes_resting_count] = shape;
                    shapes_resting_count++;

                    if(highest > shape->y)
                    {
                        highest = shape->y;
                    }

                }
            }
            else
            {
                int tx = (instructions[instruction] == '<' ? -1:1);

                
                instruction = instruction +1 < instruction_count ? instruction + 1:0; 

                if(!collision_h(&shapes_resting[0],shapes_resting_count,shape,tx,shape_data))
                {
                    shape->x+=tx;
                }

            }

            gravity = !gravity;
        }
    }


    for (size_t i = 0; i < snapshot_index; i++)
    {
        Snapshot *s1 = &snapshots[i];
        for (size_t j = i+1; j < snapshot_index; j++)
        {
            Snapshot *s2 = &snapshots[j];
            if(s1->instruction_index == s2->instruction_index)
            {
                if(s1->type == s2->type)
                {
                    for (size_t k = 0; k < 32; k++)
                    {
                        if(s1->delta[k] != s2->delta[k])
                        {
                            goto continue_loop;
                        }
                    }

                    unsigned long trillion = 1000000000000;

                    unsigned long height = 0;

                    int rocks_per_cycle = s2->id - s1->id;
                    int height_per_cycle = s2->highest - s1->highest;

                    unsigned long cycles = (trillion - s1->id) / rocks_per_cycle;

                    height += cycles * height_per_cycle;

                    int remainder = (trillion - s1->id) % rocks_per_cycle;

                    height += snapshots[i+remainder-1].highest;

                    printf("height %lu\n",height);

                    goto end_program;
                }
            }
            continue_loop:
        }
    }
    
    end_program:
}



int main()
{
    //part1();
    part2();
    return 0;
}