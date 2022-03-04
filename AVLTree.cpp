#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <math.h>

template <class T>
struct AVLNode {
    AVLNode *left;
    AVLNode *right;

    // Height of the node
    int height;
    // Number of nodes in the subtree
    const T value;

    AVLNode(T value);
    int balanceFactor();
    void updateValues();

    AVLNode *leftRotate();
    AVLNode *rightRotate();
};

template <class T>
AVLNode<T>::AVLNode(T value): value(value) {
    this->height = 1;

    this->left = nullptr;
    this->right = nullptr;
}

template <class T>
void AVLNode<T>::updateValues() {
    // The height of the node based on either left or right subtree
    this->height = std::max(
        left != nullptr ? left->height : 0,
        right != nullptr ? right->height : 0
    ) + 1;
}

template <class T>
int AVLNode<T>::balanceFactor() {
    return (
        left != nullptr ? left->height : 0
    ) - (right != nullptr ? right->height: 0);
}

template <class T>
AVLNode<T>* AVLNode<T>::leftRotate() {
    // Swap left and right
    AVLNode *r = right;
    right = right->left;
    r->left = this;

    // Update the rotated nodes so the node attributes are reflected
    this->updateValues();
    r->updateValues();

    return r;
}

template <class T>
AVLNode<T>* AVLNode<T>::rightRotate() {
    // Swap right and left
    AVLNode *l = left;
    left = left->right;
    l->right = this;

    // Update the rotated nodes so the node attributes are reflected
    this->updateValues();
    l->updateValues();

    return l;
}

template <class T>
class AVLTree {
    // Root of the AVLTree
    AVLNode<T> *root;
    void balance(std::vector<AVLNode<T>**> path);
public:
    AVLTree();
    ~AVLTree();

    bool find(T value) const;
    void insert(T value);
};

template <class T>
AVLTree<T>::AVLTree() {
    root = nullptr;
}

template <class T>
AVLTree<T>::~AVLTree() {
    /*
        Cleanup routine

        Starting from the root we traverse through the tree
        starting from the root and stash the AVLNode in the
        vector stack and delete the node.
    */
    std::vector<AVLNode<T>*> stack;

    if (root != nullptr)
        stack.push_back(root);

    while (!stack.empty()) {
        AVLNode<T> *node = stack.back();
        stack.pop_back();

        if (node->left != nullptr)
            stack.push_back(node->left);
        if (node->right != nullptr)
            stack.push_back(node->right);
        
        delete node;
    }

    root = nullptr;
}

template <class T>
void AVLTree<T>::insert(T value) {
    /*
        Insertion routine

        Starting from the root we're traversing to the intended branch
        of the subtree. And we'll only be modifying the affected
        branch and subtree for the updates and rebalance.
    */

    // Current pointing to the address of root node.
    AVLNode<T> **current = &root;
    // A stack of nodes that is being traversed to find the location for the new node
    std::vector<AVLNode<T>**> stack; 

    while (*current != nullptr) {
        stack.push_back(current);

        /*
            Traverse the branch and assign current to the intended
            next node address until we hit bottom.

            current will point to the address of the either left/right 
            node child pointer address
        */
        if ((*current)->value > value) {
            current = &((*current)->left);
        } else {
            current = &((*current)->right);
        }
    }

    // Create the new node and push it to the stack
    *current = new AVLNode<T>(value);
    stack.push_back(current);

    // Rebalance the affected nodes and its subtrees
    std::reverse(stack.begin(), stack.end());
    this->balance(stack);
}

template <class T>
void AVLTree<T>::balance(std::vector<AVLNode<T>**> stack) {
    for (AVLNode<T> **current: stack) {
        // Update the height value for the traversed nodes
        (*current)->updateValues();
        
        // left - left rotation
        if ((*current)->balanceFactor() >= 2 && (*current)->left->balanceFactor() >= 1) {
            *current = (*current)->rightRotate();
        }
        // left - right rotation 
        else if ((*current)->balanceFactor() >= 2) {
            (*current)->left = (*current)->left->leftRotate();
            *current = (*current)->rightRotate();
        }
        // right - right rotation
        else if ((*current)->balanceFactor() <= -2 && (*current)->right->balanceFactor() <= -1) {
            *current = (*current)->leftRotate();
        }
        // right - left rotation
        else if ((*current)->balanceFactor() <= -2) {
            (*current)->right = ((*current)->right)->rightRotate();
            *current = (*current)->leftRotate();
        }
    }
}

/*
    Why we dereference so much?

    It's because we want the value of the pointer. `Current` at the moment
    is the pointer to the address of the pointer. So we want the actual
    pointer and modify the value from stack.
*/

template <class T>
bool AVLTree<T>::find(T value) const {
    AVLNode<T> *current = root;

    while (current != nullptr && current->value != value) {
        if (current->value > value)
            current = current->left;
        else {
            current = current->right;
        }
    }

    return (current != nullptr) ? true : false;
}