//
//
//
//
// ----------------------------------------------------------------------------

#include "CursedArray.h"


/**
 * Default constructor
 */
template<typename T>
CursedArray<T>::CursedArray() {
    _tree = new RedBlackTree<T>;
}


/**
 * Copy constructor
 */
template<typename T>
CursedArray<T>::CursedArray(CursedArray& arr) {
    _tree = arr._tree;
}


/**
 * Destructor
 */
template<typename T>
CursedArray<T>::~CursedArray() {
    delete _tree;
}


/**
 * [] operator for getting a value for a given float index.
 */
template<typename T>
const T& CursedArray<T>::operator [](float index) const {
    Item foundItem = _tree.search(index);
    return foundItem->value;
}


/*
 * [] operator for setting a value at a given float index.
 */
template<typename T>
T& CursedArray<T>::operator [](float index) {
    // Key in _tree, overwrite value
    if (_tree.exists(index)) {
        Item foundItem = _tree.search(index);

        return foundItem->value;
    }

    // Index not in _tree
    Item newItem = (index);
    _tree.insert(newItem);

    return _tree[index];

}


/*
 * [] operator for setting a value at a given float index.
 */
template<typename T>
T& CursedArray<T>::operator [](double index) {
    return CursedArray<T>::operator []((float)index);
}


/*
 * [] Operator overloaded to error if index is an int or int literal.
 */
template<typename T>
void CursedArray<T>::operator [](int index) {
    static_assert(false, "CursedArray index cannot be an integer.");
}


/*
 * Operator overload for assignment (=).
 */
template<typename T>
T & CursedArray<T>::operator =(CursedArray const & arr){
    //TODO: Assignment operator
}


/*
 * For Debugging
 * Returns the number of elements
 */
template<typename T>
int CursedArray<T>::size() {
    return _tree.size();
}


/**
 *
 * @tparam T
 * @param index
 * @return
 */
template<typename T>
bool CursedArray<T>::exists(float index) {
    return _tree.exists(index);
}


/*
 * Returns a pointer to an array of false allIndexes.
 */
template<typename T>
float* CursedArray<T>::allIndexes() {
    float *indexArray = new float[_tree.size()];

    for (int i = 0; i < _tree.size(); ++i)
        indexArray[i] = _tree[i].index;

    return indexArray;
}


/*
 * Removes an element from the array.
 */
template<typename T>
void CursedArray<T>::remove(float key) {

}


// ----------------------------- PRIVATE FUNCTIONS -----------------------------
/*
 * Recursive function for a binary search for a given index.
 * Returns true if index exists in array.
 */
template<typename T>
bool CursedArray<T>::_findKey(float key, int leftLimit, int rightLimit) {
    if (leftLimit <= rightLimit) {
        int midpoint = leftLimit + ((rightLimit - leftLimit) / 2);

        if (_tree[midpoint].index == key)    // Key found
            return true;

        if (_tree[midpoint].index > key)
            return _findKey(key, leftLimit, midpoint - 1);

        // Remaining condition: _tree[midpoint] < index
        return _findKey(key, midpoint + 1, rightLimit);
    }

    return false;
}


/*
 * Returns true index of a given index.
 */
template<typename T>
int CursedArray<T>::_findIndex(float key) {
    return _findIndex_(key, 0, _tree.size() - 1);
}


/*
 *
 */
template<typename T>
int CursedArray<T>::_findIndex_(float key, int leftLimit, int rightLimit) {
    if (leftLimit <= rightLimit) {
        int midpoint = leftLimit + ((rightLimit - leftLimit) / 2);

        if (_tree[midpoint].index == key)    // Key found
            return midpoint;

        if (_tree[midpoint].index > key)
            return _findIndex_(key, leftLimit, midpoint - 1);

        // Remaining condition: _tree[midpoint].index < index
        return _findIndex_(key, midpoint + 1, rightLimit);
    }

    return -1;
}
