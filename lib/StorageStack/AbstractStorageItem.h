#ifndef STORAGE_ITEM_H
#define STORAGE_ITEM_H

#include <SPI.h>
#include <SD.h>

class AbstractStorageItem
{
public:
    virtual File GetNextFile();
    virtual void RewindDir();
    virtual void SetFolder(std::string);
};

#endif