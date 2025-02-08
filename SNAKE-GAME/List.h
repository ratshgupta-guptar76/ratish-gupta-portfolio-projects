#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <stdexcept>

template <typename E>
class list {
private:
    struct Node {
        E data;
        Node* next;
        Node* prev;

        Node(const E& value) : data(value), next(nullptr), prev(nullptr) {}
    };

    Node* head;
    Node* tail;
    int listSize;

public:
    list() : head(nullptr), tail(nullptr), listSize(0) {}
    ~list();

    int size() const { return listSize; }
    void addFirst(const E &element);
    void addLast(const E &element);
    void removeFirst();
    void removeLast();
    E at(int index) const;
    void set(int index, const E &element);
};

// Destructor to delete all nodes
template <typename E>
list<E>::~list() {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
    tail = nullptr;
    listSize = 0;
}

// Add element at the front
template <typename E>
void list<E>::addFirst(const E &element) {
    Node* newNode = new Node(element);
    if (!head) {
        head = tail = newNode;
    } else {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }
    listSize++;
}

// Add element at the end
template <typename E>
void list<E>::addLast(const E &element) {
    Node* newNode = new Node(element);
    if (!tail) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    listSize++;
}

// Remove first element
template <typename E>
void list<E>::removeFirst() {
    if (!head) return;
    Node* temp = head;
    head = head->next;
    if (head) head->prev = nullptr;
    else tail = nullptr;
    delete temp;
    listSize--;
}

// Remove last element
template <typename E>
void list<E>::removeLast() {
    if (!tail) return;
    Node* temp = tail;
    tail = tail->prev;
    if (tail) tail->next = nullptr;
    else head = nullptr;
    delete temp;
    listSize--;
}

// Get element at index
template <typename E>
E list<E>::at(int index) const {
    if (index < 0 || index >= listSize) throw std::out_of_range("Index out of bounds");
    Node* curr = head;
    for (int i = 0; i < index; i++) {
        curr = curr->next;
    }
    return curr->data;
}

// Set element at index
template <typename E>
void list<E>::set(int index, const E &element) {
    if (index < 0 || index >= listSize) throw std::out_of_range("Index out of bounds");
    Node* curr = head;
    for (int i = 0; i < index; i++) {
        curr = curr->next;
    }
    curr->data = element;
}

#endif
