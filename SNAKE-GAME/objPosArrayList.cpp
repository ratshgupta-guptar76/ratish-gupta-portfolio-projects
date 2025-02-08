#include "objPosArrayList.h"

objPosArrayList::objPosArrayList() { }

objPosArrayList::~objPosArrayList() { }

int objPosArrayList::getSize() const {
    return objLL.size();
}

void objPosArrayList::insertHead(objPos thisPos) {
    objLL.addFirst(thisPos);
}

void objPosArrayList::insertTail(objPos thisPos) {
    objLL.addLast(thisPos);
}

void objPosArrayList::removeHead() {
    objLL.removeFirst();
}

void objPosArrayList::removeTail() {
    objLL.removeLast();
}

objPos objPosArrayList::getHeadElement() const {
    return objLL.at(0);
}

objPos objPosArrayList::getTailElement() const {
    return objLL.at(objLL.size() - 1);
}

objPos objPosArrayList::getElement(int index) const {
    return objLL.at(index);
}

void objPosArrayList::makeHeadBody(string sym) {
    if (objLL.size() > 0) {
        objPos head = objLL.at(0);
        head.setSymbol(sym);
        objLL.set(0, head);
    }
}
