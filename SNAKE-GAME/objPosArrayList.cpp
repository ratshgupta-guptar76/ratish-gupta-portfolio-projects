#include "objPosArrayList.h"
#include <stdexcept>

objPosArrayList::objPosArrayList() {
    head = nullptr;
    tail = nullptr;
    listSize = 0;
}

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
    Node* temp = head;
    head = head->next;
    delete temp;
    listSize--;
    if (head == nullptr) {
        tail = nullptr;
    }
}

void objPosArrayList::removeTail() {
    if (head == tail) {
        delete head;
        head = tail = nullptr;
    }
    else {
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

void objPosArrayList::makeHeadBody(string sym) {
    head->OBJ.setObjPos(head->OBJ.getX(), head->OBJ.getY(), sym);
}

objPos objPosArrayList::getHeadElement() const {
    return head->OBJ;
}

objPos objPosArrayList::getTailElement() const {
    return tail->OBJ;
}

objPos objPosArrayList::getElement(int index) const {
    Node* current = head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    return current->OBJ;
}

