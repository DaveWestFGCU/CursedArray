//
//
//
//
// ----------------------------------------------------------------------------

#include "CursedArray.h"


/*
 * Default constructor
 */
template<typename T>
CursedArray<T>::CursedArray() {
    _array = new Item[INIT_CAPACITY];
    _nextIndex = 0;
    _capacity = INIT_CAPACITY;
}


/*
 * Copy constructor
 */
template<typename T>
CursedArray<T>::CursedArray(CursedArray& arr) {
    _nextIndex = arr._nextIndex;
    _capacity = arr._capacity;

    _array = new Item[arr._capacity];

    for (int i = 0; i < arr.capacity; ++i)
        _array[i] = arr._array[i];
}


/*
 * Destructor
 */
template<typename T>
CursedArray<T>::~CursedArray() {
    delete[] _array;
}

/*
 * [] operator for getting a value for a given float index.
 */
template<typename T>
const T& CursedArray<T>::operator [](float key) const {
    for (int i = 0; i < _nextIndex; ++i) {
        if (_array[i].key == key) {
            return _array[i].value;
        }
    }
}


/*
 * [] operator for setting a value at a given float index.
 */
template<typename T>
T& CursedArray<T>::operator [](float key) {
    // Key in _array, overwrite value
    for (int i = 0; i < _nextIndex; ++i) {
        if (_array[i].key == key) {
            return _array[i].value;
        }
    }

    // Key not in _array
    if (_nextIndex == _capacity) {         // Array full, expand array
        Item* newArray = new Item[_capacity * 2];

        for (int i = 0; i < _capacity; ++i) {
            newArray[i] = _array[i];
        }

        delete [] _array;
        _array = newArray;
        _capacity *= 2;
    }

    // Add item to array
    int index {0};
    while (key > _array[index].key && index < _nextIndex) {   // Find index for key
        ++index;
    }

    for (int i = _nextIndex; i > index; --i) {  // Shift items forward until index
        _array[i] = _array[i-1];
    }

    _array[index] = Item{key, setValue};
    ++_nextIndex;

    return _array[index].value;
}


/*
 * [] operator for setting a value at a given float index.
 */
template<typename T>
T& CursedArray<T>::operator [](double key) {
    return CursedArray<T>::operator []((float)key);
}


/*
 * [] Operator overloaded to error if index is an int or int literal.
 */
template<typename T>
T& CursedArray<T>::operator [](int key) {
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
 * Returns the capacity number of elements
 */
template<typename T>
int CursedArray<T>::capacity() {
    return _capacity;
}


/*
 * Binary search for an index in the array.
 * Returns true if index exists.
 */
template<typename T>
bool CursedArray<T>::exists(float key) {
    return _findKey(key, 0, _nextIndex-1);
}


/*
 * Returns a pointer to an array of false allIndexes.
 */
template<typename T>
float* CursedArray<T>::allIndexes() {
    float *indexArray = new float[_nextIndex];

    for (int i = 0; i < _nextIndex; ++i)
        indexArray[i] = _array[i].key;

    return indexArray;
}


/*
 * Returns the number of elements currently in the array.
 */
template<typename T>
int CursedArray<T>::length() {
    return _nextIndex;
}


/*
 * Removes an element from the array.
 */
template<typename T>
void CursedArray<T>::remove(float key) {
    int index = _findIndex(key);

    if (index == -1)    // Key not in array
        return;

    // Shift the elements above the item down
    for (int i = index; i < _nextIndex-1; ++i)
        _array[i] = _array[i+1];

    --_nextIndex;

    // If this remove reduces the number of elements in the array to under
    // 1/3 the size of the array, reduce the array size to 1/2 its current
    // size.
    if (_nextIndex <= .34 * _capacity) {
        Item *newArray = new Item[(int)(_capacity * 0.5)];

        for (int i = 0; i < _nextIndex; ++i)
            newArray[i] = _array[i];

        delete [] _array;
        _array = newArray;
        _capacity /= 2;
    }
}


// ----------------------------- PRIVATE FUNCTIONS -----------------------------
/*
 * Recursive function for a binary search for a given key.
 * Returns true if key exists in array.
 */
template<typename T>
bool CursedArray<T>::_findKey(float key, int leftLimit, int rightLimit) {
    if (leftLimit <= rightLimit) {
        int midpoint = leftLimit + ((rightLimit - leftLimit) / 2);

        if (_array[midpoint].key == key)    // Key found
            return true;

        if (_array[midpoint].key > key)
            return _findKey(key, leftLimit, midpoint - 1);

        // Remaining condition: _array[midpoint] < key
        return _findKey(key, midpoint + 1, rightLimit);
    }

    return false;
}


/*
 * Returns true index of a given key.
 */
template<typename T>
int CursedArray<T>::_findIndex(float key) {
    return _findIndex_(key, 0, _nextIndex - 1);
}


/*
 *
 */
template<typename T>
int CursedArray<T>::_findIndex_(float key, int leftLimit, int rightLimit) {
    if (leftLimit <= rightLimit) {
        int midpoint = leftLimit + ((rightLimit - leftLimit) / 2);

        if (_array[midpoint].key == key)    // Key found
            return midpoint;

        if (_array[midpoint].key > key)
            return _findIndex_(key, leftLimit, midpoint - 1);

        // Remaining condition: _array[midpoint].key < key
        return _findIndex_(key, midpoint + 1, rightLimit);
    }

    return -1;
}
