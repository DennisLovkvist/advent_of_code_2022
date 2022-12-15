
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

    int *trees = malloc(sizeof(int)*length);

    int stride = 0;
    int index = 0;
    for (size_t i = 0; i <= length; i++)
    {
        char c = input_raw[i];
        if(c == '\n' || c == '\0')
        {
            stride ++;
        }
        else
        {
            trees[index++] = input_raw[i]-48;//ascii shift
        }
    }

    int len = index;

    unsigned int visible = 0;
    int height = len/stride;
    printf("%i, %i\n",stride,height);
    for (size_t i = 0; i < len; i++)
    {
        int x = i % stride;
        int y = i / stride;
        int covered = 0;


        if(x == 0 || x == stride)
        {
            goto end;
        }
        else if(y == 0 || y == height)
        {
            goto end;
        }

        
        int tx = x+1;
        while(tx < stride)
        {
            int coord = y * stride + tx;
            if(trees[coord] >= trees[i])
            {
                covered = 1;
                break;
            }
            tx ++;
        }
        if(!covered)goto end;
        covered = 0;
        tx = x-1;
        while(tx >= 0)
        {
            int coord = y * stride + tx;
            if(trees[coord] >= trees[i])
            {
                covered = 2;
                break;
            }
            tx --;
        }
        if(!covered)goto end;
        covered = 0;
        int ty = y+1;
        while(ty < height)
        {
            int coord = ty * stride + x;
            if(trees[coord] >= trees[i])
            {
                covered = 3;
                break;
            }
            ty ++;
        }
        if(!covered)goto end;
        covered = 0;
        ty = y-1;
        while(ty >= 0)
        {
            int coord = ty * stride + x;
            if(trees[coord] >= trees[i])
            {
                covered = 4;
                break;
            }
            ty --;
        }
        end:
               
        if(!covered)
        {
            visible++;
        }
    }
    printf("visible: %i\n",visible);
}
void part2()
{
    char* input_raw = load_input("input.txt");
    int length = strlen(input_raw);

    int *trees = malloc(sizeof(int)*length);

    int stride = 0;
    int index = 0;
    for (size_t i = 0; i <= length; i++)
    {
        char c = input_raw[i];
        if(c == '\n' || c == '\0')
        {
            stride ++;
        }
        else
        {
            trees[index++] = input_raw[i]-48;//ascii shift
        }
    }

    int len = index;

    unsigned int visible = 0;
    int height = len/stride;

    int hightest_score = 0;

    for (size_t i = 0; i < len; i++)
    {
        int x = i % stride;
        int y = i / stride;

        int view_distance[4] = {0,0,0,0};
        int tx = x+1;
        int score = 0;
        
        while(tx < stride)
        {
            int coord = y * stride + tx;
            view_distance[0]++;
            if(trees[coord] >= trees[i])
            {
                break;
            }
            tx ++;
        }
        tx = x-1;
        while(tx >= 0)
        {
            int coord = y * stride + tx;
            view_distance[1]++;
            if(trees[coord] >= trees[i])
            {
                break;
            }
            tx --;
        }
        int ty = y+1;
        while(ty < height)
        {
            int coord = ty * stride + x;
            view_distance[2]++;
            if(trees[coord] >= trees[i])
            {
                break;
            }
            ty++;
        }
        ty = y-1;
        while(ty >= 0)
        {
            int coord = ty * stride + x;
            view_distance[3]++;
            if(trees[coord] >= trees[i])
            {
                break;
            }
            ty --;
        }

        score = view_distance[0]*view_distance[1]*view_distance[2]*view_distance[3];

               printf("%i,%i,%i,%i\n",view_distance[0],view_distance[1],view_distance[2],view_distance[3]);
        if(hightest_score < score)
        {
            hightest_score = score;
        }
    }
    printf("highest view distance: %i\n",hightest_score);
}
int main()
{

    //part1();
    part2();
    return 1;
}

