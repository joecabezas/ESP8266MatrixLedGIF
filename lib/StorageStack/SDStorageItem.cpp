#include <SPI.h>
#include <SD.h>

#include "SDStorageItem.h"

#define ERROR_SD_INITIALIZATION_FAILED 1
#define ERROR_NO_FOLDER_SET 2
#define ERROR_NO_NEXT_FILE 3

#define DEBUG

SDStorageItem::SDStorageItem(int pin)
{
    #ifdef DEBUG
    Serial.println("SDStorageItem initializing...");
    #endif
    if (!SD.begin(pin, SPI_HALF_SPEED))
    {
        #ifdef DEBUG
        Serial.println("SDStorageItem initialization failed!");
        #endif
        throw ERROR_SD_INITIALIZATION_FAILED;
    }
    #ifdef DEBUG
    Serial.println("SDStorageItem initialized!");
    #endif
}

void SDStorageItem::SetFolder(std::string path){
    folderPath = path;
    dir = SD.open(folderPath.c_str());
}

File SDStorageItem::GetNextFile()
{
    #ifdef DEBUG
    Serial.println(">>>SDStorageItem::GetNextFile");
    #endif

    if(folderPath.empty()){
        #ifdef DEBUG
        Serial.println("folderPath is empty");
        #endif
        throw ERROR_NO_FOLDER_SET;
    }

    File entry = dir.openNextFile();

    if(!entry){
        #ifdef DEBUG
        Serial.print("next file does not exist!, file name is: ");
        Serial.println(entry.name());
        #endif
        RewindDir();
        entry = dir;
    }

    while(entry.isDirectory()){
        #ifdef DEBUG
        Serial.print("current file is a directory!: ");
        Serial.println(entry.name());
        #endif
        entry = dir.openNextFile();
    }
    
    #ifdef DEBUG
    Serial.print("final file name: ");
    Serial.println(entry.name());
    #endif
    
    return entry;
}

void SDStorageItem::RewindDir()
{
    #ifdef DEBUG
    Serial.println(">>>SDStorageItem::RewindDir");
    #endif

    dir.rewindDirectory();

    #ifdef DEBUG
    Serial.print("current file after rewind: ");
    Serial.println(dir.name());
    #endif
}