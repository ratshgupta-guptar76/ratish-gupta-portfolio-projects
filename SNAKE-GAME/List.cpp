#include "List.h"

template <typename E>
list<E>::list() : first(nullptr), last(nullptr), listSize(0) {}

template <typename E>
list<E>::list(const list<E>& other) : first(nullptr), last(nullptr), listSize(0) {
    Node<E>* temp = other.first;
    while (temp != nullptr) {
        addLast(temp->data);
        temp = temp->next;
    }
}

template <typename E>
list<E>& list<E>::operator=(const list<E>& other) {
    if (this != &other) {
        clear();
        Node<E>* temp = other.first;
        while (temp != nullptr) {
            addLast(temp->data);
            temp = temp->next;
        }
    }
    return *this;
}

template <typename E>
list<E>::~list() {
    clear();
}

template <typename E>
void list<E>::clear() {
    while (first != nullptr) {
        Node<E>* temp = first;
        first = first->next;
        delete temp;
    }
    last = nullptr;
    listSize = 0;
}

template <typename E>
void list<E>::addFirst(const E& e) {
    Node<E>* newNode = new Node<E>(e, first);
    first = newNode;
    if (last == nullptr) last = newNode;
    listSize++;
}

template <typename E>
void list<E>::addLast(const E& e) {
    Node<E>* newNode = new Node<E>(e);
    if (last != nullptr) {
        last->next = newNode;
    } else {
        first = newNode;
    }
    last = newNode;
    listSize++;
}

template <typename E>
void list<E>::remove(const E& e) {
    Node<E>* current = first;
    Node<E>* previous = nullptr;

    while (current != nullptr && current->data != e) {
        previous = current;
        current = current->next;
    }

    if (current == nullptr) return; // Element not found

    if (previous == nullptr) {
        first = current->next;
        if (first == nullptr) last = nullptr;  // If list becomes empty
    } else {
        previous->next = current->next;
        if (current == last) last = previous;
    }

    delete current;
    listSize--;
}

template <typename E>
void list<E>::removeLast() {
    if (first == nullptr) return; // List is empty

    if (first == last) { // Single element case
        delete first;
        first = last = nullptr;
    } else {
        Node<E>* temp = first;
        while (temp->next != last) {
            temp = temp->next;
        }
        delete last;
        last = temp;
        last->next = nullptr;
    }

    listSize--;
}

template <typename E>
void list<E>::removeFirst() {
    if (first == nullptr) return; // List is empty

    Node<E>* temp = first;
    first = first->next;
    delete temp;

    if (first == nullptr) last = nullptr;  // List is now empty
    listSize--;
}

template <typename E>
int list<E>::size() const {
    return listSize;
}

template <typename E>
E list<E>::at(int index) const {
    if (index < 0 || index >= listSize) {
        throw std::out_of_range("Index out of range");
    }

    Node<E>* temp = first;
    for (int i = 0; i < index; ++i) {
        temp = temp->next;
    }

    return temp->data;
}

template <typename E>
void list<E>::set(int index, const E& e) {
    if (index < 0 || index >= listSize) {
        throw std::out_of_range("Index out of range");
    }

    Node<E>* temp = first;
    for (int i = 0; i < index; ++i) {
        temp = temp->next;
    }

    temp->data = e;
}
