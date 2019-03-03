#ifndef STORAGE_ITEM_H
#define STORAGE_ITEM_H

#include "FS.h"

class AbstractStorageItem
{
private:
    /* data */
public:
    virtual File GetNextFile();
    virtual void RewindDir();
    virtual void SetFolder(std::string);
};

#endif