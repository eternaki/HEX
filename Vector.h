#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>

template<typename T>
class Vector {
private:
    T* data;
    size_t capacity;
    size_t length;
public:
    Vector() : data(nullptr), capacity(0), length(0) {}
    ~Vector() {
        delete[] data;
    }
    void push_back(const T& element) {
        if (length >= capacity) {
            if (capacity == 0) capacity = 1;
            else capacity *= 2;
            T* new_data = new T[capacity];
            for (size_t i = 0; i < length; ++i) {
                new_data[i] = data[i];
            }
            delete[] data;
            data = new_data;
        }
        data[length++] = element;
    }
    T& operator[](size_t index) {
        return data[index];
    }
    const T& operator[](size_t index) const {
        return data[index];
    }
    size_t size() const {
        return length;
    }
};

#endif
