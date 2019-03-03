#include "StorageStack.h"

#define ERROR_NO_STORAGE_ITEMS 1

#define DEBUG

StorageStack::StorageStack()
{
    currentItemIndex = -1;
}

AbstractStorageItem* StorageStack::GetCurrentItem(){
    return items[currentItemIndex];
}

void StorageStack::AddStorageItem(AbstractStorageItem * storageItem){
    items.push_back(storageItem);
}

File StorageStack::GetNextFile(){
    #ifdef DEBUG
    Serial.print(">>>StorageStack::GetNextFile");
    #endif
    if(items.empty()) throw ERROR_NO_STORAGE_ITEMS;

    if((currentItemIndex < 0) || currentItemIndex > ((int)items.size() - 1)){
        currentItemIndex = 0;
    }

    #ifdef DEBUG
    Serial.print("currentItemIndex: ");
    Serial.println(currentItemIndex);
    #endif

    File file = GetCurrentItem()->GetNextFile();

    if(file) return file;

    currentItemIndex++;

    return GetNextFile();
}