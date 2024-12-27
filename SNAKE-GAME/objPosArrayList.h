#ifndef OBJPOS_ARRAYLIST_H
#define OBJPOS_ARRAYLIST_H

#include "objPos.h"

class objPosArrayList
{
    private:
        struct Node {
            objPos data;
            Node* next;
        };
        Node* head;
        Node* tail;
        int listSize;

    public:
        objPosArrayList();
        ~objPosArrayList();

        int getSize() const;
        void insertHead(objPos thisPos);
        void insertTail(objPos thisPos);
        void removeHead();
        void removeTail();
        
        objPos getHeadElement() const;
        objPos getTailElement() const;
        objPos getElement(int index) const;
};

#endif