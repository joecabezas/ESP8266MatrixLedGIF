#ifndef SPIFFS_STORAGE_ITEM_H
#define SPIFFS_STORAGE_ITEM_H

#include "AbstractStorageItem.h"

class SPIFFSStorageItem: public AbstractStorageItem{
    private:
        Dir dir;
        std::string folderPath;
    public:
        SPIFFSStorageItem();

        virtual File GetNextFile() override;
        virtual void RewindDir() override;
        virtual void SetFolder(std::string) override;
};

#endif