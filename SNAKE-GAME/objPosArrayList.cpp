#include "objPosArrayList.h"
#include <stdexcept>

objPosArrayList::objPosArrayList() : head(nullptr), tail(nullptr), listSize(0) {}

objPosArrayList::~objPosArrayList() {
    Node* current = head;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

int objPosArrayList::getSize() const {
    return listSize;
}

void objPosArrayList::insertHead(objPos thisPos) {
    Node* newNode = new Node{thisPos, head};
    head = newNode;
    if (tail == nullptr) {
        tail = head;
    }
    listSize++;
}

void objPosArrayList::insertTail(objPos thisPos) {
    Node* newNode = new Node{thisPos, nullptr};
    if (tail != nullptr) {
        tail->next = newNode;
    }
    tail = newNode;
    if (head == nullptr) {
        head = tail;
    }
    listSize++;
}

void objPosArrayList::removeHead() {
    if (head == nullptr) {
        throw std::out_of_range("List is empty");
    }
    Node* temp = head;
    head = head->next;
    delete temp;
    listSize--;
    if (head == nullptr) {
        tail = nullptr;
    }
}

void objPosArrayList::removeTail() {
    if (tail == nullptr) {
        throw std::out_of_range("List is empty");
    }
    if (head == tail) {
        delete head;
        head = tail = nullptr;
    } else {
        Node* current = head;
        while (current->next != tail) {
            current = current->next;
        }
        delete tail;
        tail = current;
        tail->next = nullptr;
    }
    listSize--;
}

objPos objPosArrayList::getHeadElement() const {
    if (head == nullptr) {
        throw std::out_of_range("List is empty");
    }
    return head->data;
}

objPos objPosArrayList::getTailElement() const {
    if (tail == nullptr) {
        throw std::out_of_range("List is empty");
    }
    return tail->data;
}

objPos objPosArrayList::getElement(int index) const {
    if (index < 0 || index >= listSize) {
        throw std::out_of_range("Index out of range");
    }
    Node* current = head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    return current->data;
}

