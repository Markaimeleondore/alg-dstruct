#include <stdio.h>
#include <stdlib.h>
#include "splay.h"

node* search_and_splay(node* root, int value);

node* construct_node(int value)
{
    node* new_one = (node*)malloc(sizeof(node));
    if(new_one == NULL)
        return NULL;
    new_one->value = value;
    new_one->left = NULL;
    new_one->right = NULL;
    return new_one;
}


node* add_node(node* root, int value)
{
    if(root == NULL)
        return construct_node(value);
    root = search_and_splay(root, value);
    if(root->value == value)
        return root;
    node* node2add = construct_node(value);
    if(root->value > value)
    {
        node2add->right = root;
        node2add->left = root->left;
        root->left = NULL;
    }
    else
    {
        node2add->left = root;
        node2add->right = root->right;
        root->right = NULL;
    }
    return node2add;
}


void free_the_tree(node* root)
{
    if(root)
    {
        free_the_tree(root->left);
        free_the_tree(root->right);
        free(root);
    }
}


node* rotate_left(node* chosen_node)
{
    if(!chosen_node)
        return NULL;
    node * buffer = chosen_node->right;
    chosen_node->right = buffer->left;
    buffer->left = chosen_node;
    return buffer;
}


node* rotate_right(node* chosen_node)
{
    if(!chosen_node)
        return NULL;
    node * buffer = chosen_node->left;
    chosen_node->left = buffer->right;
    buffer->right = chosen_node;
    return buffer;
}


node* search_and_splay(node* root, int value)
{
    if(root == NULL || root->value == value)
        return root;
    if(root->value > value)
    {
        if(root->left == NULL)
            return root;
        if(root->left->value > value)
        {
            root->left->left = search_and_splay(root->left->left, value);
            root = rotate_right(root);
        }
        else if (root->left->value < value)
        {
            root->left->right = search_and_splay(root->left->right, value);

            if(root->left->right)
            {
                root->left = rotate_left(root->left);
            }
        }
        return (root->left) ? rotate_right(root) : root;
    }
    else
    {
        if(root->right == NULL)
            return root;
        if(root->right->value > value)
        {
            root->right->left = search_and_splay(root->right->left, value);
            if(root->right->left)
                root->right = rotate_right(root->right);
        }
        else if(root->right->value < value)
        {
            root->right->right = search_and_splay(root->right->right, value);
            root = rotate_left(root);
        }
        return  (root->right) ? rotate_left(root) : root;
    }
}


node* delete_by_value(node* root, int value)
{
    if(!root)
        return NULL;
    node* cur;
    root = search_and_splay(root, value);
    if(root->value != value)
        return root;
    cur = root;
    if(!root->left)
    {
        root = root->right;
    }
    else
    {
        root = search_and_splay(root->left, value);
        root->right = cur->right;
    }
    //free(cur);
    return root;
}


void make_things_done()
{
    node* root = NULL;
    char req;
    int key;
    while(fscanf(stdin, "%c %d", &req, &key) > 0)
    {
        switch(req)
        {
            case 'a':
                root = add_node(root, key);
                break;
            case 'r':
                root = delete_by_value(root, key);
                break;
            case 'f':
                root = search_and_splay(root, key);
                if(root)
                {
                    if(root->value == key)
                        printf("yes\n");
                    else
                    printf("no\n");
                }
                else
                    printf("no\n");
                break;
        }
    }
    free(root);
}


int main()
{
    make_things_done();
    return 0;
}
