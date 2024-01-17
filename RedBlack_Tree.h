//   File: Red_Black_Tree.h
//   Date: January 13, 2024
// Author: David West
//   Desc: Red-Black Tree data structure declarations and definitions.
// ---------------------------------------------------------------------

#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

#include "Queue.h" // Used in breadth-first findValue

#include <iostream>
using std::cout;

template <typename K, typename V>
class RedBlackTree {
public:
    enum { BLACK, RED };
private:
    struct RedBlackNode {
        K key;
        V value;
        bool color;
        RedBlackNode* parent;
        RedBlackNode* leftChild;
        RedBlackNode* rightChild;

    // Constructors
        // Key and value are both known on construction
        RedBlackNode(const K & key, const V & val, bool c = RED,
                              RedBlackNode* p = nullptr, RedBlackNode* l = nullptr, RedBlackNode* r = nullptr)
                : key{key}, value{val}, parent{p}, leftChild{l}, rightChild{r}, color{c} {}
        RedBlackNode(const K & key, V && val, bool c = RED,
                              RedBlackNode* p = nullptr, RedBlackNode* l = nullptr, RedBlackNode* r = nullptr)
                : key{key}, value{std::move(val)}, parent{p},leftChild{l}, rightChild{r}, color{c} {}
        // Value is to be assigned after construction
        explicit RedBlackNode(const K & key, bool c = RED,
                              RedBlackNode* p = nullptr, RedBlackNode* l = nullptr, RedBlackNode* r = nullptr)
                : key{key}, parent{p}, leftChild{l}, rightChild{r}, color{c} {}

    };  // End RedBlackNode

    RedBlackNode* treeRoot;
    int _size;

public:
    // Constructors
    RedBlackTree();
    ~RedBlackTree();

    // Tree Management Methods
    void insert(const K & key, const V & value);
    V& cursedInsert(const K & key); // Used in [] operator overloading
    bool remove(const K & key);
    V& findValue(const K & key);
    K findKey(const V & value);
    int size();

    // Traversal Methods
    void preOrderTraverse();
    void inOrderTraverse();
    void postOrderTraverse();
    void breadthFirstTraverse();

private:
    // Tree Management Methods
    V* _insert(RedBlackNode* & parent, RedBlackNode* & newNode);
    bool _remove(const K & key, RedBlackNode* node);
    K _findMinValue(RedBlackNode* root);

    // Traversal Methods
    void _preOrderTraverse(RedBlackNode* & root, void(*operation)(RedBlackNode*));
    void _inOrderTraverse(RedBlackNode* & root, void(*operation)(RedBlackNode*));
    void _postOrderTraverse(RedBlackNode* & root, void(*operation)(RedBlackNode*));
    void _breadthFirstTraverse(RedBlackNode* root, void(*operation)(RedBlackNode*));

    // Traversal Operations
    static void _deleteNode(RedBlackNode* node);
    static void _printValue(RedBlackNode* node);
    static void _calcHeight(RedBlackNode* node);

    // Re-balancing
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
template <typename K, typename V>
RedBlackTree<K,V>::RedBlackTree() {
    treeRoot = nullptr;
    _size = 0;
}


/**
 * Destructor
 */
template <typename K, typename V>
RedBlackTree<K,V>::~RedBlackTree() {
    _postOrderTraverse(treeRoot, & RedBlackTree::_deleteNode);
}


// ---------------------------------------------------------------------
//                  Public Tree Management Methods

/**
 * Adds a (key, value) pair to the tree.
 * @param data (int) - Value to be added.
 */
template <typename K, typename V>
void RedBlackTree<K,V>::insert(const K & key, const V & value) {
    auto* newNode = new RedBlackNode(key, value);
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
 * Insertion for CursedArray [] operation overloading.
 * Checks if a node exists for reassignment, and if not inserts a node without a value for later assignment.
 * Using this function for insertion prevents having to traverse twice for insertions
 * (once to determine if the value already exists and again to insert a new value).
 * @param key Key/Index to determine placement in tree. Must be comparable.
 * @return Returns a reference to the node's value for either the node with either the matched key or a new key.
 */
template <typename K, typename V>
V& RedBlackTree<K,V>::cursedInsert(const K & key) {

    if (!treeRoot) {
        auto* newNode = new RedBlackNode(key);
        // First node to be inserted into the tree
        treeRoot = newNode;
        treeRoot->color = BLACK;
        ++_size;
        return treeRoot->value;
    }

    // Tree root exists, traverse to the appropriate node or leaf
    RedBlackNode* currentNode = treeRoot;
    RedBlackNode* parentNode;
    bool nodeExists = false;

    while (currentNode and !nodeExists) {
        if (currentNode->key == key) {
            nodeExists = true;
            parentNode = currentNode->parent;

        } else if (currentNode->key > key) {
            parentNode = currentNode;
            currentNode = currentNode->leftChild;

        } else {
            parentNode = currentNode;
            currentNode = currentNode->rightChild;
        }
    }

    // Key is in the tree, overwrite its value
    if (currentNode and currentNode->key == key) {
        return currentNode->value;
    }

    // Key is not in the tree, add as a leaf
    auto* newNode = new RedBlackNode(key);
    newNode->parent = parentNode;

    if (key < parentNode->key) {
        parentNode->leftChild = newNode;
    } else {
        parentNode->rightChild = newNode;
    }
    ++_size;
    cout << _size;
    return newNode->value;
}


/**
 * Finds the value stored at a key is in the tree.
 * @param key (K) - Key to find.
 * @return - Returns a pointer to the value stored at a key.
 */
template <typename K, typename V>
V& RedBlackTree<K,V>::findValue(const K & key) {
    RedBlackNode* currentNode = treeRoot;
    bool nodeExists = false;

    while (currentNode and !nodeExists) {
        if (currentNode->key == key) {
            nodeExists = true;

        } else if (currentNode->key > key) {
            currentNode = currentNode->leftChild;

        } else {
            currentNode = currentNode->rightChild;
        }
    }

    if (!currentNode)   // Reached the end of a branch and currentNode is a nullptr
        return nullptr;

    return &currentNode->value;

} // End findValue()


/**
 * Finds whether a value is in the tree.
 * @param value (V) - Value to search for
 * @return - Returns the key for the value.
 */
template <typename K, typename V>
K RedBlackTree<K,V>::findKey(const V & value) {

} // End findValue()


/**
 * Removes a value from the tree.
 * @param value - Value to be found and removed.
 * @return - True if value existed within the tree, false if the value did not exist.
 */
template <typename K, typename V>
bool RedBlackTree<K,V>::remove(const K & key) {
    return _remove(key, treeRoot);
}


// ---------------------------------------------------------------------
//                  Private Tree Management Methods

/**
 * Private function to add a new node with a given value to the tree.
 * Called by the public insert function.
 * Duplicates are added in the left child.
 * @param value <V> - Value of the node to add.
 * @param root - Value of the current traversal node.
 * @return Returns a pointer to the current node.
 */
template <typename K, typename V>
V* RedBlackTree<K,V>::_insert(RedBlackNode* & parent, RedBlackNode* & newNode) {
    V* valuePtr;
        // Key > parent key, go right
    if (newNode->key > parent->key) {

        if (!parent->rightChild) {      // Parent does not have a right child
            parent->rightChild = newNode;
            valuePtr = &newNode->value;
            newNode->parent = parent;
            ++_size;
        }
        else {      // Parent has a right child, go to it

            valuePtr = _insert(parent->rightChild, newNode);
        }

    } // end new key > parent key

        // key <= parent key, go left
    else if (newNode->key < parent->key){
        if (!parent->leftChild) {   // Parent does not have a left child
            parent->leftChild = newNode;
            valuePtr = &newNode->value;
            newNode->parent = parent;
            ++_size;
        }
        else {      // Parent has a left child, go to it
            valuePtr = _insert(parent->leftChild, newNode);
        }
    }   // end new key <= parent key

    _checkColor(newNode);

    return valuePtr;
}   // end _insert()


/**
 * Recursive function to find and delete a key from the tree.
 * @param key - Value to be found and removed.
 * @param node - RedBlackNode* to the root of a tree or subtree.
 * @param isFound - Bool to be returned by calling function for whether key was found within the tree.
 * @return Returns true if the key to remove was found.
 */
template <typename K, typename V>
bool RedBlackTree<K,V>::_remove(const K & key, RedBlackNode* node) {
    // Base case 1: End of findValue, key not in tree
    if (!node)
        return false;

    // Base case 2: Value found at this node.
    // 3 Sub-cases: 1) Value found in a parent of 2 children
    //              2) Value found in a parent of 1 child
    //              3) Value found in a leaf
    if (node->key == key) {
        if (node->leftChild and node->rightChild) {      // Parent of 2 children
            node->key = _findMinValue(node->rightChild);
            _remove(node->key, node->rightChild);

        } else if (node->leftChild or node->rightChild) {        // Parent of 1 child
            RedBlackNode* tempNode = node;
            node = (node->leftChild) ? node->leftChild : node->rightChild;

            node->parent = tempNode->parent;

            if (tempNode->parent->leftChild == tempNode)
                tempNode->parent->leftChild = node;
            else
                tempNode->parent->rightChild = node;

            delete tempNode;
            --_size;

        } else {    // Leaf
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

    // Recursive case: Value higher or lower than current node's key
    if (key < node->key) {
        return _remove(key, node->leftChild);
    } else {
        return _remove(key, node->rightChild);
    }

} // End _remove()


/**
 * Finds the minimum value in a tree or subtree of a given root.
 * @param root - Root of the tree or subtree to be searched.
 * @return Returns the minimum value.
 */
template <typename K, typename V>
K RedBlackTree<K,V>::_findMinValue(RedBlackNode* root ) {
    // Base case 1: Tree does not exist
    if (!root)
        return -1;

    // Base case 2: RedBlackNode has no left child, found min node
    if (!root->leftChild) {
        return root->key;
    }

    // Recursive case: RedBlackNode has a left child
    return _findMinValue(root->leftChild);

} // End _findMinValue


/**
 *
 * @tparam K Key for tree positioning/balancing.
 * @tparam V Value stored at given key.
 * @return Returns the number of elements in the array.
 */
template <typename K, typename V>
inline int RedBlackTree<K,V>::size() {
    return _size;
}


// ---------------------------------------------------------------------
//                       Public Traversal Methods

/**
 * Traverses tree or subtree in the order of:
 * 1. Visit root node
 * 2. Visit left subtree
 * 3. Visit right subtree
 **/
template <typename K, typename V>
void RedBlackTree<K,V>::preOrderTraverse() {
    _preOrderTraverse(treeRoot, &_printValue);
}


/**
 * Traverses tree or subtree in the order of:
 * 1. Visit left subtree
 * 2. Visit root node
 * 3. Visit right subtree
 */
template <typename K, typename V>
void RedBlackTree<K,V>::inOrderTraverse() {
    _inOrderTraverse(treeRoot, &_printValue);
}


/**
 * Traverses tree or subtree in the order of:
 * 1. Visit left subtree
 * 2. Visit right subtree
 * 3. Visit root node
 */
template <typename K, typename V>
void RedBlackTree<K,V>::postOrderTraverse() {
    _postOrderTraverse(treeRoot, &_printValue);
}


/**
 * Traverses the tree or subtree from in order of depth, from left to right.
 */
template <typename K, typename V>
void RedBlackTree<K,V>::breadthFirstTraverse() {
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
template <typename K, typename V>
void RedBlackTree<K,V>::_preOrderTraverse(RedBlackNode* & root, void(*operation)(RedBlackNode*)) {
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
template <typename K, typename V>
void RedBlackTree<K,V>::_inOrderTraverse(RedBlackNode* & root, void(*operation)(RedBlackNode*)) {
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
template <typename K, typename V>
void RedBlackTree<K,V>::_postOrderTraverse(RedBlackNode* & root, void(*operation)(RedBlackNode*)) {
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
template <typename K, typename V>
void RedBlackTree<K,V>::_breadthFirstTraverse(RedBlackNode* root, void(*operation)(RedBlackNode*)) {
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
template <typename K, typename V>
void RedBlackTree<K,V>::_deleteNode(RedBlackNode* node) {
    delete node;
}


/**
 * Prints the value of node to console.
 * @param node - The node whose value will be printed to console.
 */
template <typename K, typename V>
void RedBlackTree<K,V>::_printValue(RedBlackNode* node) {
    cout << "( " << node->value << " | " << (node->color ? "R" : "B") << " ) ";
}


/**
 * Finds and sets the height of a node. Used in a breadth first findValue for AVL re-balancing.
 * @param node - RedBlackNode whose height needs to be evaluated.
 */
template <typename K, typename T>
void RedBlackTree<K,T>::_calcHeight(RedBlackNode* node) {

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
//                        Red-Black Re-balancing


/**
 *
 * @tparam T
 * @param node
 */
template <typename K, typename V>
void RedBlackTree<K,V>::_leftRotate(RedBlackNode* node) {
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
template <typename K, typename V>
void RedBlackTree<K,V>::_rightRotate(RedBlackNode* node) {
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
template <typename K, typename V>
void RedBlackTree<K,V>::_leftRightRotate(RedBlackNode* node) {
    _leftRotate(node->leftChild);
    _rightRotate(node);
}


/**
 *
 * @tparam T
 * @param node
 */
template <typename K, typename V>
void RedBlackTree<K,V>::_rightLeftRotate(RedBlackNode* node) {
    _rightRotate(node->rightChild);
    _leftRotate(node);
}


/**
 *
 * @tparam T
 * @param node
 */
template <typename K, typename V>
void RedBlackTree<K,V>::_rotate(RedBlackNode* node) {
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
template <typename K, typename V>
void RedBlackTree<K,V>::_correctTree(RedBlackNode* node) {
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

        // Aunt findKey (not always true, can be nullptr) and is red
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
template <typename K, typename V>
void RedBlackTree<K,V>::_checkColor(RedBlackNode* node) {
    if (node == treeRoot)   // Tree root has no parents and thus no conflicts.
        return;

    if (node->color == RED && node->parent->color == RED) { // Breaks 2 adjacent red node rule
        _correctTree(node);
    }

    _checkColor(node->parent);
    treeRoot->color = BLACK;
} // End _checkColor()


#endif //REDBLACKTREE_H
