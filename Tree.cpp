//
// Created by summage on 8/3/2022.
//

#include "Tree.h"
#define RightLeafLabel 1
#define LeftLeafLabel 0

int Block_AVLTree::isRightLeafLabel() const{
    return label;
}
int Block_AVLTree::showKey() const{
    return key;
}
int Block_AVLTree::showVal() const{
    return key;
}
Block_AVLTree::Block_AVLTree(int val):key(val){}
Block_AVLTree::Block_AVLTree(int val, Block_AVLTree * root): key(val), parent(root){}
Block_AVLTree::Block_AVLTree(int val, Block_AVLTree * root, Block_AVLTree * l, Block_AVLTree * r):
        key(val), parent(root), left(l), right(r){
    if(l)
        LDepth = std::max(l->LDepth, l->RDepth)+1;
    if(r)
        RDepth = std::max(r->LDepth, r->RDepth)+1;
}

void Block_AVLTree::setLabel(int l){
    label = l;
}
Block_AVLTree::~Block_AVLTree()=default;



AVLTree::AVLTree(int val){
    root = new Block_AVLTree(val);
}
//set up from an ordered array
AVLTree::AVLTree(int * array, int len){

}

Block_AVLTree * AVLTree::find(int key){
    Block_AVLTree * nowPt = root;
    while(true){
        if (nowPt->key == key)
            return nowPt;
        else if(nowPt->key < key){
            if (nowPt->right)
                nowPt = nowPt->right;
            else
                break;
        }else{
            if (nowPt->left)
                nowPt = nowPt->left;
            else
                break;
        }
    }
    return nullptr;
}

Block_AVLTree * AVLTree::tackleDul(Block_AVLTree * x, Block_AVLTree * y){
    delete y;
    return x;
}

int AVLTree::insert(int val){
    auto * block = new Block_AVLTree(val);
    return insert(block);
}
int AVLTree::insert(Block_AVLTree * block){
    if(!root)
        root = block;
    Block_AVLTree * nowPt = root;
    while(true){
        if(nowPt->key == block->key){
            tackleDul(nowPt, block);
            return 1;
        } else if(nowPt->key < block->key) {
            if (nowPt->right) {
                nowPt = nowPt->right;
            }else {
                nowPt->right = block;
                block->parent = nowPt;
                block->setLabel(RightLeafLabel);
                nowPt->RDepth = 1;
                break;
            }
        }else{
            if (nowPt->left){
                nowPt = nowPt->left;
            }else{
                nowPt->left = block;
                block->parent = nowPt;
                block->setLabel(LeftLeafLabel);
                nowPt->LDepth = 1;
                break;
            }
        }
    }
    for(int label = block->label; nowPt->parent; nowPt = nowPt->parent){
        if(label == RightLeafLabel){
            if(nowPt->RDepth > nowPt->LDepth) {
                if (nowPt->isRightLeafLabel()) {
                    if (nowPt->parent->RDepth > nowPt->parent->LDepth)
                        nowPt = fixRR(nowPt->parent)->right;
                    else
                        nowPt->parent->RDepth++;
                }else {
                    if (nowPt->parent->RDepth < nowPt->parent->LDepth)
                        nowPt = fixLR(nowPt->parent)->left;
                    else
                        nowPt->parent->LDepth++;
                }
            }
            else
                break;
        }else{
            if(nowPt->RDepth < nowPt->LDepth){
                if(nowPt->isRightLeafLabel()) {
                    if (nowPt->parent->RDepth > nowPt->parent->LDepth)
                        nowPt = fixRL(nowPt->parent)->right;
                    else
                        nowPt->parent->RDepth++;
                }else {
                    if (nowPt->parent->RDepth < nowPt->parent->LDepth)
                        nowPt = fixLL(nowPt->parent)->left;
                    else
                        nowPt->parent->LDepth++;
                }
            }else
                break;
        }
        label = nowPt->label;
    }
    return 0;
}
int AVLTree::insertReplic(Block_AVLTree * block){
    //复制后插入
    return insert(block->key);
}

//如果存在目标节点，执行删除并完成对父节点指针的修改(集中在后部)
// 深度修改和平衡检查交由fixDel函数解决
int AVLTree::del(int key){
    Block_AVLTree * nowPt = find(key);

    if (!nowPt)
        return -1;

    Block_AVLTree * node;
    //找到替换节点并修改替换节点的子节点指针和深度
    //替换节点向上部分在后部集中修改
    if (nowPt->LDepth && nowPt->RDepth){
        //有双子树，使用较深侧的叶节点代替删除节点的位置
        //此时替代节点不会发生失衡
        if (nowPt->LDepth > nowPt->RDepth){
            // find the largest element on nowPt`s left
            for(node = nowPt->left; node->RDepth; node = node->right);
            if (node == nowPt->left){
                //如果目标的左子树根就是左侧最大值
                //此处修改前左侧最深，而此时左侧深度减一，所以必然往上传递
                node->right = nowPt->right;
                node->RDepth = nowPt->RDepth;
            }else{
                //将左侧最大值从原处剪下
                node->parent->right = nullptr;
                node->parent->RDepth = 0;
                //修复失衡
                if (node->parent->LDepth == 2)
                    fixL(node->parent);
                //嵌入目标节点位置
                node->left = nowPt->left;
                node->LDepth = calDepth(nowPt->left);
                node->right = nowPt->right;
                node->RDepth = nowPt->RDepth;
                //左侧深度无变化则停止上行
                if (node->LDepth > node->RDepth){
                    node->label = nowPt->label;
                    node->parent = nowPt->parent;
                    if (nowPt->isRightLeafLabel())
                        nowPt->parent->right = node;
                    else
                        nowPt->parent->left = node;
                    delete nowPt;
                    return 0;
                }
            }
        }else{
            for(node = nowPt->right; node->LDepth; node = node->left);
            if (node == nowPt->right){
                node->left = nowPt->left;
                node->LDepth = nowPt->LDepth;
            } else{
                node->parent->left = nullptr;
                node->parent->LDepth = 0;
                if (node->parent->RDepth == 2)
                    fixR(node->parent);
                node->left = nowPt->left;
                node->LDepth = nowPt->LDepth;
                node->right = nowPt->right;
                node->RDepth = calDepth(nowPt->right);
            }
            // 和上方位置不同的原因在于可能修正前左右深度变相同所以对两种替换节点都需要检查
            if (node->LDepth < node->RDepth){
                node->label = nowPt->label;
                node->parent = nowPt->parent;
                if (nowPt->isRightLeafLabel())
                    nowPt->parent->right = node;
                else
                    nowPt->parent->left = node;
                delete nowPt;
                return 0;
            }
        }
    }else
        //只有一个子树或没有子树时使用仅有的子树或空指针作为node且必然上行
        node = (nowPt->left) ? nowPt->left : nowPt->right;

    //修改向上部分
    if (node) {
        node->label = nowPt->label;
        node->parent = nowPt->parent;
    }
    if (nowPt->parent) {
        if (nowPt->isRightLeafLabel())
            nowPt->parent->right = node;
        else
            nowPt->parent->left = node;
        fixDel(nowPt->parent, nowPt->label);
    }else
        root = node;
    delete nowPt;
    return 0;
}

Block_AVLTree * AVLTree::pop(int key){
    Block_AVLTree * nowPt = find(key);
    if (!nowPt)
        return nowPt;
    Block_AVLTree * res = nowPt;
    int label = nowPt->label;
    nowPt = nowPt->parent;
    if (label == RightLeafLabel)
        nowPt->right = nullptr;
    else
        nowPt->left = nullptr;

    fixDel(nowPt, label);
    return res;
}

void AVLTree::visualize(int indent = 4){
    if (!root)
        return;
    Block_AVLTree * nowPt = root;
    int now = 0, next = 1, depth = calDepth(nowPt) - 1, num = 1;
    std::cout << std::endl << "******************************" << std::endl << depth;
    indent *= (int)pow(2, depth);
    visualizeBlock(nowPt, indent, queue[next]);
    std::cout << std::endl;
    while(depth--) {
        //change to fit the next layer
        now = 1 - now;
        next = 1 - next;
        num *= 2;
        //the first element of the new layer
        nowPt = queue[now][0];
        queue[now].pop_front();
        std::cout << depth;
        visualizeBlock(nowPt, indent / 2, queue[next]);
        //the rest element but the last one
        for (int i = num; --i > 0; ) {
            nowPt = queue[now][0];
            queue[now].pop_front();
            visualizeBlock(nowPt, indent, queue[next]);
        }
        std::cout << std::endl;
        indent /= 2;
    }
    queue[0].clear();
    queue[1].clear();
    std::cout << "******************************" << std::endl << std::endl;
}
void AVLTree::visualizeBlock(Block_AVLTree *node, int indent, std::deque<Block_AVLTree *> &queue){
    if (node) {
        std::cout << std::setw(indent) << node->key;
        queue.push_back(node->left);
        queue.push_back(node->right);
    }else {
        std::cout << std::setw(indent) << '*';
        queue.push_back(nullptr);
        queue.push_back(nullptr);
    }
}

inline int AVLTree::calDepth(Block_AVLTree * node){
        if (node)
            return std::max(node->RDepth, node->LDepth) + 1;
        else
            return -1;
    }

//修正分支深度并检查当前节点是否平衡，（若需要）修正上部节点对应分支深度并向上传递检查
//label为当前节点被改动的分支的标志
void AVLTree::fixDel(Block_AVLTree * node, int label){
    while(node){
//        std::cout << "fixDel"<<std::endl;
//        visualize(2);
//        std::cout <<std::endl;
        //下层左右分支不同处理
        if (label == LeftLeafLabel){
            node->LDepth --;
            if (node->LDepth + 1 == node->RDepth)
                break;
            if (node->LDepth + 2 == node->RDepth){
                node = fixR(node);
                if (node->parent) {
                    if (node->isRightLeafLabel()) {
                        if (calDepth(node) == node->parent->RDepth)
                            break;
                    } else {
                        if (calDepth(node) == node->parent->LDepth)
                            break;
                    }
                }
            }
        }else{
            node->RDepth --;
            if (node->RDepth + 1 == node->LDepth)
                break;
            if (node->RDepth + 2 == node->LDepth){
                node = fixL(node);
                if (node->parent) {
                    if (node->isRightLeafLabel()) {
                        if (calDepth(node) == node->parent->RDepth)
                            break;
                    } else {
                        if (calDepth(node) == node->parent->LDepth)
                            break;
                    }
                }
            }
        }
        //下层左右分支共通处理——上层深度修正
        label = node->label;
        node = node->parent;
    }
}
Block_AVLTree * AVLTree::fix(Block_AVLTree * node){
    if (node->RDepth == node->LDepth + 2)
        return fixR(node);
    if (node->LDepth == node->RDepth + 2)
        return fixL(node);
}
Block_AVLTree * AVLTree::fixR(Block_AVLTree * node){
    if(node->right->RDepth < node->right->LDepth)
        return fixRL(node);
    else
        return fixRR(node);
}
Block_AVLTree * AVLTree::fixL(Block_AVLTree * node){
    if(node->left->LDepth < node->left->RDepth)
        return fixLR(node);
    else
        return fixLL(node);
}
Block_AVLTree * AVLTree::fixRR(Block_AVLTree * node){
    if (node->parent) {
        if (node->isRightLeafLabel())
            node->parent->right = node->right;
        else
            node->parent->left = node->right;
    }else{
        root = node->right;
    }
    node->right->parent = node->parent;
    node->parent = node->right;
    node->right = node->parent->left;
    node->parent->left = node;

    node->parent->label = node->label;
    node->label = LeftLeafLabel;
    if (node->right)
        node->right->label = RightLeafLabel;
    //re-cal the depth
    node->RDepth = node->parent->LDepth;
    node->parent->LDepth = calDepth(node);

    return node->parent;
}
Block_AVLTree * AVLTree::fixLL(Block_AVLTree * node){
    //左一改为node，node的父节点改为左一,左一改为左一的右一，同时自己变为原左一的右一
    if (node->parent) {
        if (node->isRightLeafLabel())
            node->parent->right = node->left;
        else
            node->parent->left = node->left;
    }else{
        root = node->left;
    }
    node->left->parent = node->parent;
    node->parent = node->left;
    node->left = node->parent->right;
    node->parent->right = node;

    node->parent->label = node->label;
    node->label = RightLeafLabel;
    if (node->left)
        node->left->label = LeftLeafLabel;
    //re-cal the depth
    node->LDepth = node->parent->RDepth;
    node->parent->RDepth = calDepth(node);

    return node->parent;
}
Block_AVLTree * AVLTree::fixRL(Block_AVLTree * node){
    //1;1l 2;3 2r;3l 3r;
    //3;1 2;1l 3l 3r 2r
    if (node->parent) {
        if (node->isRightLeafLabel())
            node->parent->right = node->right->left;
        else
            node->parent->left = node->right->left;
    }else{
        root = node->left->right;
    }
    node->right->left->parent = node->parent;
    node->parent = node->right->left;
    node->right->left = node->parent->right;
    node->parent->right = node->right;
    node->right = node->parent->left;
    node->parent->left = node;

    node->parent->label = node->label;
    //switch node
    node = node->parent;
    node->right->label = RightLeafLabel;
    node->left->label = LeftLeafLabel;
    if (node->left->right)
        node->left->right->label = RightLeafLabel;
    if (node->right->left)
        node->right->left->label = LeftLeafLabel;

    //re-cal the depth
    node->right->LDepth = node->RDepth;
    node->left->RDepth = node->LDepth;
    node->RDepth = calDepth(node->right);
    node->LDepth = calDepth(node->left);

    return node;
}
Block_AVLTree * AVLTree::fixLR(Block_AVLTree * node){
    //1; 2 1r;2l 3; 3l 3r
    //3; 2 1; 2l 3l 3r 1r
    if (node->parent) {
        if (node->isRightLeafLabel())
            node->parent->right = node->left->right;
        else
            node->parent->left = node->left->right;
    }else{
        root = node->left->right;
    }
    node->left->right->parent = node->parent;
    node->parent = node->left->right;
    node->left->right = node->parent->left;
    node->parent->left = node->left;
    node->left = node->parent->right;
    node->parent->right = node;

    node->parent->label = node->label;
    // switch node to the pre LR node
    node = node->parent;
    node->right->label = RightLeafLabel;
    if (node->left->right)
        node->left->right->label = RightLeafLabel;
    if (node->right->left)
        node->right->left->label = LeftLeafLabel;

    //re-cal the depth
    node->right->LDepth = node->RDepth;
    node->left->RDepth = node->LDepth;
    node->RDepth = calDepth(node->right);
    node->LDepth = calDepth(node->left);

    return node;
}
