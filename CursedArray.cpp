//
//
//
//
// ----------------------------------------------------------------------------

#ifndef CURSED_ARRAY
#define CURSED_ARRAY

#include "RedBlack_Tree.h"


template <typename T>
class CursedArray {

private:

    struct Proxy {   // https://stackoverflow.com/questions/18670530/properly-overloading-bracket-operator-for-hashtable-get-and-set
        CursedArray<T> * _ca;
        float _key;
    public:
        Proxy( CursedArray<T> * ca, float key) : _ca(ca), _key(key) {}
        Proxy( CursedArray<T> const * ca, const float & key) : _ca(ca), _key(key) {}

        operator T() const {
            T* value = _ca->_get(_key);
            if (value)
                return *value;
            else
                
        }

        void operator=(T value) {
            _ca->_set(_key, value);
        }
    };


    RedBlackTree<float, T> _tree;

public:
    Proxy operator [](float index) {
        return Proxy(this, index);
    }


private:
    T* _get(float index);
    void _set(float index, T value);
};


template <typename T>
T* CursedArray<T>::_get(float index) {
    return &_tree.findValue(index);
}


template <typename T>
void CursedArray<T>::_set(float index, T value) {
    _tree.insert(index, value);
}


#endif //CURSED_ARRAY