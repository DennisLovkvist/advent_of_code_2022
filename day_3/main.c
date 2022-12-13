
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
    char* input_raw = load_input("input_example.txt");
    int length = strlen(input_raw);

    int count = 0;
    int start = 0;
    int sum = 0;
    for (size_t i = 0; i < length+1; i++)
    {
        if(input_raw[i] == '\n' || i == length)
        {
            //alloc on stack
            char *line = alloca(sizeof(char)*count+1);
            memcpy(line,&input_raw[start],sizeof(char)*count);
            line[count] = '\0';

            int middle = count/2;
            for (size_t j = 0; j < middle; j++)
            {
                for (size_t k = middle; k < count; k++)
                {
                    if(line[j] == line[k])
                    {
                        int c = line[j];
                        if(c > 96 && c < 123)
                        {
                            c-=96;
                        }
                        else
                        {
                            c-=38;
                        }
                        sum += c;
                        goto end_loop;
                    }
                }
            }
            end_loop:
            start += count+1;
            count = 0;
        }
        else
        {
            count ++;
        }
    }

    printf("%s%i","sum of the priorities: ", sum);
}
void part2()
{
    char* input_raw = load_input("input_part_2.txt");
    int length = strlen(input_raw);

    int count = 0;
    int start = 0;
    int sum = 0;
    int index = 0;

    char *lines[3];
    int lengths[3] = {0,0,0};

    for (size_t i = 0; i < length+1; i++)
    {
        if(input_raw[i] == '\n' || i == length)
        {
            lines[index] = alloca(sizeof(char)*count+1);
            memcpy(lines[index],&input_raw[start],sizeof(char)*count);
            lines[index][count] = '\0';
            lengths[index] = count;

            if(index == 2)
            {
                for (size_t i = 0; i < lengths[0]; i++)
                {
                    int found = 0;
                    for (size_t j = 0; j < lengths[1]; j++)
                    {
                        if(lines[0][i] == lines[1][j])
                        {
                            for (size_t k = 0; k < lengths[2]; k++)
                            {
                                if(lines[1][j] == lines[2][k])
                                {
                                    int c = lines[1][j];
                                    if(c > 96 && c < 123)
                                    {
                                        c-=96;
                                    }
                                    else
                                    {
                                        c-=38;
                                    }
                                    sum += c; 
                                    index = 0;
                                    goto next_group;
                                }
                            }
                        }
                    }
                }    
            }
            index++;

            next_group:
            start += count+1;
            count = 0;
        }
        else
        {
            count++;
        }
    }
     
    printf("sum of the priorities: %i\n",sum);
    
}
int main()
{
    //part1();
    part2();
    return 1;
}