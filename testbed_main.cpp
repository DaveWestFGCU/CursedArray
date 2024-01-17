#include <iostream>
#include "CursedArray.cpp"

using std::string, std::cout;

template<typename T>
void printArray(CursedArray<T>& arr);

int main() {
    CursedArray<string> array;

    array[3.f] = "apple";
    string item = array[3.f];
    cout << item;
    /*
    array[0.5] = "orange";
    /*
    cout << "array[3.f] = " << array[3.f] << "\n";
    cout << "array[0.5] = " << array[0.5] << "\n";

    array[-1.2] = "banana";
    float kiwiIndex = -0.07;
    array[kiwiIndex] = "kiwi";
    array[-20.23] = "blueberry";
    cout << array[3.f] << " ";
    cout << array[3.f] << " ";
    cout << array[3.f] << " ";
    cout << array[3.f] << " ";
    cout << array[0.5] << " ";
    cout << array[-1.2] << " ";
    cout << array[-0.07] << " ";
    cout << array[-20.23] << "\n";
    cout << "size:" << array.size() << "\n";

    array.remove(-0.07);
    cout << "size:" << array.size() << "\n";
    array.remove(-1.2);
    cout << "size:" << array.size() << "\n";
    array.remove(3.f);
    cout << "size:" << array.size() << "\n";
    array.remove(-20.23);
    cout << "size:" << array.size() << "\n";

    array[3.f];
    array[3.f];
    array[3.f];
    array[3.f];
    array[3.f];
    cout << "Hi!\n";
    cout << array[3.f] << " ";
    cout << array[3.f] << " ";
    cout << array[3.f] << " ";
    cout << array[3.f] << " ";
    cout << array[0.5] << " ";
    cout << array[-1.2] << " ";
    cout << array[-0.07] << " ";
    cout << array[-20.23] << " ";
    cout << "\nsize:" << array.size() << "\n";
*/
    return 0;
}

/*
template<typename V>
void printArray(CursedArray<V>& arr) {
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