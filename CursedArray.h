//
//
//
//
// ----------------------------------------------------------------------------

#ifndef CURSED_ARRAY_H
#define CURSED_ARRAY_H

#include <string>
#include "RedBlack_Tree.cpp"

// TODO: iterators to find saved keys/values

template<typename T>
class CursedArray {

private:
    RedBlackTree<float, T> _tree;
    T setValue;

public:
    CursedArray();
    ~CursedArray();
    const T & operator =(float key) const;
/*
    const V & operator [](float key) const;
    V & operator [](double key);
    V & operator [](float key);
    void operator [](int key);    // No int indexes, overloaded to cause an error
    V & operator [](std::string);

    // TODO: addition (++ += +) changes indexKey (RHL = -value to decrease index)
    V & operator ++();
    V & operator +=(float value);
    V & operator +(float value);

    // TODO: subtraction (-- -= -) changes value (RHL = -value to increase value)
    V & operator --();
    V & operator -=(float value);
    V & operator -(float value);

    V & operator =(CursedArray const & arr);

    bool findByValue(float key);
    void remove(float key);
    float* allIndexes();
    int length();

    // debugging, will be removed
    int size();

private:
    bool _findKey(float key, int leftLimit, int rightLimit);
    int _findIndex(float key);
    int _findIndex_(float key, int leftLimit, int rightLimit);
    */
};

#endif //CURSED_ARRAY_H
