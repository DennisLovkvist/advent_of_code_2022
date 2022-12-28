
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


typedef struct Valve Valve;
struct Valve
{
    int id, index;
    int flow_rate;
    int neighbour_count;
    Valve **neighbours;
    int *distances;

    int connections[6];
    int connection_count;
    int best_at_t[30];

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
void parse_line(char *line, Valve *valve, int valve_index)
{

    valve->id = (line[7] - 65) * 26 + (line[6] - 65);
    int index = 23;
    char c = line[index];

    while (c != ';')
    {
        index++;
        c = line[index];
    }
    valve->flow_rate = parse_number(line, 23, index);

    while (1)
    {
        if (c == ',')
        {
            index -= 2;
            goto lol;
        }
        else if (c == '\0')
        {
            valve->connections[valve->connection_count] = (line[index - 1] - 65) * 26 + (line[index - 2] - 65);
            valve->connection_count++;
            goto end;
        }
        index++;
        c = line[index];
    }
lol:
    c = line[index];

    while (1)
    {
        if (c == ',')
        {
            valve->connections[valve->connection_count] = (line[index - 1] - 65) * 26 + (line[index - 2] - 65);
            valve->connection_count++;
        }
        else if (c == '\0')
        {
            valve->connections[valve->connection_count] = (line[index - 1] - 65) * 26 + (line[index - 2] - 65);
            valve->connection_count++;
            goto end;
        }

        index++;
        c = line[index];
    }
end:
}
void valve_init(Valve *valve, int valve_count_total)
{
    valve->id = 0;
    valve->flow_rate = 0;
    valve->neighbour_count = valve_count_total;

    valve->neighbours = malloc(sizeof(Valve *) * valve_count_total);
    valve->distances = malloc(sizeof(int) * valve_count_total);

    for (size_t i = 0; i < valve_count_total; i++)
    {
        valve->neighbours[i] = NULL;
        valve->distances[i] = 1000000;
    }
    for (size_t i = 0; i < 6; i++)
    {
        valve->connections[i] = 0;
    }
    valve->connection_count = 0;

    for (size_t i = 0; i < 30; i++)
    {
        valve->best_at_t[i] = -1;
    }
}
void calc_distance(Valve *origin, Valve *valve, int step)
{
    valve->distances[origin->index] = step;

    for (size_t i = 0; i < valve->connection_count; i++)
    {
        for (size_t j = 0; j < valve->neighbour_count; j++)
        {
            if (valve->neighbours[j]->id == valve->connections[i])
            {
                if (valve->neighbours[j]->distances[origin->index] > step + 1)
                {
                    calc_distance(origin, valve->neighbours[j], step + 1);
                }
                break;
            }
        }
    }
}
void id_to_readable(int id, char *notation)
{
    int n1 = (id % 26) + 65;
    int n2 = (id / 26) + 65;

    notation[0] = n1;
    notation[1] = n2;
    notation[2] = '\0';
}
Valve *parse_input(int *valve_count)
{
    char *input_raw = load_input("input.txt");
    int length = strlen(input_raw);

    int index = 0;
    int start = 0;

    

    while (index <= length)
    {
        char c = input_raw[index];
        if (c == '\n' || c == '\0')
        {
            *valve_count = *valve_count +1;
        }
        index++;
    }

    int lc = *valve_count;


    Valve *valves = malloc(sizeof(Valve) * lc);

    for (size_t i = 0; i < lc; i++)
    {
        valve_init(&valves[i], lc);
    }

    index = 0;
    int valve_index = 0;
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

                parse_line(line, &valves[valve_index], valve_index);
                valve_index++;
            }

            start = index + 1;
        }

        index++;
    }
    for (size_t i = 0; i < lc; i++)
    {
        valves[i].index = i;
        for (size_t j = 0; j < lc; j++)
        {
            valves[i].neighbours[j] = &valves[j];
        }
    }

    for (size_t i = 0; i < lc; i++)
    {
        calc_distance(&valves[i], &valves[i], 0);
    }
    free(input_raw);

    return valves;
}
int max_flow(Valve *valve,int time, int depth, int max_depth, int *open_valves, int valve_count)
{
    int flow = valve->flow_rate * (30-(time));
    open_valves[valve->index] = 1;
    if(depth >= max_depth)return flow;
    if(time >= 30)return 0;
    int best_sub_flow = 0;  

    for (size_t i = 0; i < valve->neighbour_count; i++)
    {
        if(!open_valves[i])
        {
            Valve *next_valve = valve->neighbours[i];
            int time_cost = valve->distances[i]+1;

            int *open_valves_copy = malloc(sizeof(int)*valve_count);
            for (size_t j = 0; j < valve_count; j++)
            {
                open_valves_copy[j] = open_valves[j];
            }
            int sub_flow = 0;

            
            sub_flow = max_flow(next_valve,time + time_cost,depth+1,max_depth,open_valves_copy,valve_count);

            
            free(open_valves_copy);

            if(sub_flow > best_sub_flow)
            {
                best_sub_flow = sub_flow;
            }
        }
    }

    return flow + best_sub_flow;
}
void part1()
{
    int valve_count = 0;
    Valve *valves = parse_input(&valve_count);

    int *open_valves = malloc(sizeof(int)*valve_count);

    int max_depth = 1;
    Valve *starting_valve;
    for (size_t i = 0; i < valve_count; i++)
    {
        if(valves[i].flow_rate == 0)
        {
            open_valves[i] = 1;
        }
        else
        {
            open_valves[i] = 0;
            max_depth++;
        }
        if(valves[i].id == 0)
        {
            starting_valve = &valves[i];
        }
    }
    int f = max_flow(starting_valve,0,1,max_depth,&open_valves[0],valve_count);

    printf("%i\n",f);   


    
}

int main()
{
    part1();
    
    return 1;
}
