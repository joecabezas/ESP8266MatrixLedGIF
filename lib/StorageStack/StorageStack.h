#ifndef STORAGESTACK_H
#define STORAGESTACK_H

#include <vector>

#include "FS.h"
#include "AbstractStorageItem.h"

class StorageStack
{
private:
    std::vector<AbstractStorageItem *> items;

    int currentItemIndex;

    AbstractStorageItem * GetCurrentItem();
public:
    StorageStack();

    void AddStorageItem(AbstractStorageItem *);
    File GetNextFile();
};

#endif