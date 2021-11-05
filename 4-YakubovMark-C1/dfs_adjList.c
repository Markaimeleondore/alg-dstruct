#include <stdio.h>
#include <stdlib.h>
#define MAXN 100000


typedef struct node
{
    int vert;
    struct node* next;
} node;

typedef struct graph_of_nodes
{
    int number_of_vert;
    int* visited;
    struct node** adjLists;
} graph;


int get_minimum_vert(graph* graph, node* temp)
{
    node* cur = temp;
    int min_size = MAXN;
    while(cur)
    {
        if(cur->vert < min_size && graph->visited[cur->vert] != 1)
        {
            min_size = cur->vert;
        }
        cur = cur->next;
    }
    return min_size == MAXN ? -1 : min_size;
}

int counter_of_not_visited(graph* graph, node* start_of_adjList)
{
    int cnt = 0;
    node* cur = start_of_adjList;
    while(cur)
    {
        if(graph->visited[cur->vert] != 1)
        {
           cnt++;
        }
        cur = cur->next;
    }
    return cnt;
}


void dfs(graph* graph, int vert) {
        struct node* start_of_adjList = graph->adjLists[vert];
        struct node* this_vert_iterator = start_of_adjList;

        if(vert == -1)
            return;

        int number_of_not_visited_verts_at_this_moment = counter_of_not_visited(graph, this_vert_iterator);
        graph->visited[vert] = 1;
        printf("%d ", vert);

        while(number_of_not_visited_verts_at_this_moment--) {
            int connected_vert = get_minimum_vert(graph, this_vert_iterator);

            if(graph->visited[connected_vert] == 0) {
                dfs(graph, connected_vert);
            }
        }
}


struct node* create_node(int number_of_vert)
{
    node* node_created = malloc(sizeof(node));
    node_created->vert = number_of_vert;
    node_created->next = NULL;
    return node_created;
}


graph* create_graph(int quantity_of_verts)
{
    graph* graph = malloc(sizeof(graph));
    graph->number_of_vert = quantity_of_verts;
    graph->adjLists = malloc(quantity_of_verts * sizeof(node*));
    graph->visited = malloc(quantity_of_verts * sizeof(int));

    for (int i = 0; i < quantity_of_verts; i++) {
        graph->adjLists[i] = NULL;
        graph->visited[i] = 0;
    }
    return graph;
}


void add_connection(graph* graph, int first_connected, int second_connected)
{
    //add connection first to second
    node* newNode = create_node(second_connected);
    newNode->next = graph->adjLists[first_connected];
    graph->adjLists[first_connected] = newNode;
    //add connection second to first
    newNode = create_node(first_connected);
    newNode->next = graph->adjLists[second_connected];
    graph->adjLists[second_connected] = newNode;
}


int main()
{
    int size_of_graph;
    scanf("%d", &size_of_graph);
    graph* new_graph = create_graph(size_of_graph);

    for(int i = 0; i < size_of_graph; i++)
    {
        int vert; char trash;
        scanf("%d%c", &vert, &trash);
        while(trash == ' ')
        {
            scanf("%d%c", &vert, &trash);
            add_connection(new_graph, i, vert);
        }
    }

    dfs(new_graph, 0);

    return 0;
}
