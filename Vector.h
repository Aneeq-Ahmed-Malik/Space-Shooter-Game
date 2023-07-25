#ifndef VECTOR_H_
#define VECTOR_H_

using namespace std;

template <class T>
class Vector
{
private:
    T *arr;
public:

    int capacity;
    int current;

    Vector()
    {
        arr = new T[1];
        capacity = 1;
        current = 0;
    }

    void push(T data)
    {
        if (current == capacity)
        {
            T *temp = new T[3 + capacity];

            for (int i = 0; i < capacity; i++)
                temp[i] = arr[i];

            delete[] arr;
            capacity += 3;
            arr = temp;
        }

        arr[current] = data;
        current++;
    }

    T &operator[](int index)
    {
        if (index < current)
            return arr[index];
        return arr[current];
    }

    void remove(int index)
    {
        T *temp = new T[current - 1];
        int count = 0;
        for (int i = 0; i < current; i++)
        {
            if (i == index)
                continue;
            temp[count++] = arr[i];
        }

        delete[] arr;
        --current;
        capacity = current;
        arr = temp;
    }
    
    ~Vector()
    {
        delete[] arr;
    }
};

#endif
