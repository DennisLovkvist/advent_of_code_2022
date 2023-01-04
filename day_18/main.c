
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


typedef struct Point Point;
struct Point
{
    Vector3i position;
    unsigned int surfaces[27];
    unsigned int status;
    unsigned int subset;
    unsigned int reachable;
};

#define SUBSET_VOID 0
#define SUBSET_SOLID 1
#define SUBSET_VOID_CONTAINED 2

#define STATUS_UNVISITED 0
#define STATUS_VISITED 1
#define STATUS_QUEUED 2

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
void parse_line(char *line, Point *point)
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

    point->position.x = vector[0];
    point->position.y = vector[1];
    point->position.z = vector[2];
    
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
    
    
    Point *cubes = malloc(sizeof(Point)*cube_count);

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
        Point *cube = &cubes[i];
        for (size_t k = 0; k < KERNEL_SIZE; k++)
        {
            cube->surfaces[k] = 0;
        }
        
    }
    for (size_t i = 0; i < cube_count; i++)
    {
        Point *cube = &cubes[i];
        for (size_t j = 0; j < cube_count; j++)
        {
            for (size_t k = 0; k < KERNEL_SIZE; k++)
            {
                Point *cube2 = &cubes[j];
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
        Point *cube = &cubes[i];
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
int surface_area(Point *manifold, int volume,Vector3i kernel[KERNEL_SIZE])
{
    for (size_t i = 0; i < volume; i++)
    {
        Point *point = &manifold[i];
        for (size_t k = 0; k < KERNEL_SIZE; k++)
        {
            point->surfaces[k] = 0;
        }
        
    }
    for (size_t i = 0; i < volume; i++)
    {
        Point *point = &manifold[i];
        for (size_t j = 0; j < volume; j++)
        {
            for (size_t k = 0; k < KERNEL_SIZE; k++)
            {
                Point *point2 = &manifold[j];
                Vector3i neightbour = 
                {
                    point->position.x + kernel[k].x,
                    point->position.y + kernel[k].y,
                    point->position.z + kernel[k].z
                };
                //if cmp_vector3i return 1 surface at k is blocked
                if(cmp_vector3i(neightbour,point2->position))
                {
                    point->surfaces[k] = 1;
                }
            }
            
        }
    }
    int none_connected_surfaces = 0;
    for (size_t i = 0; i < volume; i++)
    {
        Point *point = &manifold[i];
        for (size_t k = 0; k < KERNEL_SIZE; k++)
        {
            if(!point->surfaces[k])
            {
                none_connected_surfaces++;
            }
        }
    }

    return none_connected_surfaces;
}
int index_from_xyz(Vector3i coord,Vector3i min,Vector3i max)
{
    int WIDTH = abs(max.x - min.x);
    int HEIGHT = abs(max.y - min.y);
    int DEPTH = abs(max.z - min.z);

    return coord.x + coord.y*WIDTH + coord.z*WIDTH*HEIGHT;
}
#define UNVISITED 0
#define VISITED_EMPTY 1
#define VISITED_FILLED 2


void part2()
{
    char *input_raw = load_input("input.txt");
    int length = strlen(input_raw);

    int index = 0;
    int manifold_subset_lava_volume = 0;
    while (index <= length)
    {
        char c = input_raw[index];
        if (c == '\n' || c == '\0')
        {
           manifold_subset_lava_volume ++;
        }
        index++;
    }
    
    
    Point *manifold_subset_lava = malloc(sizeof(Point)*manifold_subset_lava_volume);

    index = 0;
    manifold_subset_lava_volume = 0;
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

                parse_line(line, &manifold_subset_lava[manifold_subset_lava_volume]);
                
                manifold_subset_lava_volume++;
            }

            start = index + 1;
        }

        index++;
    }

    Vector3i kernel[KERNEL_SIZE] = 
    {
        
        {0,0,-1},{-1,0,0},{1,0,0},{0,-1,0},{0,1,0},{0,0,1}
        
    };

    for (size_t i = 0; i < manifold_subset_lava_volume; i++)
    {
        Point *point = &manifold_subset_lava[i];
        for (size_t k = 0; k < KERNEL_SIZE; k++)
        {
            point->surfaces[k] = 0;
        }
        
    }
    Vector3i min = {1000,1000,1000};
    Vector3i max = {0,0,0};

    for (size_t i = 0; i < manifold_subset_lava_volume; i++)
    {
        Point *point = &manifold_subset_lava[i];
        max.x = max.x < point->position.x ? point->position.x:max.x;
        max.y = max.y < point->position.y ? point->position.y:max.y;
        max.z = max.z < point->position.z ? point->position.z:max.z;

    }

    for (size_t i = 0; i < manifold_subset_lava_volume; i++)
    {
        Point *point = &manifold_subset_lava[i];
        point->position.x +=1;
        point->position.y +=1;
        point->position.z +=1;
    }


    min.x = 0;
    min.y = 0;
    min.z = 0;

    max.x +=4;
    max.y +=4;
    max.z +=4;

    int manifold_volume = (max.x * max.y * max.z);


    Point *manifold = malloc(sizeof(Point)*manifold_volume);

    for (size_t i = 0; i < manifold_volume; i++)
    {
        Point *point = &manifold[i];
        point->reachable = 0;
        point->status = STATUS_UNVISITED;
    }
    
    for (size_t x = 0; x < max.x; x++)
    {
        for (size_t y = 0; y < max.y; y++)
        {
            for (size_t z = 0; z < max.z; z++)
            {
                
                Vector3i coord = {x,y,z};
                int index = index_from_xyz(coord,min,max);
                Point *point = &manifold[index];
                point->position.x = coord.x;
                point->position.y = coord.y;
                point->position.z = coord.z;

                point->subset = SUBSET_VOID;//Default

                for (size_t i = 0; i < manifold_subset_lava_volume; i++)
                {
                    Point *point_solid = &manifold_subset_lava[i];
                    if(cmp_vector3i(point->position,point_solid->position))
                    {
                        point->subset = SUBSET_SOLID;
                        break;
                    }
                }
            }
        }
    }

    int *q = malloc(sizeof(int)*manifold_volume);
    int q_size = 0;

    q[q_size] = 0;
    q_size++;
    int manifold_subset_gas_volume = 0;
    while(q_size != 0)
    {
        Point *point = &manifold[q[0]];

        point->status = STATUS_VISITED;
        manifold_subset_gas_volume++;

        point->reachable = 1;
        
        for (size_t i = 0; i+1 < q_size; i++)
        {
            q[i] = q[i+1];
        }
        q_size --;
        for (size_t k = 0; k < KERNEL_SIZE; k++)
        {
            Vector3i adjacent_position = 
            {
                point->position.x + kernel[k].x,
                point->position.y + kernel[k].y,
                point->position.z + kernel[k].z
            };

            unsigned int within_bounds_x = adjacent_position.x >= 0 && adjacent_position.x <= max.x;
            unsigned int within_bounds_y = adjacent_position.y >= 0 && adjacent_position.y <= max.y;
            unsigned int within_bounds_z = adjacent_position.z >= 0 && adjacent_position.z <= max.z;

            if(within_bounds_x * within_bounds_y * within_bounds_z != 0)
            {
                int adjacent_index = index_from_xyz(adjacent_position,min,max);
                if(adjacent_index >= 0 && adjacent_index < manifold_volume)
                {
                    Point *adjacent_point = &manifold[adjacent_index];

                    if(adjacent_point->status == STATUS_UNVISITED)
                    {
                        for (size_t i = 0; i < manifold_subset_lava_volume; i++)
                        {
                            Point *point_lava_droplet = &manifold_subset_lava[i];
                            if(cmp_vector3i(point_lava_droplet->position,adjacent_point->position))
                            {
                                goto skip;
                            }
                        }
                        adjacent_point->status = STATUS_QUEUED;
                        q[q_size] = adjacent_index;
                        q_size++;
                        skip:
                    }
                }
            }
        }
    }

    Point *manifold_subset_gas = malloc(sizeof(Point)*manifold_subset_gas_volume);
    int n = 0;
    for (size_t i = 0; i < manifold_volume; i++)
    {
        if(manifold[i].reachable)
        {
            manifold_subset_gas[n].position.x = manifold[i].position.x;
            manifold_subset_gas[n].position.y = manifold[i].position.y;
            manifold_subset_gas[n].position.z = manifold[i].position.z;
            n++;
        }
    }

    int surface_area_lava = surface_area(manifold_subset_lava,manifold_subset_lava_volume,kernel);
    int surface_area_gas = surface_area(manifold_subset_gas,manifold_subset_gas_volume,kernel);

    int surface_area_manifold = (max.x*max.y+max.y*max.z+max.z*max.x)*2;

    printf("%i\n",surface_area_gas-surface_area_manifold);

}
int main()
{
    //part1();
    part2();
    return 0;
}