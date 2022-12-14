
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

    int input_width = 9;
    int input_height = 8;
    int stack_limit = input_width * input_height;
    int stacks_length = input_width*stack_limit;

    char* input_raw = load_input("input.txt");
    int length = strlen(input_raw);

    
    int index = 0,sum = 0,count = 0,empty = 0,carret = 0;

    //pars input
    int x = 0;
    int y = input_height-1;

    char stacks[stacks_length];
    for (size_t i = 0; i < stacks_length; i++)
    {
        stacks[i] = '_';
    }
    int *heights = alloca(sizeof(int)*input_width);
    for (size_t i = 0; i < input_width; i++)
    {
        heights[i] = 0;
    }
    for (size_t i = 0; i < length; i+=4)
    {
        for (size_t j = i; j < i+4; j++)
        {
            char c = input_raw[j];

            if(c >= 65 && c <= 90)
            {
                stacks[(x*stack_limit) + y] = c;
                heights[x]++;
                break;
            }
        }
        x++;
        if(x > input_width-1)
        {
            x = 0;
            y --;
        }
    }
    
    //debug print
    for (int Y = stack_limit-1; Y >= 0; Y--)
    {
        for (int X = 0; X < input_width; X++)
        {
            int coord = (X*stack_limit) + Y;
            printf("%c",stacks[coord]);
        }

        printf("\n");
    }

    //jump to instructions
    while(input_raw[carret] != 'm' && input_raw[carret] != '\0')
    {
        carret ++;
    }

    //parse and execute instructions
    int operation_position = 0;
    int operation_values[3] = {0,0,0};
    char digits[3] = {'\0','\0','\0'};
    while (carret <= length)
    {
        char c = input_raw[carret];

        if(c >= 48 && c <= 57)//ascii upper case letters range
        {
            digits[count++] = c;
        }
        else
        {
            if(count > 0)
            {
                operation_values[operation_position++] = atoi(digits);
                digits[0] = digits[1] = digits[2] = '\0';
                count = 0;
            }
        }
        if(operation_position > 2)
        {
            //printf("move %i from %i to %i\n", operation_values[0],operation_values[1],operation_values[2]);
            count = 0;
            //loops n times whrer n = quantity of boxes that are instructed to be moved
            for (size_t i = 0; i < operation_values[0]; i++)
            {
                //finds and gets the source
                int x = operation_values[1]-1;
                int y = heights[x]-1;
                int coord = (x*stack_limit) + y;
                char target = stacks[coord];
                stacks[coord] = '_';
                heights[x] --;

                //modifies the destination
                x = operation_values[2]-1;
                y = heights[x];
                coord = (x*stack_limit) + y;
                stacks[coord] = target;
                heights[x]++;
            }

            operation_position = 0;
        }
        carret ++;
    }
    //debug print
    for (int Y = stack_limit-1; Y >= 0; Y--)
    {
        for (int X = 0; X < input_width; X++)
        {
            int coord = (X*stack_limit) + Y;
            printf("%c",stacks[coord]);
        }
        printf("\n");
    }

    //print result
    printf("%s","upper most crates: ");
    for (size_t i = 0; i < input_width; i++)
    {
        int x = i;
        int y = heights[x]-1;
        int coord = (x*stack_limit) + y;
        printf("%c",stacks[coord]);
    }

}
void part2()
{

    int input_width = 9;
    int input_height = 8;
    int stack_limit = input_width * input_height;
    int stacks_length = input_width*stack_limit;

    char* input_raw = load_input("input.txt");
    int length = strlen(input_raw);

    
    int index = 0,sum = 0,count = 0,empty = 0,carret = 0;

    //pars input
    int x = 0;
    int y = input_height-1;

    char stacks[stacks_length];
    for (size_t i = 0; i < stacks_length; i++)
    {
        stacks[i] = '_';
    }
    int *heights = alloca(sizeof(int)*input_width);
    for (size_t i = 0; i < input_width; i++)
    {
        heights[i] = 0;
    }
    for (size_t i = 0; i < length; i+=4)
    {
        for (size_t j = i; j < i+4; j++)
        {
            char c = input_raw[j];

            if(c >= 65 && c <= 90)
            {
                stacks[(x*stack_limit) + y] = c;
                heights[x]++;
                break;
            }
        }
        x++;
        if(x > input_width-1)
        {
            x = 0;
            y --;
        }
    }
    
    //debug print
    for (int Y = stack_limit-1; Y >= 0; Y--)
    {
        for (int X = 0; X < input_width; X++)
        {
            int coord = (X*stack_limit) + Y;
            printf("%c",stacks[coord]);
        }

        printf("\n");
    }

    //jump to instructions
    while(input_raw[carret] != 'm' && input_raw[carret] != '\0')
    {
        carret ++;
    }

    //parse and execute instructions
    int operation_position = 0;
    int operation_values[3] = {0,0,0};
    char digits[3] = {'\0','\0','\0'};
    while (carret <= length)
    {
        char c = input_raw[carret];

        if(c >= 48 && c <= 57)//ascii upper case letters range
        {
            digits[count++] = c;
        }
        else
        {
            if(count > 0)
            {
                operation_values[operation_position++] = atoi(digits);
                digits[0] = digits[1] = digits[2] = '\0';
                count = 0;
            }
        }
        if(operation_position > 2)
        {
            //printf("move %i from %i to %i\n", operation_values[0],operation_values[1],operation_values[2]);
            count = 0;
            //loops n times whrer n = quantity of boxes that are instructed to be moved
            int m = operation_values[0];
            for (size_t i = 0; i < operation_values[0]; i++)
            {
                //finds and gets the source
                //different from part 2, here we just start moving the lowest crate first and adjut the total stack height after the operations
                int x = operation_values[1]-1;
                int y = heights[x]-m;
                m--;
                int coord = (x*stack_limit) + y;
                char target = stacks[coord];
                stacks[coord] = '_';
                

                //modifies the destination
                x = operation_values[2]-1;
                y = heights[x];
                coord = (x*stack_limit) + y;
                stacks[coord] = target;
                heights[x]++;
            }

            //cut height
            int x = operation_values[1]-1;
            heights[x] -=operation_values[0];
            

            operation_position = 0;
        }
        carret ++;
    }
    //debug print
    for (int Y = stack_limit-1; Y >= 0; Y--)
    {
        for (int X = 0; X < input_width; X++)
        {
            int coord = (X*stack_limit) + Y;
            printf("%c",stacks[coord]);
        }
        printf("\n");
    }

    //print result
    printf("%s","upper most crates: ");
    for (size_t i = 0; i < input_width; i++)
    {
        int x = i;
        int y = heights[x]-1;
        int coord = (x*stack_limit) + y;
        printf("%c",stacks[coord]);
    }

}
int main()
{
    //part1();
    part2();
    return 1;
}

