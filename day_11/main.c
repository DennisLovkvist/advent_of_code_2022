
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

typedef struct Monkey Monkey;
struct Monkey
{
    int index;
    int items[32];
    int item_count;
    int worry_operator, worry_operand,worry_operand_is_number;
    int test_operand;
    int monkey_index_0;
    int monkey_index_1;
    int number_of_inspections;
};

#define PARSING_HEADER 0
#define PARSING_ITEMS 1
#define PARSING_WORRY 2
#define PARSING_TEST 3
#define PARSING_TEST_TRUE 4
#define PARSING_TEST_FALSE 5

#define OPERATOR_ADD 0
#define OPERATOR_MUL 1


void parse_header(char*line,int length, Monkey *monkey)
{
    int carret = 0;
    int start = 0,end = 0;
    while (carret < length)
    {
        if(line[carret] == ' ')
        {
            start = carret+1;
        }
        else if(line[carret] == ':')
        {
            end = carret;
            break;
        }
        carret ++;
    }

    int len = end-start;
    char *digits = alloca(sizeof(char)*(len+1)); 
    memcpy(digits,&line[start],sizeof(char)*(len));
    digits[len] = '\0';

    monkey->index = atoi(digits);

}
void to_arrayi(char*line,int length, char delimiter,int array[32], int *array_size)
{
    *array_size = 0;
    int delimiters = 0;
    for (size_t i = 0; i < length; i++)
    {
        if(line[i] == delimiter)
        {
            delimiters ++;
        }
    }
    int start = 1, end = 0;
    for (size_t i = 0; i <= length; i++)
    {
        if(line[i] == delimiter || line[i] == '\0')
        {
            end = i;

            int len = end - start;
            char *digit = alloca(sizeof(char)*(len+1)); 
            memcpy(digit,&line[start],sizeof(char)*(len));
            digit[len] = '\0';
            array[*array_size] = atoi(digit);
            *array_size = *array_size +1;
            start = i+1;
        }
    }
}
void parse_items(char*line,int length, Monkey *monkey)
{
    int carret = 0;
    while (carret < length)
    {
        if(line[carret] == ':')
        {
            int len = length-carret;
            char *number_sequence = alloca(sizeof(char)*(len+1)); 
            memcpy(number_sequence,&line[carret],sizeof(char)*(len));
            number_sequence[len] = '\0';

             to_arrayi(number_sequence,len,',', monkey->items, &monkey->item_count);
        }
        carret++;
    }

}
void parse_worry(char*line,int length, Monkey *monkey)
{
    int carret = 0;
    while (carret < length)
    {
        if(line[carret] == '*')
        {
            carret +=2;
            monkey->worry_operator = OPERATOR_MUL;
            break;
        }
        else if(line[carret] == '+')
        {
            monkey->worry_operator = OPERATOR_ADD;
            carret +=2;
            break;
        }
        carret ++;
    }

    if(line[carret] == 'o')
    {
        monkey->worry_operand_is_number = 0;
    }
    else
    {
        monkey->worry_operand_is_number = 1;

        int len = length-carret;
        char *digits = alloca(sizeof(char)*(len+1)); 
        memcpy(digits,&line[carret],sizeof(char)*(len));
        digits[len] = '\0';
        monkey->worry_operand = atoi(digits);
    }

}
int parse_test(char*line,int length, Monkey *monkey)
{
    int carret = length-1;
    while (carret >= 0)
    {
        if(line[carret] == ' ')
        {
            carret++;
            break;
        }
        carret --;
    }
    int len = length-carret;
    char *digits = alloca(sizeof(char)*(len+1)); 
    memcpy(digits,&line[carret],sizeof(char)*(len));
    digits[len] = '\0';
    return atoi(digits);
}
void print_monkey(Monkey *monkey)
{
    printf("Monkey %i:\n", monkey->index);
    printf("    Starting items: ", monkey->index);
    for (size_t i = 0; i < monkey->item_count; i++)
    {
        if(i == monkey->item_count-1)
        {
            printf("%i", monkey->items[i]);
        }
        else
        {
            printf("%i, ", monkey->items[i]);
        }
    }
    printf("\n");
    if(monkey->worry_operand_is_number)
    {
        char operator = (monkey->worry_operator == OPERATOR_ADD)?'+':'*';
        printf("    Operation: new = old %c %i:\n", operator, monkey->worry_operand);
    }
    else
    {
        char operator = (monkey->worry_operator == OPERATOR_ADD)?'+':'*';
        printf("    Operation: new = old %c %s:\n", operator, "old");
    }
    printf("    Test: divisible by %i:\n", monkey->test_operand);
    printf("        If true: throw to monkey %i:\n", monkey->monkey_index_1);
    printf("        If false: throw to monkey %i:\n", monkey->monkey_index_0);
    
}
Monkey *get_monkey_by_index(Monkey monkey_pool[10],int monkey_count, int index)
{
    for (size_t i = 0; i < monkey_count; i++)
    {
        if(monkey_pool[i].index == index)
        {
            return &monkey_pool[i];
        }
    }

    return NULL;
    
}
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
int main()
{
    char* input_raw = load_input("input(example).txt");
    int length = strlen(input_raw);
    
    int parsing_stage = PARSING_HEADER;

    int index = 0;
    int start = 0;

    Monkey monkey_pool[10];
    int monkey_count = 0;

    while (index <= length)
    {
        if(input_raw[index] == '\n'|| input_raw[index] == '\0')
        {
            int len = index - start;
            char *line = alloca(sizeof(char)*(len+1)); 
            memcpy(line,&input_raw[start],sizeof(char)*(len));
            line[len] = '\0';

            Monkey *monkey = &monkey_pool[monkey_count];

            if(parsing_stage == PARSING_HEADER)
            {
                parse_header(line,len,monkey);
                parsing_stage = PARSING_ITEMS;
            }
            else if(parsing_stage == PARSING_ITEMS)
            {
                parse_items(line,len,monkey);
                parsing_stage = PARSING_WORRY;
            }
            else if(parsing_stage == PARSING_WORRY)
            {
                parse_worry(line,len,monkey);
                parsing_stage = PARSING_TEST;
            }
            else if(parsing_stage == PARSING_TEST)
            {
                monkey->test_operand = parse_test(line,len,monkey);
                parsing_stage = PARSING_TEST_TRUE;
            }
            else if(parsing_stage == PARSING_TEST_TRUE)
            {
                monkey->monkey_index_1 = parse_test(line,len,monkey);
                parsing_stage = PARSING_TEST_FALSE;
            }
            else if(parsing_stage == PARSING_TEST_FALSE)
            {
                monkey->monkey_index_0 = parse_test(line,len,monkey);
                parsing_stage = PARSING_HEADER;
                monkey_count ++;
                index++;
            }
            start = index+1;
        }


        index++;
    }

    for (size_t i = 0; i < monkey_count; i++)
    {
       monkey_pool[i].number_of_inspections = 0;
    }
    
    #define DEBUG 0
    //print_monkey(&monkey_pool[0]);

    int round = 0;
    for (size_t i = 0; i < 20; i++)
    {
        for (size_t j = 0; j < monkey_count; j++)
        {
        
            Monkey *monkey = &monkey_pool[j];
            int item_count = monkey->item_count;
            for (size_t k = 0; k < monkey->item_count; k++)
            {
                int *item = &monkey->items[k];
                //Inspection
                monkey->number_of_inspections++;
                if(DEBUG)printf("Monkey %i inspects an item with a worry level of %i.\n",monkey->index,*item);
                if(monkey->worry_operator == OPERATOR_ADD)
                {
                    if(monkey->worry_operand_is_number)
                    {
                        *item += monkey->worry_operand;
                        if(DEBUG)printf("    Worry level increases by %i to %i.\n",monkey->worry_operand,*item);
                    }
                    else
                    {
                        *item += *item;
                        if(DEBUG)printf("    Worry level increases by %i to %i.\n",*item,*item);
                    }
                }
                else if(monkey->worry_operator == OPERATOR_MUL)
                {
                    if(monkey->worry_operand_is_number)
                    {
                        *item *= monkey->worry_operand;
                        if(DEBUG)printf("    Worry level is multiplied by %i to %i.\n",monkey->worry_operand,*item);
                    }
                    else
                    {
                        *item *= *item;
                        if(DEBUG)printf("    Worry level is multiplied by %i to %i.\n",*item,*item);
                    }
                }
                *item /= 3;
                if(DEBUG)printf("    Monkey gets bored with item. Worry level is divided by 3 to %i.\n", *item);
                //Test

                int monkey_index;

                if(*item % monkey->test_operand == 0)
                {
                    if(DEBUG)printf("    Current worry level is not divisible by %i.\n",monkey->test_operand);
                    monkey_index = monkey->monkey_index_1;
                }
                else
                {
                    if(DEBUG)printf("    Current worry level is divisible by %i.\n",monkey->test_operand);
                    monkey_index = monkey->monkey_index_0;
                }

                if(DEBUG)printf("    Item with worry level %i is thrown to monkey %i.\n\n",*item,monkey_index);

                Monkey *receiving_monkey = get_monkey_by_index(monkey_pool,monkey_count,monkey_index);
                receiving_monkey->items[receiving_monkey->item_count] = *item;
                receiving_monkey->item_count ++;
                *item = 0;
                item_count --;

            }

            monkey->item_count = item_count;

        }

        
        
    }

    for (size_t i = 0; i < monkey_count; i++)
    {
        Monkey *monkey = &monkey_pool[i];
        printf("Monkey %i ",monkey->index);
        for (size_t j = 0; j < monkey->item_count; j++)
        {
            printf("%i, ",monkey->items[j]);
        }
        printf("\n");
        
    }
    int most_active_index[2] = {0,0};
    int most_active_value[2] = {0,0};   
    for (size_t i = 0; i < monkey_count; i++)
    {
        Monkey *monkey = &monkey_pool[i];
        printf("Monkey %i inspected items %i times.\n",monkey->index,monkey->number_of_inspections);

        if(monkey->number_of_inspections > most_active_value[0])
        {
            most_active_value[1] = most_active_value[0];
            most_active_index[1] = most_active_index[0];
            most_active_value[0] = monkey->number_of_inspections;
            most_active_index[0] = i;
        }
        else if(monkey->number_of_inspections > most_active_value[1])
        {
            most_active_value[1] = monkey->number_of_inspections;
            most_active_index[1] = i;
        }
    }

     
    printf("%i\n", most_active_value[0]*most_active_value[1]);
    
    return 1;
}

