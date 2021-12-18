#include <stdio.h>
#include <stdlib.h>
//#pragma once
#define MAXN 100000
typedef struct node
{
    int vert1;
    int vert2;
    struct node* next;
} node;

int *when_visited;
int *tracking;
int size_of_graph;
node* start;
int * tests_results;
int number_of_tests;

node* init_node(int vert1, int vert2)
{
    node* inited = (node*)malloc(sizeof(node));
    if(inited == NULL)
    {
        fprintf(stderr, "Memory allocation error in init_node function.\n");
        return NULL;
    }

    inited->vert1 = vert1;
    inited->vert2 = vert2;
    inited->next = NULL;
    return inited;
}


void add_node(node* node_ptr, int vert1, int vert2)
{
    node* ptr_to_next = node_ptr->next;
    node* new_node = (node*)malloc(sizeof(node));
    if(new_node == NULL)
    {
        fprintf(stderr, "Memory allocation error in add_node function.\n");
        return;
    }

    node_ptr->next = new_node;
    new_node->vert1 = vert1;
    new_node->vert2 = vert2;
    new_node->next = ptr_to_next;

}


int edge_checker(int vert1, int vert2, node* start)
{
    node* cur = start;
    if(!cur)
        return -1;

    while(cur)
    {
        if((cur->vert1 == vert1 && cur->vert2 == vert2) || (cur->vert1 == vert2 && cur->vert2 == vert1))
            return 1;
        cur = cur -> next;
    }
    return 0;
}


int hamilton(int k, int graph_size, int *when_visited, int *tracking, node* start_edges)
{
    int flag = 0;
    for(int v = 1; v <= graph_size && !flag; v++)
    {
        if(edge_checker(v, tracking[k - 1], start_edges))
        {
            if(k == graph_size) flag = 1;
            else if(when_visited[v] == -1)
            {
                when_visited[v] = k;
                tracking[k] = v;
                flag = hamilton(k + 1, graph_size, when_visited, tracking, start_edges);
                if(!flag) when_visited[v] = -1;
            }
        }

    }
    return flag;
}


int init_graph(int size)
{
    if(size <= 1)
        return -1;
    when_visited = (int*)malloc(sizeof(int) * (size_of_graph + 1));
    tracking = (int*)malloc(sizeof(int) * (size_of_graph + 1));
    if(when_visited == NULL || tracking == NULL)
        return -1;

    for(int i = 1; i <= size_of_graph; i++)
    {
        when_visited[i] = -1;
    }
    tracking[1] = 1;
    when_visited[1] = 1;
    return 1;
}


void to_free_structs()
{
    free(tracking);
    free(when_visited);
    free(start);
    size_of_graph = 0;
}


int get_data_and_receive_data(char input_name[], char output_name[])
{
   FILE *read_data;
    if((read_data = fopen(input_name, "r")))
    {
        fscanf(read_data,"%d\n", &size_of_graph);
        if(size_of_graph > 1)
        {
            init_graph(size_of_graph);
            int vert1, vert2;
            fscanf(read_data,"%d %d\n", &vert1, &vert2);
            start = init_node(vert1, vert2);
            while(fscanf(read_data,"%d %d\n", &vert1, &vert2) != EOF)
            {
                add_node(start, vert1, vert2);
            }
            fclose(read_data);

            FILE *file = fopen(output_name, "w");
            if(hamilton(2, size_of_graph + 1, when_visited, tracking, start))
            {
               for(int i = 1; i <= size_of_graph; i++) fprintf(file,"%d ", tracking[i]);
            }
            else
            {
                fprintf(file,"%d", 0);
            }
            to_free_structs();
        return 1;
        }
    }
    return 0;
}

int run_test(char input[])
{
    FILE *read_data;
    if((read_data = fopen(input, "r")))
    {
        char trash;
        int vert1, vert2; 
        fscanf(read_data,"%d\n", &number_of_tests);
        if(number_of_tests < 1)
            return -1;
        tests_results = (int*)malloc(sizeof(int) * (number_of_tests + 1));
        int cnt = 0;
        while(cnt < number_of_tests)
        {
            fscanf(read_data,"%d\n", &size_of_graph);
            if(init_graph(size_of_graph) == -1)
                return -1;
            fscanf(read_data,"%d %d%c", &vert1, &vert2, &trash);
            start = init_node(vert1, vert2);
            while(trash != '-')
            {
                fscanf(read_data,"%d %d%c", &vert1, &vert2, &trash);
                add_node(start, vert1, vert2);
            }
            tests_results[cnt] = 1;
            if(hamilton(2, size_of_graph + 1, when_visited, tracking, start))
            {
                int * answer = (int*)malloc(sizeof(int) * (size_of_graph + 1));
                if(answer == NULL)
                    return -1;
                for(int i = 0; i < size_of_graph; i++)
                {
                    fscanf(read_data, "%d ", &answer[i]);
                    if(answer[i] != tracking[i + 1])
                    {
                        tests_results[cnt] = 0;
                        break;
                    }
                }
            }
            else
            {
                int is_zero;
                fscanf(read_data, "%d \n", &is_zero);
                if(is_zero != 0)
                {
                    tests_results[cnt] = 0;
                }
            }
            cnt++;
            to_free_structs();
        }
        fclose(read_data);
        return 1;
    }
    fprintf(stderr, "Wrong file name.\n");
    return -1;
}



int main()
{
    char file_name[] = "test_compilation.txt";
    if(run_test(file_name) == 1)
    {
    for(int i = 0; i < number_of_tests; i++)
        {
            if(tests_results[i])
                printf("Test number %d is passed successfully\n", i + 1);
            else
                printf("Test number %d is failed\n", i + 1);
        }
    free(tests_results);
    }
    return 0;
}
