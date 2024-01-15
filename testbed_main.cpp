#include <iostream>
#include "CursedArray.cpp"

using std::string, std::cout;

template<typename T>
void printArray(CursedArray<T>& arr);

int main() {
    CursedArray<string> array;


    array[3.f] = "apple";
    /*
    array[0.5] = "orange";
    array[-1.2] = "banana";
    float kiwiIndex = -0.07;
    array[kiwiIndex] = "kiwi";
    array[-20.23] = "blueberry";
    printArray(array);

    array.remove(-0.07);
    array.remove(-1.2);
    array.remove(3.0);
    array.remove(-20.23);
    printArray(array);
    */

    return 0;
}

/*
template<typename T>
void printArray(CursedArray<T>& arr) {
    float* arrPtr = arr.allIndexes();
    int lastIndex = arr.length();

    cout << "CursedArray: ";
    for (int i = 0; i < lastIndex; ++i) {
        cout << arrPtr[i] << " : " << arr[arrPtr[i]];
        if (i != lastIndex-1)
            cout << ", ";
    }
    cout << '\n';

    delete[] arrPtr;
}
*/