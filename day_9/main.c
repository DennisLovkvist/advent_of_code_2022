
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <unistd.h>

#define TWO_SQUARED 1.41

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
void part1()
{
    char* input_raw = load_input("input.txt");
    int length = strlen(input_raw);

    Vector2i *visited = malloc(sizeof(Vector2i)*100000);
    visited[0].x = 0;
    visited[0].y = 0;
    int visit_count = 1;

        Vector2i head = {0,0};
        Vector2i tail = {0,0};
        int index = 0;

        int start = 0;
    while(index <= length)
    {
        if(input_raw[index] == '\n'|| input_raw[index] == '\0')
        {


            start = index+1;
        char direction = input_raw[i];
        int magnitude = input_raw[i+2]-48;//ascii shift


        printf("%c -> %i\n", direction,magnitude);

        for (size_t j = 0; j < magnitude; j++)
        {
            int prev_x = head.x;
            int prev_y = head.y;

            if(direction == 'R')
            {
                head.x ++;
            }
            if(direction == 'L')
            {
                head.x --;
            }
            if(direction == 'U')
            {
                head.y ++;
            }
            if(direction == 'D')
            {
                head.y --;
            }

            int dx = head.x - tail.x;
            int dy = head.y - tail.y;

            
            if(abs(dx) > 1)
            {
                tail.x += (head.x > tail.x) ? 1:-1;
                tail.y = prev_y;
            }
            if(abs(dy) > 1)
            {
                tail.y += (head.y > tail.y) ? 1:-1;
                tail.x = prev_x;
            }

            
            unsigned int allready_visited = 0;
            for (size_t k = 0; k < visit_count; k++)
            {
                if(visited[k].x == tail.x && visited[k].y == tail.y)
                {
                    allready_visited = 1;
                    break;
                }
            }
            if(!allready_visited)
            {
                visited[visit_count].x = tail.x;
                visited[visit_count].y = tail.y;
                visit_count++;
            }
index++;
/*
            for (int j = 10; j > -1; j--)
            {
                for (int i = -1; i < 10; i++)
                {
                    if(head.x == i && head.y == j)
                    {
                        printf("H");
                    }
                    else if(tail.x == i && tail.y == j)
                    {
                        printf("T");
                    }
                    else
                    {
                        printf(".");
                    }
                }
                printf("\n");
            }
            
*/

            //sleep(1);
                
            
        }

    }
    }


        printf("%i\n",visit_count);
    
    
}
int main()
{

    part1();
    //part2();
    return 1;
}

