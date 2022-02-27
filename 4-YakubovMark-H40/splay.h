#ifndef SPLAY_H_INCLUDED
#define SPLAY_H_INCLUDED


typedef struct splay_node
{
    int value;
    struct splay_node * left;
    struct splay_node * right;
} node;


node* construct_node(int value);
node* add_node(node* root, int value);
void free_the_tree(node* root);
node* rotate_left(node* chosen_node);
node* rotate_right(node* chosen_node);
node* search_and_splay(node* root, int value);
node* delete_by_value(node* root, int value);
void make_things_done();

#endif // SPLAY_H_INCLUDED
