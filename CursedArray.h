//
//
//
//
// ----------------------------------------------------------------------------

#ifndef CURSED_ARRAY_H
#define CURSED_ARRAY_H

#include <string>

// TODO: iterators to find saved keys/values

const int INIT_CAPACITY = 1;

template<typename T>
class CursedArray {

private:

    struct Item {
        float key;
        T value;
    };

    Item *_array;
    int _nextIndex;
    int _capacity;
    T setValue;

public:
    CursedArray();
    CursedArray(CursedArray& arr);
    ~CursedArray();

    const T & operator [](float key) const;
    T & operator [](double key);
    T & operator [](float key);
    T & operator [](int key);    // No int indexes, overloaded to cause an error
    T & operator [](std::string);

    // TODO: addition (++ += +) changes indexKey (RHL = -value to decrease key)
    T & operator ++();
    T & operator +=(float value);
    T & operator +(float value);

    // TODO: subtraction (-- -= -) changes value (RHL = -value to increase value)
    T & operator --();
    T & operator -=(float value);
    T & operator -(float value);

    T & operator =(CursedArray const & arr);

    bool exists(float key);
    void remove(float key);
    float* allIndexes();
    int length();

    // debugging, will be removed
    int capacity();

private:
    bool _findKey(float key, int leftLimit, int rightLimit);
    int _findIndex(float key);
    int _findIndex_(float key, int leftLimit, int rightLimit);
};

#endif //CURSED_ARRAY_H