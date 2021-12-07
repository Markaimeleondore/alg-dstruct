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
node* init_node(int vert1, int vert2)
{
    node* inited = (node*)malloc(sizeof(node));
    inited->vert1 = vert1;
    inited->vert2 = vert2;
    inited->next = NULL;
    return inited;
}

void add_node(node* node_ptr, int vert1, int vert2)
{
    node* ptr_to_next = node_ptr->next;
    node* new_node = (node*)malloc(sizeof(node));
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
        cur = cur->next;
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
            free(tracking);
            free(when_visited);
            free(start);
            size_of_graph = 0;
        return 1;
        }
    }
    return 0;
}

int main()
{
    char* file_name = "hamilton_chain_mixed.txt";
    get_data_and_receive_data(file_name, "output1.txt");
    return 0;
}
