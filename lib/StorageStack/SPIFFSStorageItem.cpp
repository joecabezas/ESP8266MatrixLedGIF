#ifdef REMOVE_THIS_WHEN_FS_FILE_IS_COMPATIBLE
#include "SPIFFSStorageItem.h"

#define ERROR_NO_FOLDER_SET 1
#define ERROR_NO_NEXT_FILE 2

#define DEBUG

SPIFFSStorageItem::SPIFFSStorageItem()
{
    SPIFFS.begin();
}

void SPIFFSStorageItem::SetFolder(std::string path){
    folderPath = path;
    RewindDir();
}

File SPIFFSStorageItem::GetNextFile()
{
    #ifdef DEBUG
    Serial.println(">>>SPIFFSStorageItem::GetNextFile");
    #endif

    if(folderPath.empty()) throw ERROR_NO_FOLDER_SET;

    if(!dir.next()){
        RewindDir();
        dir.next();
    }
    
    #ifdef DEBUG
    Serial.print(">>>filename: ");
    Serial.println(dir.fileName());
    #endif
    
    return dir.openFile("r");
}

void SPIFFSStorageItem::RewindDir()
{
    #ifdef DEBUG
    Serial.print(">>>SPIFFSStorageItem::RewindDir");
    #endif

    dir = SPIFFS.openDir(folderPath.c_str());
}
#endif