//
// Created by aleksey on 28.11.16.
//

#ifndef TASKSOPENMP_TREE_H
#define TASKSOPENMP_TREE_H

#include <cstdlib>

// Число вершин в результирующем дереве
int num_nodes = 0;
// Ограничиваем число создаваемых вершин
// Число вершин может быть несколько больше
const int low_limit_nodes = 150;
// Вероятность создания вершины - потомка (в данном случае 0,7)
const int percent = 70;// Для номеров вершин
const int plus_num_node = 50;
// Вершина графа (дерева)


struct node
{
    struct node *left;
    struct node *right;
    int num;
};

// Создаем дерево
void tree_init(struct node *tree, int mode)
{
    static int loc_val;
    if (mode < 1)
        loc_val = 1;
    tree->num = loc_val++;
    tree->left = NULL;
    tree->right = NULL;
    // Чтобы не плодить слишком большие деревья
    if (loc_val > low_limit_nodes)
        return;
    // Потомок слева
    int n = rand() % 100;
    if (n < percent)
    {
        tree->left = new node;
        tree_init(tree->left, 1);
    }
    // Потомок справа
    n = rand() % 100;
    if (n < percent)
    {
        tree->right = new node;
        tree_init(tree->right, 1);
    }
    return;
}

// Имитируем обработку узла
void work(int n)
{
    // Например, считаем факториал
    int k = 1;
    for(int i = 2; i <= (n * plus_num_node) * (n * plus_num_node) ; i++)
        k *= i;
    return;
}


#endif //TASKSOPENMP_TREE_H
