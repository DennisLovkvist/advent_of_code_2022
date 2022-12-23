
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
#define Y 2000000
//#define Y 10
int main()
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

    Vector2i *ranges = malloc(sizeof(Vector2i)*line_count);
    int ranges_index = 0;

    
    for (size_t i = 0; i < line_count; i++)
    {
        Vector2i b = beacons[i];
        Vector2i s = sensors[i];


        int dist_s_b = distance_manhattan(s.x,s.y,b.x,b.y);
        int dist_s_y = distance_manhattan(s.x,s.y,s.x,Y);

        if(dist_s_y < dist_s_b)
        {
            int lol = dist_s_b-dist_s_y;

            
            int min = s.x - lol;
        
            int max = s.x+lol;

            unsigned int flag = 0;

            for (size_t j = 0; j < ranges_index; j++)
            {
                if(ranges[j].x <= min && ranges[j].y >= max)
                {
                    flag = 1;
                    break;
                }
                else if(min <= ranges[j].x && max >= ranges[j].y)
                {
                    ranges[j].x = min;
                    ranges[j].y = max;
                    flag = 1;
                    break;
                }
                else
                {
                    if(min < ranges[j].x && max < ranges[j].y)
                    {
                        ranges[j].x = min;
                        flag = 1;
                        break;
                    }
                    else if(max > ranges[j].y && min > ranges[j].x)
                    {
                        ranges[j].y = max;
                        flag = 1;
                        break;
                    }
                }
            }

            if(!flag)
            {
                ranges[ranges_index].x = min;
                ranges[ranges_index].y = max;
                ranges_index++;
            }
            
        }
        

        
    }
  
  for (size_t j = 0; j < ranges_index; j++)
    {
        printf("%i\n",ranges[j].y-ranges[j].x);

    }

    



    
    
    

    return 1;
}

