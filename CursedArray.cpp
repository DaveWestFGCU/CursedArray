//
//
//
//
// ----------------------------------------------------------------------------

#ifndef CURSED_ARRAY
#define CURSED_ARRAY

#include "RedBlack_Tree.cpp"


template <typename T>
class CursedArray {

private:
    static RedBlackTree<float, T>* _tree;

public:
    CursedArray();
    ~CursedArray();

    const T & operator [](float index) const;
    void operator =(T value);
    CursedArray<T> & operator [](float index);
    T & operator [](double index);
    void operator [](int index);


    bool remove(float index);
    int size();
    int _index;
};


/**
 * Default constructor
 */
template <typename T>
CursedArray<T>::CursedArray() {
    _tree = new RedBlackTree<float, T>;
}


/**
 * Destructor
 */
template <typename T>
CursedArray<T>::~CursedArray() {
    delete _tree;
}


/**
 * [] operator for accessing a value for a given float index.
 */
template <typename T>
const T & CursedArray<T>::operator [](float index) const {
    return _tree->findValue(index);
}


/**
 *
 * @param value
 */
template <typename T>
void CursedArray<T>::operator=(T value) {
    _tree->insert(_index, value);
}

/**
 * [] operator for assigning a value at a given float index.
 * @tparam T Template datatype for the value to store.
 * @param index Index at which to store the value.
 * @return Returns a reference to the index's value.
 */
template <typename T>
CursedArray<T> & CursedArray<T>::operator [](float index) {
    this->_index = index;
    return *this;
}


/**
 * [] operator overloading to convert a double index to a float index.
 */
template<typename T>
T& CursedArray<T>::operator [](double index) {
    return CursedArray<T>::operator []((float)index);
}


/**
 * [] Operator overloaded to error if index is an int or int literal.
 */
template<typename T>
void CursedArray<T>::operator [](int) {
    static_assert(false, "CursedArray index cannot be an integer.");
}


/**
 *
 * @tparam T
 * @param index
 * @return Returns true if the index was found and removed.
 */
template <typename T>
bool CursedArray<T>::remove(float index) {
    return _tree->remove(index);
}


template <typename T>
inline int CursedArray<T>::size() {
    return _tree->size();
}


#endif //CURSED_ARRAY