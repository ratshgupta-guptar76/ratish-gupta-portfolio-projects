#ifndef OBJPOS_ARRAYLIST_H
#define OBJPOS_ARRAYLIST_H

#include "objPos.h"

class objPosArrayList
{
    private:
        struct Node {
            objPos OBJ;
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
        void makeHeadBody(string sym);
        
        objPos getHeadElement() const;
        objPos getTailElement() const;
        objPos getElement(int index) const;
};

#endif