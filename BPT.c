#include <stdio.h>
#include <stdlib.h>
#define true 1
#define false 0

#define param 2
#define children_num 2 * param + 1
#define max_key_number 2 * param

typedef struct BPT_node
{
    int is_leaf;
    int num_of_keys;
    struct BPT_node* parent, * left, * right;
    struct BPT_node* children[children_num];
    int keys[max_key_number];
} node;


void delete_from_node(node* root, node* node, int key);
void merge(node* root, node* some_node, int key);


node* construct_node()
{
    node * new_node = (node *)malloc(sizeof(node));

    if (!new_node)
        return NULL;

    new_node->is_leaf = true;
    new_node->parent = new_node->left = new_node->right = NULL;
    new_node->num_of_keys = 0;

    for (int i = 0; i < children_num; i++)
        new_node->children[i] = NULL;

    return new_node;
}


node* get_node(node* root, int searching_for)
{
    node* cur = root;
    if (!cur)
        return NULL;
    while (cur->is_leaf == false)
    {
        for (int i = 0; i <= cur->num_of_keys; i++)
        {
            if (cur->num_of_keys == i || searching_for < cur->keys[i])
            {
                cur = cur->children[i];
                break;
            }
        }
    }
    return cur;
}


int is_in_tree(node* root, int searching_for)
{
    node* buffer = get_node(root, searching_for);
    if (buffer)
        for (int i = 0; i < buffer->num_of_keys; i++)
            if (buffer->keys[i] == searching_for)
                return true;
    return false;
}


int get_min(node* node)
{
    for (int i = 0; i < max_key_number; i++)
        if (node->children[i])
            return get_min(node->children[i]);
    return node->keys[0];
}


int get_position(node* some_leaf, int key)
{
    int position = 0;
    while (position < some_leaf->num_of_keys && some_leaf->keys[position] < key)
        position++;
    return position;
}


void update_keys(node* some_node, int key)
{
    int pos_upd = 0;
    node* buffer = some_node->parent;
    while (buffer)
    {
        pos_upd = get_position(buffer, key);
        if(pos_upd < buffer->num_of_keys)
            buffer->keys[pos_upd] = get_min(buffer->children[pos_upd + 1]);
        buffer = buffer->parent;
    }
}


void split(node** root, node* some_node)
{
    node* new_node = construct_node();
    new_node->is_leaf = false;

    if (some_node->right)
    {
        new_node->right = some_node->right;
        some_node->right->left = new_node;
        some_node->right = new_node;
        new_node->left = some_node;
    }
    else
    {
        new_node->right = some_node->right;
        some_node->right = new_node;
        new_node->left = some_node;
    }

    int key_value = some_node->keys[param];
    new_node->num_of_keys = param - 1;
    some_node->num_of_keys = param;

    for (int i = 0; i < new_node->num_of_keys; i++)
    {
        new_node->keys[i] = some_node->keys[i + param + 1];
        new_node->children[i] = some_node->children[i + param + 1];
        some_node->children[i + param + 1] = NULL;
    }
    new_node->children[new_node->num_of_keys] = some_node->children[max_key_number];
    some_node->children[max_key_number] = NULL;

    if (some_node->is_leaf)
    {
        new_node->num_of_keys++;
        new_node->is_leaf = true;
        for (int i = new_node->num_of_keys - 1; i >= 1; i--)
            new_node->keys[i] = new_node->keys[i - 1];
        new_node->keys[0] = some_node->keys[param];
    }
    if (some_node == (*root))
    {
        (*root) = (node *)malloc(sizeof(node));
        if (!(*root))
            return;
        (*root)->keys[0] = key_value;
        (*root)->children[0] = some_node;
        (*root)->children[1] = new_node;
        for (int i = 0; i < children_num; i++)
        {
            if (new_node->children[i])
                new_node->children[i]->parent = new_node;
            if (some_node->children[i])
                some_node->children[i]->parent = some_node;
        }
        (*root)->is_leaf = false;
        (*root)->parent = (*root)->left = (*root)->right = NULL;
        for (int i = 2; i < children_num; i++)
            (*root)->children[i] = NULL;
        (*root)->num_of_keys = 1;
        some_node->parent = (*root);
        new_node->parent = (*root);
    }
    else
    {
        new_node->parent = some_node->parent;
        int pos = get_position(some_node->parent, key_value);

        for (int i = some_node->parent->num_of_keys; i >= pos; i--)
            some_node->parent->keys[i] = some_node->parent->keys[i - 1];
        for (int i = some_node->parent->num_of_keys + 1; i >= pos + 2; i--)
            some_node->parent->children[i] = some_node->parent->children[i - 1];

        for (int i = 0; i < max_key_number; i++)
            if (new_node->children[i])
                new_node->children[i]->parent = new_node;
        some_node->parent->keys[pos] = key_value;
        some_node->parent->children[pos + 1] = new_node;
        some_node->parent->num_of_keys++;

        if (some_node->parent->num_of_keys == max_key_number)
            split(root, some_node->parent);
    }
}


void insert(node** root, int key)
{
    node * leaf = get_node((*root), key);
    if (!leaf)
        return;
    for (int i = 0; i < leaf->num_of_keys; i++)
    {
        if (leaf->keys[i] == key)
            return;
    }
    int pos = get_position(leaf, key);
    for (int i = leaf->num_of_keys; i >= pos + 1; i--)
        leaf->keys[i] = leaf->keys[i - 1];
    leaf->keys[pos] = key;
    leaf->num_of_keys++;
    if (leaf->num_of_keys == max_key_number)
        split(root, leaf);
}


void merge(node* root, node* some_node, int key)
{
    if (!root || !some_node)
        return;
    if (some_node->right && some_node->right->parent == some_node->parent)
    {
        for (int i = 0; i < some_node->right->num_of_keys; i++)
        {
            some_node->keys[some_node->num_of_keys] = some_node->right->keys[i];
            some_node->children[some_node->num_of_keys + 1] = some_node->right->children[i];
            if (some_node->right->children[i])
                some_node->right->children[i]->parent = some_node;
            some_node->num_of_keys++;
        }
        some_node->children[some_node->num_of_keys + 1] = some_node->right->children[some_node->right->num_of_keys];
        if (some_node->right->children[some_node->right->num_of_keys])
            some_node->right->children[some_node->right->num_of_keys]->parent = some_node;
        if (some_node->right->right)
            some_node->right->right->left = some_node;
        some_node->right = some_node->right->right;

        delete_from_node(root, some_node->parent, get_min(some_node->right));
        update_keys(some_node, key);
    }
    else
    {
        for (int i = 0; i < some_node->num_of_keys; i++)
        {
            some_node->left->keys[some_node->left->num_of_keys] = some_node->keys[i];
            some_node->left->children[some_node->left->num_of_keys + 1] = some_node->children[i];
            if (some_node->children[i])
                some_node->children[i]->parent = some_node->left;
            some_node->left->num_of_keys++;
        }
        some_node->left->children[some_node->left->num_of_keys + 1] = some_node->children[some_node->num_of_keys];
        if (some_node->children[some_node->num_of_keys])
            some_node->children[some_node->num_of_keys]->parent = some_node->left;
        some_node->left->right = some_node->right;
        if (some_node->right)
            some_node->right->left = some_node->left;
        delete_from_node(root, some_node->left->parent, get_min(some_node));
        update_keys(some_node->left, key);
    }
}


void delete_from_node(node* root, node* some_node, int key)
{
    if (!root || !some_node)
        return;
    int pos = get_position(some_node, key);
    for (int i = pos; i < some_node->num_of_keys; i++)
        some_node->keys[i] = some_node->keys[i + 1];
    for (int i = pos + 1; i <= some_node->num_of_keys; i++)
        some_node->children[i] = some_node->children[i + 1];
    for (int i = some_node->num_of_keys + 1; i < children_num; i++)
        some_node->children[i] = NULL;
    some_node->num_of_keys--;
    if (some_node == root)
    {
        for (int i = 0; i < children_num; i++)
        {
            if (some_node->children[i])
            {
                if (some_node->children[i]->num_of_keys == 0)
                    some_node->children[i] = NULL;
            }
        }
        if (root->num_of_keys == 0 && root->children[0])
            root = root->children[0];
        return;
    }
    if (some_node->num_of_keys >= 1) update_keys(some_node, key);
    else
    {
        if (some_node->num_of_keys < param - 1)
        {
            node* right = some_node->right;
            node* left = some_node->left;
            if (left && left->num_of_keys > param - 1 && left->parent == some_node->parent)
            {
                left->num_of_keys--;
                some_node->num_of_keys++;
                for (int i = 1; i < some_node->num_of_keys; i++)
                {
                    some_node->keys[i] = some_node->keys[i - 1];
                    some_node->children[i] = some_node->children[i - 1];
                }
                some_node->children[some_node->num_of_keys] = some_node->children[some_node->num_of_keys - 1];
                some_node->keys[0] = left->keys[left->num_of_keys];
                some_node->children[0] = left->children[left->num_of_keys + 1];
                left->children[left->num_of_keys + 1] = NULL;
                if (some_node->children[0])
                    some_node->children[0]->parent = some_node;
                update_keys(some_node, key);
            }
            else
            {
                if (right && right->num_of_keys > param - 1 && right->parent == some_node->parent)
                {
                    some_node->num_of_keys++;
                    some_node->keys[some_node->num_of_keys - 1] = right->keys[0];
                    some_node->children[some_node->num_of_keys] = right->children[0];
                    if (some_node->children[some_node->num_of_keys])
                        some_node->children[some_node->num_of_keys]->parent = some_node;
                    for (int i = 0; i < right->num_of_keys; i++)
                    {
                        right->keys[i] = right->keys[i + 1];
                        right->children[i] = right->children[i + 1];
                    }
                    for (int i = 0; i < max_key_number; i++)
                    {
                        if (right->children[i] == right->children[i + 1] && right->children[i])
                        {
                            right->children[i + 1] = NULL;
                            break;
                        }
                    }
                    right->num_of_keys--;
                    update_keys(some_node, key);
                }
                else
                    merge(root, some_node, key);
            }
        }

    }
}


void delete_by_key(node* root, int key)
{
    if (!root)
        return;
    if (!is_in_tree(root, key))
        return;
    node* leaf = get_node(root, key);
    delete_from_node(root, leaf, key);
}


void print(node * root, int lvl, int child)
{
    if(root)
    {
        for(int i = 0; i < root->num_of_keys; i++)
            printf("lvl %i child %i key %i\n", lvl, child, root->keys[i]);
        for (int i = 0; i < children_num; i++)
            if(root->children[i])
                print(root->children[i], lvl + 1, i);
    }
}


int main()
{

    node* root = construct_node();
    char req;
    int key;
    while (fscanf(stdin, "%c %d", &req, &key) > 0)
    {
        switch (req)
        {
        case 'a':
            insert(&root, key);
            break;
        case 'r':
            delete_by_key(root, key);
            break;
        case 'f':
            if (is_in_tree(root, key))
                printf("yes\n");
            else
                printf("no\n");
            break;
        case 'd':
            print(root, 0, 0);
        }
    }
    return 0;
}
