#ifndef TREE_H
#define TREE_H

#include <inttypes.h>
#include <stdbool.h>
#include <optional>
#include <vector>

typedef struct Node
{
    struct Node *prev;
    struct Node **children;
    int64_t value;
} Node;

void node_empty(Node *n);
void node_add_children(Node *root, Node *children, int64_t name);
void node_delete(Node *n, int64_t name);
void node_print_tree(Node* n);
std::optional<std::vector<int>> find_path(Node *n, int64_t name, std::vector<int> &current_path);
std::optional<std::vector<int>> find_path(Node* root ,int name);
int64_t node_get_children_count(Node *n);
int64_t ping(Node *n, int64_t name);
Node *node_find(Node *n, int64_t value);

#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <optional>
#include <vector>
#include <zmq.hpp>

#define SEPARATOR " "
#define TABS "\t\t\t\t\t\t\t\t\t\t"

int64_t max(int64_t n1, int64_t n2)
{
    if (n1 > n2)
        return n1;
    return n2;
}

void node_empty(Node *n)
{
    n->children = (Node **)calloc(1, sizeof(Node *));
    n->children[0] = NULL;
    n->prev = NULL;
    n->value = 0;
}

void node_delete(Node* n, int64_t name) {
    Node* victim = node_find(n, name);
    int64_t pcount = node_get_children_count(victim->prev);
    int64_t vcount = node_get_children_count(victim);


    int64_t index;
    for(int64_t i = 0; i < pcount; i++){
        if (victim->prev->children[i]->value == name) index = i;
    }
    for(int64_t i = index; i < pcount - 1; i++){
        victim->prev->children[i] = victim->prev->children[i+1];
    }

    victim->prev->children = (Node **)realloc(victim->prev->children, (pcount + vcount) * sizeof(Node *));

    for (int64_t i = 0; i < vcount; ++i) {
        victim->prev->children[pcount - 1 + i] = victim->children[i];
        victim->children[i]->prev = victim->prev;
    }
    victim->prev->children[pcount + vcount - 1] = NULL; 
    free(victim->children);
}


void _node_print_tree(Node* n, int lvl) {
    if (n == NULL) return;

    lvl += 1;
    int seps = (int) (strlen(SEPARATOR) * lvl);

    printf("%*s%ld", (int) (seps - strlen(SEPARATOR)), SEPARATOR, n->value);
    printf("\n");

    if (n->children[0] == NULL) return;

    printf("%*s\\\n", (int) (seps - strlen(SEPARATOR) + 1), SEPARATOR);
    for (uint64_t i = 0; n->children[i] != NULL; ++i) {
        _node_print_tree(n->children[i], lvl);
    }
}

void node_print_tree(Node* n) {
    _node_print_tree(n, 1);
}

void node_add_children(Node *parent, Node *child, int64_t name) {
    Node *node = node_find(parent, name);
    int64_t old_count = node_get_children_count(node);

    node->children = (Node **)realloc(node->children, (old_count + 2) * sizeof(Node *));
    node->children[old_count] = child;
    child->prev = node;
    node->children[old_count + 1] = NULL;
}

int64_t node_get_children_count(Node *n)
{
    int64_t count = 0;
    while (n->children[count] != NULL)
        ++count;
    return count;
}

std::optional<std::vector<int>> find_path(Node *n, int64_t name, std::vector<int> &current_path)
{
    if (n == nullptr)
        return std::nullopt;

    current_path.push_back(n->value);

    if (name == n->value)
        return current_path;
    int64_t count = node_get_children_count(n);

    for (int i = 0; i < count; i++)
    {
        std::optional<std::vector<int>> ok = find_path(n->children[i], name, current_path);
        if (ok) return ok;
    }
    current_path.pop_back();

    return std::nullopt;
}

std::optional<std::vector<int>> find_path(Node* root ,int name)
{
    std::vector<int> v;
    return find_path(root, name, v);
}

int64_t ping(Node *n, int64_t name)
{
    int64_t count = node_get_children_count(n);
    if (name == n->value)
    {
        return 1;
    }
    int64_t sum = 0;
    for (int64_t i = 0; i < count; i++)
    {
        sum += ping(n->children[i], name);
    }
    return sum;
}

Node *node_find(Node *n, int64_t value)
{
    if (n->value == value)
    {
        return n;
    }
    for (int64_t i = 0; i < node_get_children_count(n); i++)
    {
        Node *found = node_find(n->children[i], value);
        if (found != NULL)
        {
            return found;
        }
    }
    return NULL;
}