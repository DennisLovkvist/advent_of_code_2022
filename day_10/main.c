
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#define CLOCK_CYCLE_NOOP 1
#define CLOCK_CYCLE_ADD 2

#define INSTRUCTION_NOOP 0
#define INSTRUCTION_ADD 1

#define WIDTH 40
#define HEIGHT 6



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
int eval_signal(int cycles_total,int reg)
{
    for (size_t i = 20; i <=220; i+=40)
    {
         if(cycles_total == i)
        {
            return reg*cycles_total;
        }
    }

    return 0;
}
void render(int cycles_total,int reg)
{
    if(cycles_total % 40 == 0)
    {
        printf("\n");
    }
    else
    {
        int x = cycles_total % 40;
        if(x == reg || x == reg+1 || x == reg+2)
        {
            printf("#");
        }
        else
        {
            printf(".");
        }
    }
}
void exec_noop(int *cycles_total,int *reg, int *signal)
{
    int cycles_op = 1;
    while(cycles_op > 0)
    {
        *cycles_total = *cycles_total +1;
        *signal = *signal + eval_signal(*cycles_total,*reg);
        render(*cycles_total,*reg);
        cycles_op--;
    }
}
void exec_add(int *cycles_total, int *reg, int value, int *signal)
{
    int cycles_op = CLOCK_CYCLE_ADD;
    while(cycles_op > 0)
    {
        *cycles_total = *cycles_total +1;
        *signal = *signal + eval_signal(*cycles_total,*reg);
        render(*cycles_total,*reg);
        cycles_op--;
    }
    *reg += value;

}
int main()
{
    char* input_raw = load_input("input.txt");
    int length = strlen(input_raw);
    int index = 0;
    int delimiter = 0;
    int start = 0;

    int reg  = 1;
    int cycles_total = 0;
    int signal = 0;

    while(index <length)
    {
        if(input_raw[index] == '\n' || input_raw[index] == '\0')
        {
            char *instruction;
            int value = 0;
            int inst = INSTRUCTION_NOOP;

            if(delimiter > start)
            {
                int len = delimiter-start;
                instruction = alloca(sizeof(char)*(len+1));
                memcpy(instruction, &input_raw[start],sizeof(char)*(len));
                instruction[len] = '\0';

                len = index-delimiter;
                char *v = alloca(sizeof(char)*(len+1));
                memcpy(v, &input_raw[delimiter],sizeof(char)*(len));
                v[len] = '\0';

                value = atoi(v);

                inst = INSTRUCTION_ADD;
            }
            else
            {
                int len = index-start;
                instruction = alloca(sizeof(char)*(len+1));
                memcpy(instruction, &input_raw[start],sizeof(char)*(len));
                instruction[len] = '\0';
                inst = INSTRUCTION_NOOP;
            }

            

            if(inst == INSTRUCTION_NOOP)
            {
                exec_noop(&cycles_total,&reg,&signal);
            } 
            else if(inst == INSTRUCTION_ADD)
            {
                exec_add(&cycles_total,&reg,value,&signal);
            } 


            start = index+1;
            delimiter = start;
        }
        else if(input_raw[index] == ' ')
        {
            delimiter = index;
        }

        index++;
    }
    
    
    printf("%i\n",signal);
    return 1;
}

