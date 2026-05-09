#pragma once
#include<iostream>
using namespace std;
template<typename T>
class Storage {
private:
    int capacity, count;
    T** items;
public:
    Storage(int cap = 100);
    Storage(const Storage<T>& other);
    ~Storage();

    bool add(T* item);
    T* get(int index) const;
    T* getByID(int id) const;
    bool remove(int id);
    int  getCount() const;
    void printAll() const;

    void sortDesc();
    void sortAsc();
};

template<typename T>
Storage<T>::Storage(int cap) : capacity(cap), count(0) {
    items = new T * [capacity];
}
template<typename T>
Storage<T>::Storage(const Storage<T>& other) : capacity(other.capacity), count(other.count) {
    items = new T * [capacity];
    for (int i = 0; i < count; i++)
        items[i] = new T(*other.items[i]);
}
template<typename T>
Storage<T>::~Storage() {
    for (int i = 0; i < count; i++)
        delete items[i];
    delete[] items;
}
template<typename T>
bool Storage<T>::add(T* item) {
    if (count >= capacity)
        return false;
    items[count++] = item;
    return true;
}
template<typename T>
T* Storage<T>::get(int index) const {
    if (index < 0 || index >= count)
        return nullptr;
    return items[index];
}
template<typename T>
T* Storage<T>::getByID(int id) const {
    for (int i = 0; i < count; i++)
        if (items[i]->getID() == id)
            return items[i];
    return nullptr;
}
template<typename T>
bool Storage<T>::remove(int id) {
    for (int i = 0; i < count; i++) {
        if (items[i]->getID() == id) {
            delete items[i];
            for (int j = i; j < count - 1; j++)
                items[j] = items[j + 1];
            count--;
            return true;
        }
    }
    return false;
}
template<typename T>
int Storage<T>::getCount() const {
    return count;
}
template<typename T>
void Storage<T>::printAll() const {
    for (int i = 0; i < count; i++) items[i]->printInfo();
}
template<typename T> void Storage<T>::sortDesc() {
    for (int i = 0; i < count - 1; i++)
        for (int j = 0; j < count - 1 - i; j++)
            if (*items[j + 1] > *items[j]) {
                T* tmp = items[j];
                items[j] = items[j + 1];
                items[j + 1] = tmp;
            }
}
template<typename T>
void Storage<T>::sortAsc() {
    for (int i = 0; i < count - 1; i++)
        for (int j = 0; j < count - 1 - i; j++)
            if (*items[j] > *items[j + 1]) {
                T* tmp = items[j];
                items[j] = items[j + 1];
                items[j + 1] = tmp;
            }
}
