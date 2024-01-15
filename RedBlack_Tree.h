//   File: Red_Black_Tree.h
//   Date: January 13, 2024
// Author: David West
//   Desc: Red-Black Tree data structure declarations and definitions.
// ---------------------------------------------------------------------

#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

#include "Queue.h" // Used in breadth-first search

#include <iostream>
using std::cout;

template<typename T>
class RedBlackTree {
public:
    enum { BLACK, RED };
private:
    struct RedBlackNode {
        T value;
        bool color;
        RedBlackNode* parent;
        RedBlackNode* leftChild;
        RedBlackNode* rightChild;

        // Constructors
        explicit RedBlackNode(const T & val, RedBlackNode* p = nullptr, RedBlackNode* l = nullptr, RedBlackNode* r = nullptr, bool c = RED)
                : value{val}, parent{p}, leftChild{l}, rightChild{r}, color{c} {}
        explicit RedBlackNode(T && val, RedBlackNode* p = nullptr, RedBlackNode* l = nullptr, RedBlackNode* r = nullptr, bool c = RED)
                : value{std::move(val)}, parent{p},leftChild{l}, rightChild{r}, color{c} {}

    };  // End RedBlackNode

    RedBlackNode* treeRoot;
    int _size;

public:
    // Constructors
    RedBlackTree();
    ~RedBlackTree();

    // Tree Management Methods
    void insert(const T & value);
    bool remove(const T & value);
    T search(const T & value);
    bool exists(const T & value);

    // Traversal Methods
    void preOrderTraverse();
    void inOrderTraverse();
    void postOrderTraverse();
    void breadthFirstTraverse();

private:
    // Tree Management Methods
    void _insert(RedBlackNode* & parent, RedBlackNode* & newNode);
    bool _remove(const T & value, RedBlackNode* node);

    int _findMinValue(RedBlackNode* root);
    int _height(RedBlackNode* node);

    // Traversal Methods
    void _preOrderTraverse(RedBlackNode* & root, void(*operation)(RedBlackNode*));
    void _inOrderTraverse(RedBlackNode* & root, void(*operation)(RedBlackNode*));
    void _postOrderTraverse(RedBlackNode* & root, void(*operation)(RedBlackNode*));
    void _breadthFirstTraverse(RedBlackNode* root, void(*operation)(RedBlackNode*));

    // Traversal Operations
    static void _deleteNode(RedBlackNode* node);
    static void _printValue(RedBlackNode* node);
    static void _calcHeight(RedBlackNode* node);

    // AVL Re-balancing
    void _leftRotate(RedBlackNode* node);
    void _rightRotate(RedBlackNode* node);
    void _leftRightRotate(RedBlackNode* node);
    void _rightLeftRotate(RedBlackNode* node);

    void _rotate(RedBlackNode* node);
    void _correctTree(RedBlackNode* node);
    void _checkColor(RedBlackNode* node);
};


// ---------------------------------------------------------------------
//                          Constructors

/**
 * Default constructor
 */
template<typename T>
RedBlackTree<T>::RedBlackTree() {
    treeRoot = nullptr;
    _size = 0;
}


/**
 * Destructor
 */
template<typename T>
RedBlackTree<T>::~RedBlackTree() {
    _postOrderTraverse(treeRoot, & RedBlackTree::_deleteNode);
}


// ---------------------------------------------------------------------
//                  Public Tree Management Methods

/**
 * Adds a value to the tree.
 * @param data (int) - Value to be added.
 */
template<typename T>
void RedBlackTree<T>::insert(const T & value) {
    auto* newNode = new RedBlackNode(value);

    if (!treeRoot) {
        // First node to be inserted into the tree
        treeRoot = newNode;
        treeRoot->color = BLACK;
        ++_size;
        return;
    }

        // A tree root exists
    _insert(treeRoot, newNode);

} // End insert()


/**
 * Finds whether a value exists in the tree.
 * @param value (T) - Value to search for.
 * @return - Returns true if value exists in the tree, false if not in the tree.
 */
template<typename T>
T RedBlackTree<T>::search(const T & value) {
    RedBlackNode* currentNode = treeRoot;
    bool nodeExists = false;

    while (currentNode and !nodeExists) {

        if (currentNode->value == value) {
            nodeExists = true;

        } else if (currentNode->value < value) {
            currentNode = currentNode->leftChild;

        } else {
            currentNode = currentNode->rightChild;
        }
    }

    return currentNode->value;

} // End search()


/**
 * Finds whether a value exists in the tree.
 * @param value (T) - Value to search for.
 * @return - Returns true if value exists in the tree, false if not in the tree.
 */
template<typename T>
bool RedBlackTree<T>::exists(const T & value) {
    RedBlackNode* currentNode = treeRoot;
    bool nodeExists = false;

    while (currentNode and !nodeExists) {

        if (currentNode->value == value) {
            nodeExists = true;

        } else if (currentNode->value < value) {
            currentNode = currentNode->leftChild;

        } else {
            currentNode = currentNode->rightChild;
        }
    }

    return nodeExists;

} // End search()


/**
 * Removes a value from the tree.
 * @param value - Value to be found and removed.
 * @return - True if value existed within the tree, false if the value did not exist.
 */
template<typename T>
bool RedBlackTree<T>::remove(const T & value) {
    return _remove(value, treeRoot);
}


// ---------------------------------------------------------------------
//                  Private Tree Management Methods

/**
 * Private function to add a new node with a given value to the tree.
 * Called by the public insert function.
 * Duplicates are added in the left child.
 * @param value <T> - Value of the node to add.
 * @param root - Value of the current traversal node.
 * @return Returns a pointer to the current node.
 */
template<typename T>
void RedBlackTree<T>::_insert(RedBlackNode* & parent, RedBlackNode* & newNode) {
        // Value > parent value, go right
    if (newNode->value > parent->value) {

        if (!parent->rightChild) {      // Parent does not have a right child
            parent->rightChild = newNode;
            newNode->parent = parent;
            ++_size;
        }
        else {      // Parent has a right child, go to it
            _insert(parent->rightChild, newNode);
        }

    } // end new value > parent value

        // Value <= parent value, go left
    else {
        if (!parent->leftChild) {   // Parent does not have a left child
            parent->leftChild = newNode;
            newNode->parent = parent;
            ++_size;
        }
        else {      // Parent has a left child, go to it
            _insert(parent->leftChild, newNode);
        }
    }   // end new value <= parent Value

    _checkColor(newNode);

}   // end _insert()


/**
 * Recursive function to find and delete a value from the tree.
 * @param value - Value to be found and removed.
 * @param node - RedBlackNode* to the root of a tree or subtree.
 * @param isFound - Bool to be returned by calling function for whether value was found within the tree.
 * @return Returns true if the value to remove was found.
 */
template<typename T>
bool RedBlackTree<T>::_remove(const T & value, RedBlackNode* node) {
    // Base case 1: End of search, value not in tree
    if (!node)
        return false;

    // Base case 2: Value found at this node.
    if (node->value == value) {
        if (node->leftChild and node->rightChild) {      // Value was found in a parent of two children
            node->value = _findMinValue(node->rightChild);
            _remove(node->value, node->rightChild);

        } else if (node->leftChild or node->rightChild) {        // Value was found in a parent of one child
            RedBlackNode* tempNode = node;
            node = (node->leftChild) ? node->leftChild : node->rightChild;

            node->parent = tempNode->parent;

            if (tempNode->parent->leftChild == tempNode)
                tempNode->parent->leftChild = node;
            else
                tempNode->parent->rightChild = node;

            delete tempNode;
            --_size;
        } else {    // Value was found in a terminal node
            RedBlackNode* parent = node->parent;

            if (parent->leftChild == node)
                parent->leftChild = nullptr;
            else
                parent->rightChild = nullptr;

            delete node;
            --_size;
            node = parent;
        }

        _checkColor(node);

        return true;
    }

    // Recursive case: Value higher or lower than current node's value
    if (value < node->value) {
        return _remove(value, node->leftChild);
    } else {
        return _remove(value, node->rightChild);
    }

} // End _remove()


/**
 * Finds the minimum value in a tree or subtree of a given root.
 * @param root - Root of the tree or subtree to be searched.
 * @return Returns the minimum value.
 */
template<typename T>
int RedBlackTree<T>::_findMinValue(RedBlackNode* root ) {
    // Base case 1: Tree does not exist
    if ( !root )
        return -1;

    // Base case 2: RedBlackNode has no left child, found min node
    if ( !(root -> leftChild ) ) {
        return root -> value;
    }

    // Recursive case: RedBlackNode has a left child
    return _findMinValue( root -> leftChild );

} // End _findMinValue


/**
 * Returns the height of a node. Used in AVL balancing.
 * @param node - RedBlackNode whose height we're interested in.
 * @return (int) - Height of the node, -1 if the node doesn't exist.
 */
template<typename T>
inline int RedBlackTree<T>::_height(RedBlackNode* node) {
    return (node) ? node->height : -1;
} // End _height()


// ---------------------------------------------------------------------
//                       Public Traversal Methods

/**
 * Traverses tree or subtree in the order of:
 * 1. Visit root node
 * 2. Visit left subtree
 * 3. Visit right subtree
 **/
template<typename T>
void RedBlackTree<T>::preOrderTraverse() {
    _preOrderTraverse(treeRoot, &_printValue);
}


/**
 * Traverses tree or subtree in the order of:
 * 1. Visit left subtree
 * 2. Visit root node
 * 3. Visit right subtree
 */
template<typename T>
void RedBlackTree<T>::inOrderTraverse() {
    _inOrderTraverse(treeRoot, &_printValue);
}


/**
 * Traverses tree or subtree in the order of:
 * 1. Visit left subtree
 * 2. Visit right subtree
 * 3. Visit root node
 */
template<typename T>
void RedBlackTree<T>::postOrderTraverse() {
    _postOrderTraverse(treeRoot, &_printValue);
}


/**
 * Traverses the tree or subtree from in order of depth, from left to right.
 */
template<typename T>
void RedBlackTree<T>::breadthFirstTraverse() {
    _breadthFirstTraverse(treeRoot, &_printValue);

} // End breadthFirstTraverse()


// ---------------------------------------------------------------------
//                       Private Traversal Methods
//           Applies an operation (function) to each node during traversal.

/**
 * Recursive method for Pre-Order traversal: root -> left -> right
 * @param root - The root of the tree or subtree to be traversed.
 * @param operation - Function to execute on each node.
 */
template<typename T>
void RedBlackTree<T>::_preOrderTraverse(RedBlackNode* & root, void(*operation)(RedBlackNode*)) {
    // Do something
    operation(root);

    // Visit left child
    if (root->leftChild)
        _preOrderTraverse(root->leftChild , &_printValue);

    // Visit right child
    if (root->rightChild)
        _preOrderTraverse(root->rightChild , &_printValue);

} // End _preOrderTraverse


/**
 * Recursive method for In-Order traversal: left -> root -> right
 * @param root - The root of the tree or subtree to be traversed.
 * @param operation - Function to execute on each node.
 */
template<typename T>
void RedBlackTree<T>::_inOrderTraverse(RedBlackNode* & root, void(*operation)(RedBlackNode*)) {
    // Visit left child
    if (root->leftChild)
        _inOrderTraverse(root->leftChild, &_printValue);

    // Do something
    operation(root);

    // Visit right child
    if (root->rightChild)
        _inOrderTraverse(root->rightChild, &_printValue);

} // End _inOrderTraverse()


/**
 * Recursive method for Post-Order traversal: left -> right -> root
 * @param root - The root of the tree or subtree to be traversed.
 * @param operation - Function to execute on each node.
 */
template<typename T>
void RedBlackTree<T>::_postOrderTraverse(RedBlackNode* & root, void(*operation)(RedBlackNode*)) {
    // Visit left child
    if (root->leftChild)
        _postOrderTraverse(root->leftChild, operation);

    // Visit right child
    if ( root -> rightChild)
        _postOrderTraverse(root->rightChild, operation);

    // Do something
    operation(root);

} // End _postOrderTraverse()


/**
 * Traverses the tree or subtree from in order of depth, from left to right, but recursively.
 * @param root - Root of tree or subtree to evaluate.
 * @param operation - Pointer to a function to perform at each node.
 */
template <typename T>
void RedBlackTree<T>::_breadthFirstTraverse(RedBlackNode* root, void(*operation)(RedBlackNode*)) {
    Queue<RedBlackNode*> unvisitedNodes;
    unvisitedNodes.enqueue(root);

    while(!unvisitedNodes.isEmpty()) {
        RedBlackNode* currentNode = unvisitedNodes.deque();

        if (currentNode->leftChild) { unvisitedNodes.enqueue(currentNode->leftChild); }
        if (currentNode->rightChild) { unvisitedNodes.enqueue(currentNode->rightChild); }

        operation(currentNode);
    }

} // End _breadthFirstTraverse()


// ---------------------------------------------------------------------
//                       Traversal Operations

/**
 * Deletes a node.
 * @param node - The node to delete.
 */
template<typename T>
void RedBlackTree<T>::_deleteNode(RedBlackNode* node) {
    delete node;
}


/**
 * Prints the value of node to console.
 * @param node - The node whose value will be printed to console.
 */
template<typename T>
void RedBlackTree<T>::_printValue(RedBlackNode* node) {
    cout << "( " << node->value << " | " << (node->color ? "R" : "B") << " ) ";
}


/**
 * Finds and sets the height of a node. Used in a breadth first search for AVL re-balancing.
 * @param node - RedBlackNode whose height needs to be evaluated.
 */
template<typename T>
void RedBlackTree<T>::_calcHeight(RedBlackNode* node) {

    if (node->leftChild && node->rightChild)
        node->height = std::max(node->leftChild->height, node->rightChild->height) + 1;

    else if (node->leftChild)
        node->height = node->leftChild->height + 1;

    else if (node->rightChild)
        node->height = node->rightChild->height + 1;

    else
        node->height = 0;

}


// ---------------------------------------------------------------------
//                       AVL Re-balancing


/**
 *
 * @tparam T
 * @param node
 */
template <typename T>
void RedBlackTree<T>::_leftRotate(RedBlackNode* node) {
    RedBlackNode* temp = node->rightChild;
    node->rightChild = temp->leftChild;

    if (node->rightChild)
        node->rightChild->parent = node;

    temp->leftChild = node;
    node->parent = temp;

    if (!node->parent) {
        // We are the root node
        treeRoot = temp;
        temp->parent = nullptr;
    } else {
        // We are not the root node
        temp->parent = node->parent;

        if (node == temp->parent->leftChild)
            // Node is the left child of its parent
            temp->parent->leftChild = temp;
        else
            // Node is the right child of its parent
            temp->parent->rightChild = temp;


    }

}


/**
 *
 * @tparam T
 * @param node
 */
template <typename T>
void RedBlackTree<T>::_rightRotate(RedBlackNode* node) {
    RedBlackNode* temp = node->leftChild;
    node->leftChild = temp->rightChild;

    if (node->leftChild)
        node->leftChild->parent = node;

    if (!node->parent) {
        // We are the root node
        treeRoot = temp;
        temp->parent = nullptr;
    } else {
        // We are not the root node
        temp->parent = node->parent;

        if (node == node->parent->leftChild)
            // Node is the left child of its parent
            temp->parent->leftChild = temp;
        else if(node == node->parent->rightChild)
            // Node is the right child of its parent
            temp->parent->rightChild = temp;
    }

    temp->rightChild = node;
    node->parent = temp;

} // End _rotateRight


/**
 *
 * @tparam T
 * @param node
 */
template <typename T>
void RedBlackTree<T>::_leftRightRotate(RedBlackNode* node) {
    _leftRotate(node->leftChild);
    _rightRotate(node);
}


/**
 *
 * @tparam T
 * @param node
 */
template <typename T>
void RedBlackTree<T>::_rightLeftRotate(RedBlackNode* node) {
    _rightRotate(node->rightChild);
    _leftRotate(node);
}


/**
 *
 * @tparam T
 * @param node
 */
template <typename T>
void RedBlackTree<T>::_rotate(RedBlackNode* node) {
    RedBlackNode* parentNode = node->parent;
    RedBlackNode* grandparent = parentNode->parent;

    if (node == parentNode->leftChild) {    // Node is left child of parent

        if (parentNode == grandparent->leftChild) {     // Parent is left child of grandparent
            _rightRotate(grandparent);
            node->color = RED;
            parentNode->color = BLACK;
            if (parentNode->rightChild)
                parentNode->rightChild->color = RED;
            return;
        }
            // Parent is right child of grandparent
        _rightLeftRotate(grandparent);
        node->color = BLACK;    // Node is now the parent to the old parent and grandparent
        node->rightChild->color = RED;
        node->leftChild->color = RED;
        return;
    }

    // Node is the right child of parent
    if (parentNode == grandparent->rightChild) {
        // Parent is the right child of the grandparent
        _leftRotate(grandparent);
        node->color = RED;
        parentNode->color = BLACK;
        if (parentNode->leftChild)
            parentNode->leftChild->color = RED;
        return;
    }

    // Parent is the left child of the grandparent
    _leftRightRotate(grandparent);
    node->color = BLACK;
    node->rightChild->color = RED;
    node->leftChild->color = RED;

}


/**
 *
 * @tparam T
 * @param node
 */
template <typename T>
void RedBlackTree<T>::_correctTree(RedBlackNode* node) {
    RedBlackNode* parentNode = node->parent;
    RedBlackNode* grandparent = (parentNode->parent) ? grandparent = parentNode->parent : nullptr;

    if(!grandparent) {
        return;     // There is no grandparent, there is no correcting necessary
    }

        // Aunt is the grandparent's opposite child to the parent
    RedBlackNode* aunt = (parentNode == grandparent->leftChild) ? grandparent->rightChild : grandparent->leftChild;

        // Aunt doesn't exist or is black
    if (!aunt or aunt->color == BLACK) {
        _rotate(node);
        return;
    }

        // Aunt exists (not always true, can be nullptr) and is red
    if (aunt) {
        aunt->color = BLACK;
    }
    grandparent->color = RED;
    parentNode->color = BLACK;
}



/**
 *
 * @tparam T
 * @param colorCheck
 */
template <typename T>
void RedBlackTree<T>::_checkColor(RedBlackNode* node) {
    if (node == treeRoot)   // Tree root has no parents and thus no conflicts.
        return;

    if (node->color == RED && node->parent->color == RED) { // Breaks 2 adjacent red node rule
        _correctTree(node);
    }

    _checkColor(node->parent);
    treeRoot->color = BLACK;
} // End _checkColor()


#endif //REDBLACKTREE_H
