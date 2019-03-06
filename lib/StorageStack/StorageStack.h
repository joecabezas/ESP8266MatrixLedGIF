#ifndef STORAGESTACK_H
#define STORAGESTACK_H

#include <vector>
#include <SD.h>

//sadly for now there is no concencus about file system base class
//between fs::File and SD:File
//there is a ongoing PR to solve this
//https://github.com/esp8266/Arduino/pull/5525
// 
// #include "FS.h"

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