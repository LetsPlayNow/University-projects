#pragma once
#include <string>

class Array
{
public:
    Array(int size, int element = 0)
    {
        pointer = new int[size];
        for (int i = 0; i < size; i++)
            pointer[i] = element;
    }

    ~Array()
    {
        delete[] pointer;
    }

    int Size()
    {
        return size;
    }

    int operator[](int index)
    {
        if (index < 0 || index > size - 1)
            throw "Out of index";

        return pointer[index];
    }

    std::string ToString()
    {
        std::string  obj = "";
        for (int i = 0; i < size; i++)
        {
            throw "Non implementer";
        }
    }

private:
    int * pointer;
    int size;
};