#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAXN 100000
#define MN 50000.001
typedef struct node
{
    double beginning;
    double end;
    struct node * left;
    struct node * right;
    struct node * parent;
} node;

int solution_counter = 0;
double * solution_beginnings;
double * solution_ends;


node* addnode(node *tree, double beginning, double end)
{
  if (tree == NULL)
    {
    tree = malloc(sizeof(node));
    if(!tree)
        return NULL;
    tree->beginning = beginning;
    tree->end = end;
    tree->left =  NULL;
    tree->right = NULL;
    }
  else  if (beginning < tree->beginning)
    tree->left = addnode(tree->left, beginning, end);
  else
    tree->right = addnode(tree->right, beginning, end);
  return(tree);
}


node* find_by_borders(node* head, double beginning, double end)
{
    node* cur = head;
    while(cur)
    {
        if(cur->beginning == beginning && cur->end == end)
            return cur;
        if(beginning < cur->beginning)
            cur = cur->left;
        else
            cur = cur->right;
    }
    return NULL;
}


node* get_min(node * start)
{
    node* cur = start;
    while(cur->left)
    {
        cur = cur->left;
    }
    return cur;
}


node* get_next_after_deleted(node* start)
{
    node* left_buffer = NULL, *parent_buffer = start;
    if(parent_buffer->right)
        return get_min(parent_buffer->right);
    left_buffer = parent_buffer->parent;
    while(left_buffer && (parent_buffer == left_buffer->right))
    {
        parent_buffer = left_buffer;
        left_buffer = left_buffer->parent;
    }
    return left_buffer;
}


int delete_by_border(node* head, double beginning, double end)
{
    node* cur, *buffer;
    cur = find_by_borders(head, beginning, end);
    /* No subtrees case */
    if(cur->left == NULL && cur->right == NULL)
    {
        if(cur->parent != NULL)
            buffer = cur->parent;
        if(cur == buffer->right) buffer->right = NULL;
        else buffer->left = NULL;
        free(cur);
    }
    /* only right subtree */
    if(cur->left == NULL && cur->right != NULL)
    {
        if(cur->parent != NULL)
            buffer = cur->parent;
        if(cur == buffer->right)
            buffer->right = cur->right;
        else buffer->left = cur->right;
        free(cur);
    }
    /* only left subtree */
    if(cur->left != NULL && cur->right == NULL)
    {
        if(cur->parent != NULL)
            buffer = cur->parent;
        if(cur == buffer->right)
            buffer->right = cur->left;
        else
            buffer->left = cur->left;
        free(cur);
    }
    /* Two subtrees case */
    if(cur->left != NULL && cur->right != NULL)
    {
        buffer = get_next_after_deleted(cur);
        cur->beginning = buffer->beginning;
        cur->end = buffer->end;
        if(buffer->right == NULL)
            buffer->parent->left = NULL;
        else
            buffer->parent->left = buffer->right;
        free(buffer);
    }
    return 1;
}


int compare_borders(node* cur, double beginning, double end)
{
    if(cur->beginning <= beginning && cur->end >= beginning)
        return 1;
    else if(cur->beginning >= beginning && cur->beginning <= end)
        return 1;
    else
        return 0;
}


double* push_back(double* array, double element)
{
    int i = 0;
    while(array[i] != MN) i++;
    array[i] = element;
    return array;
}


void intersections(node* start, double beginning, double end, double * beginnings, double * ends)
{
    if(start == NULL)
        return;
    intersections(start->left, beginning, end, beginnings, ends);
    if(compare_borders(start, beginning, end))
    {
        push_back(beginnings, start->beginning);
        push_back(ends, start->end);
        solution_counter++;
    }
    intersections(start->right, beginning, end, beginnings, ends);
}


int main()
{
    char file_name[] = "intersections.txt";
    FILE * data;
    if((data = fopen(file_name, "r")))
    {
        double beginning, end;
        int quantity_of_intervals;
        char marker;
        int quantity_of_requests;
        node * head = NULL;
        fscanf(data, "%d\n", &quantity_of_intervals);
        int iterator = quantity_of_intervals;
        double * beginnings = malloc(sizeof(double) * iterator);
        if(beginnings == NULL)
            return -1;
        double * ends = malloc(sizeof(double) * iterator);
        if(ends == NULL)
            return -1;
        while(iterator--)
        {
            fscanf(data, "%lf %lf\n", &beginning, &end);
            head = addnode(head, beginning, end);
        }
        fscanf(data, "%d\n", &quantity_of_requests);
        int * passed_or_not = malloc(sizeof(int)*quantity_of_requests);
        int test_cnt = 0;
        if(passed_or_not == NULL)
            return -1;
        while(quantity_of_requests--)
        {
        passed_or_not[test_cnt] = 1;
        fscanf(data, "%lf %lf\n", &beginning, &end);
        for(int i = 0; i < quantity_of_intervals; i++)
        {
            beginnings[i] = MN;
            ends[i] = MN;
        }
        intersections(head, beginning, end, beginnings, ends);
        solution_beginnings = malloc(sizeof(double) * solution_counter);
        solution_ends = malloc(sizeof(double) * solution_counter);
        if(!solution_beginnings || !solution_ends)
            return -1;
        for(int j = 0; j < solution_counter; j++)
        {
            fscanf(data, "%lf %lf\n", &solution_beginnings[j], &solution_ends[j]);
            if(solution_beginnings[j] != beginnings[j] || solution_ends[j] != ends[j])
            {
                printf("iter: %d test: %d sol: %lf %lf found: %lf %lf sol cnt: %d\n", j, test_cnt, solution_beginnings[j], solution_ends[j], beginnings[j], ends[j], solution_counter);
                passed_or_not[test_cnt] = 0;
                break;
            }
        }
        fscanf(data, "%c\n", &marker);
        free(solution_beginnings);
        free(solution_ends);
        solution_counter = 0;
        test_cnt++;
        }
        fclose(data);
        for(int i = 0; i < test_cnt; i++)
        {
            if(passed_or_not[i])
                printf("Test number: %d is successfully passed!\n", i + 1);
            else
                printf("Test number: %d is failed.\n", i + 1);
        }
        return 0;
    }
    else
        fprintf(stderr, "Wrong file name.\n");
    return -1;
}
