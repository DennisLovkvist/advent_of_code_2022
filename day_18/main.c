
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct Vector3i Vector3i;
struct Vector3i
{
    int x,y,z;
};


typedef struct Cube Cube;
struct Cube
{
    Vector3i position;
    unsigned int surfaces[27];
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
void parse_line(char *line, Cube *cube)
{
    
    int vector[3] = {0,0,0};
    int element = 0;
    int index = 0;
    char c = line[index];
    int start = 0;
    while (c != '\0')
    {
        if(c == ',')
        {
            vector[element] = parse_number(line,start,index);
            element++;

            start = index+1;
        }
        index++;
        c = line[index];
    }

    vector[element] = parse_number(line,start,index);

    cube->position.x = vector[0];
    cube->position.y = vector[1];
    cube->position.z = vector[2];
    
}
unsigned int cmp_vector3i(Vector3i vec1, Vector3i vec2)
{
    return (vec1.x == vec2.x && vec1.y == vec2.y && vec1.z == vec2.z) ? 1:0;
}
#define KERNEL_SIZE 6
void part1()
{
    char *input_raw = load_input("input(example).txt");
    int length = strlen(input_raw);

    int index = 0;
    int cube_count = 0;
    while (index <= length)
    {
        char c = input_raw[index];
        if (c == '\n' || c == '\0')
        {
           cube_count ++;
        }
        index++;
    }
    
    
    Cube *cubes = malloc(sizeof(Cube)*cube_count);

    index = 0;
    cube_count = 0;
    int start = 0;

    while (index <= length)
    {
        char c = input_raw[index];
        if (c == '\n' || c == '\0')
        {
            int len = index - start;
            if (len > 0)
            {
                char *line = alloca(sizeof(char) * (len + 1));
                memcpy(line, &input_raw[start], sizeof(char) * len);
                line[len] = '\0';

                parse_line(line, &cubes[cube_count]);
                
                cube_count++;
            }

            start = index + 1;
        }

        index++;
    }

    Vector3i kernel[KERNEL_SIZE] = 
    {
        
        {0,0,-1},{-1,0,0},{1,0,0},{0,-1,0},{0,1,0},{0,0,1}
        
    };

    for (size_t i = 0; i < cube_count; i++)
    {
        Cube *cube = &cubes[i];
        for (size_t k = 0; k < KERNEL_SIZE; k++)
        {
            cube->surfaces[k] = 0;
        }
        
    }
    for (size_t i = 0; i < cube_count; i++)
    {
        Cube *cube = &cubes[i];
        for (size_t j = 0; j < cube_count; j++)
        {
            for (size_t k = 0; k < KERNEL_SIZE; k++)
            {
                Cube *cube2 = &cubes[j];
                Vector3i neightbour = 
                {
                    cube->position.x + kernel[k].x,
                    cube->position.y + kernel[k].y,
                    cube->position.z + kernel[k].z
                };
                //if cmp_vector3i return 1 surface at k is blocked
                if(cmp_vector3i(neightbour,cube2->position))
                {
                    cube->surfaces[k] = 1;
                }
            }
            
        }
    }
    int none_connected_surfaces = 0;
    for (size_t i = 0; i < cube_count; i++)
    {
        Cube *cube = &cubes[i];
        for (size_t k = 0; k < KERNEL_SIZE; k++)
        {
            if(!cube->surfaces[k])
            {
                none_connected_surfaces++;
            }
        }
    }

    printf("%i\n", none_connected_surfaces);
    
}

int main()
{
    part1();
    return 0;
}