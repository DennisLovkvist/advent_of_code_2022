
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
void parse_line(char *line,Vector2i *sensor, Vector2i *beacon)
{
    int coords[4] = {0,0,0,0};
    int coord_index = 0;
    int index = 0;
    char c = line[index];
    int start = 0;
    while(1)
    {
        if(c == '=')
        {
            start = index +1;
        }
        else if(c == ',' || c == ':')
        {
            coords[coord_index] = parse_number(line,start,index);
            coord_index ++;
        }
        else if(c == '\0')
        {
            coords[coord_index] = parse_number(line,start,index);
            coord_index ++;
            goto end;
        }
        
        index++;
         c = line[index];

         
    }
    end:
    sensor->x = coords[0];
    sensor->y = coords[1];
    beacon->x = coords[2];
    beacon->y = coords[3];
}

int distance_manhattan(int x1, int y1, int x2, int y2)
{
   return abs(x2-x1)+abs(y2-y1);
}
void part1()
{
    char* input_raw = load_input("input.txt");
    int length = strlen(input_raw);

    int index = 0;
    int start = 0;
    int count = 0;

    int line_count = 0;

    while(index <= length)
    {
        char c = input_raw[index];
        if(c == '\n' || c == '\0')
        {
            line_count ++;
        }
        index++;
    }
    

    Vector2i *sensors = malloc(sizeof(Vector2i)*line_count);
    Vector2i *beacons = malloc(sizeof(Vector2i)*line_count);

    index = 0;
    line_count = 0;

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


                parse_line(line,&sensors[line_count],&beacons[line_count]);
                line_count++;
            }

            start = index +1;
        }
        
        index++;
    }

    Vector2i *covered_ranges = malloc(sizeof(Vector2i)*line_count);
    int covered_ranges_count = 0;

    
    for (size_t i = 0; i < line_count; i++)
    {
        Vector2i beacon = beacons[i];
        Vector2i sensor = sensors[i];


        int dist_sensor_to_beacon = distance_manhattan(sensor.x,sensor.y,beacon.x,beacon.y);
        int dist_sensor_to_y = distance_manhattan(sensor.x,sensor.y,sensor.x,2000000);

        if(dist_sensor_to_y < dist_sensor_to_beacon)
        {
            int n = dist_sensor_to_beacon - dist_sensor_to_y;
            int min = sensor.x - n;
            int max = sensor.x + n;

            unsigned int flag = 0;

            for (size_t j = 0; j < covered_ranges_count; j++)
            {
                if(covered_ranges[j].x <= min && covered_ranges[j].y >= max)
                {
                    flag = 1;
                    break;
                }
                else if(min <= covered_ranges[j].x && max >= covered_ranges[j].y)
                {
                    covered_ranges[j].x = min;
                    covered_ranges[j].y = max;
                    flag = 1;
                    break;
                }
                else
                {
                    if(min < covered_ranges[j].x && max < covered_ranges[j].y)
                    {
                        covered_ranges[j].x = min;
                        flag = 1;
                        break;
                    }
                    else if(max > covered_ranges[j].y && min > covered_ranges[j].x)
                    {
                        covered_ranges[j].y = max;
                        flag = 1;
                        break;
                    }
                }
            }

            if(!flag)
            {
                covered_ranges[covered_ranges_count].x = min;
                covered_ranges[covered_ranges_count].y = max;
                covered_ranges_count++;
            }
            
        }
        

        
    }
    for (size_t j = 0; j < covered_ranges_count; j++)
    {
        printf("%i\n",covered_ranges[j].y-covered_ranges[j].x);

    }
}
void part2()
{
    char* input_raw = load_input("input.txt");
    int length = strlen(input_raw);

    int index = 0;
    int start = 0;
    int count = 0;

    int line_count = 0;

    while(index <= length)
    {
        char c = input_raw[index];
        if(c == '\n' || c == '\0')
        {
            line_count ++;
        }
        index++;
    }
    

    Vector2i *sensors = malloc(sizeof(Vector2i)*line_count);
    Vector2i *beacons = malloc(sizeof(Vector2i)*line_count);

    index = 0;
    line_count = 0;

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


                parse_line(line,&sensors[line_count],&beacons[line_count]);
                line_count++;
            }

            start = index +1;
        }
        
        index++;
    }

    Vector2i *covered_range = malloc(sizeof(Vector2i)*line_count);
    
    #define FOUR_MILLION 4000000
    
    for (int  i = 0; i < FOUR_MILLION; i++)
    {
        int covered_ranges_count = 0;

        for (size_t j = 0; j < line_count; j++)
        {
            Vector2i beacon = beacons[j];
            Vector2i sensor = sensors[j];

            int dist_sensor_to_beacon = distance_manhattan(sensor.x,sensor.y,beacon.x,beacon.y);
            int dist_sensor_to_y = distance_manhattan(sensor.x,sensor.y,sensor.x,i);

            if(dist_sensor_to_y <= dist_sensor_to_beacon)
            {
                int n = dist_sensor_to_beacon - dist_sensor_to_y;
                int min = sensor.x - n;
                int max = sensor.x + n;
                
                covered_range[covered_ranges_count].x = min;
                covered_range[covered_ranges_count].y = max;
                covered_ranges_count++;
            }
        }

        for (int j = 0; j < FOUR_MILLION; j++)
        {
            unsigned int within_range = 0;
            for (int l = 0; l < covered_ranges_count; l++)
            {
                if(j >= covered_range[l].x && j <= covered_range[l].y)
                {
                    j = covered_range[l].y;
                    within_range = 1;
                    goto end;
                }
            }

            end:
            if(!within_range)
            {
                unsigned long x = (unsigned long)j;
                unsigned long y = (unsigned long)i;
                unsigned long four_million = FOUR_MILLION;
                unsigned long tuning_frequency = x * four_million + y;
                printf("tuning frequency: %lu\n",tuning_frequency);
                return;
            }
        }
    }
}
int main()
{
    part1();
    part2();
    return 1;
}
