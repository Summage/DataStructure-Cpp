//
// Created by summage on 8/3/2022.
//

#ifndef DATASTRUCT_TREE_H
#define DATASTRUCT_TREE_H
#include <algorithm>
#include <deque>
#include <iostream>
#include <iomanip>
#include <math.h>

#define RightLeafLabel 1
#define LeftLeafLabel 0

class Block_AVLTree{
    friend class AVLTree;

public:
    int isRightLeafLabel() const;
    int showKey() const;
    int showVal() const;

private:
    int LDepth = 0, RDepth = 0, key, label = 0;
    Block_AVLTree * left = nullptr, * right = nullptr;
    Block_AVLTree * parent = nullptr;

    explicit Block_AVLTree(int val);
    Block_AVLTree(int val, Block_AVLTree * root);
    Block_AVLTree(int val, Block_AVLTree * root, Block_AVLTree * l, Block_AVLTree * r);

    void setLabel(int l);
    ~Block_AVLTree();
};
class AVLTree{
    // 1 重复关键字
    // -1 关键字不存在
    Block_AVLTree * root = nullptr;
public:
    explicit AVLTree(int val);
    //set up from an ordered array
    AVLTree(int * array, int len);

    Block_AVLTree * find(int key);

    static Block_AVLTree * tackleDul(Block_AVLTree * x, Block_AVLTree * y);

    int insert(int val);
    int insert(Block_AVLTree * block);
    int insertReplic(Block_AVLTree * block);

    int del(int key);

    Block_AVLTree * pop(int key);

    void visualize(int indent);

private:
    std::deque<Block_AVLTree *> queue[2];
    inline static int calDepth(Block_AVLTree * node);

    inline static void visualizeBlock(Block_AVLTree * node, int indent, std::deque<Block_AVLTree *>& queue);

    void fixDel(Block_AVLTree * node, int label);

    Block_AVLTree * fix(Block_AVLTree * node);
    Block_AVLTree * fixR(Block_AVLTree * node);
    Block_AVLTree * fixL(Block_AVLTree * node);

    Block_AVLTree * fixRR(Block_AVLTree * node);
    Block_AVLTree * fixLL(Block_AVLTree * node);
    Block_AVLTree * fixRL(Block_AVLTree * node);
    Block_AVLTree * fixLR(Block_AVLTree * node);
};
#endif //DATASTRUCT_TREE_H
