#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define true 1
#define false 0

typedef struct hash_node
{
    char * data;
    struct hash_node * next;
} node;

int hashify(char * str, unsigned int table_size)
{
    if(!str)
        return -1;
    size_t hash_sum = 0;
    int len = strlen(str);
    for(int i = 0; i < len; i++)
        hash_sum += str[i];
    return hash_sum % table_size;
}


int find(node ** table, unsigned int table_size, char * str)
{
    node * cur = table[hashify(str, table_size)];
    while(cur)
    {
        if(!strcmp(cur->data, str))
            return 1;
        cur = cur->next;
    }
    return 0;
}


void add(node ** table, unsigned int table_size, char * str)
{
    if(find(table, table_size, str))
       return;
    node * new_node = (node*)malloc(sizeof(node));
    new_node->data = (char*)malloc(strlen(str) + 1);
    if(!new_node) return;
    unsigned int id = hashify(str, table_size);
    strcpy(new_node->data, str);
    new_node->next = table[id];
    table[id] = new_node;
}


void del(node ** table, unsigned int table_size, char * to_del)
{
    if(!find(table, table_size, to_del))
        return;
    unsigned int id = hashify(to_del, table_size);
    node * target_node = table[id];
    while(strcmp(target_node->data, to_del))
        target_node = target_node->next;
    if(!strcmp(target_node->data, table[id]->data))
    {
        if(table[id]->next)
            table[id] = table[id]->next;
        else
            table[id] = NULL;
        free(target_node->data);
        free(target_node);
    }
    else
    {
        node * cur = table[id];
        while(cur->next != target_node)
        {
            cur = cur->next;
        }
        cur->next = target_node->next;
        free(target_node->data);
        free(target_node);
    }
}


int main()
{
    const unsigned int size = 909091;
    node ** table = (node**)malloc(sizeof(node) * size);
    if(!table)
        return -1;
    for(int i = 0; i < size; i++)
        table[i] = NULL;
    char req;
	char key_val [100];
	while (scanf("%c", &req) > 0)
	{
	    switch(req)
	    {
	        case 'a':
                scanf("%s", &key_val);
                add(table, size, key_val);
                break;
            case 'r':
                scanf("%s", &key_val);
                del(table, size, key_val);
                break;
            case 'f':
                scanf("%s", &key_val);
            if (find(table, size, key_val))
                printf("%s", "yes\n");
            else
                printf("%s", "no\n");
	    }
	}
    return 0;
}
