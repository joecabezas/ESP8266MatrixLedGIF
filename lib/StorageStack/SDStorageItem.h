#ifndef SD_STORAGE_ITEM_H
#define SD_STORAGE_ITEM_H

#include "AbstractStorageItem.h"

class SDStorageItem: public AbstractStorageItem{
    private:
        File dir;
        std::string folderPath;
    public:
        SDStorageItem(int pin);

        virtual File GetNextFile() override;
        virtual void RewindDir() override;
        virtual void SetFolder(std::string) override;
};

#endif