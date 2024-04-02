#ifndef __SD_READ_WRITE_H
#define __SD_READ_WRITE_H

#include "Arduino.h"
#include "FS.h"
#include "SD_MMC.h"

// SD
#define SD_MMC_CMD 38  //Please do not modify it.
#define SD_MMC_CLK 39  //Please do not modify it.
#define SD_MMC_D0 40   //Please do not modify it.

class SDManager {
public:
  static void initSD();
  static void listDir(fs::FS &fs, const char *dirname, uint8_t levels);
  static void createDir(fs::FS &fs, const char *path);
  static void removeDir(fs::FS &fs, const char *path);
  static String readFile(fs::FS &fs, const char *path);
  static void writeFile(fs::FS &fs, const char *path, const char *message);
  static void appendFile(fs::FS &fs, const char *path, const char *message);
  static void renameFile(fs::FS &fs, const char *path1, const char *path2);
  static void deleteFile(fs::FS &fs, const char *path);
  static void testFileIO(fs::FS &fs, const char *path);
  static bool findSSIDInFile(fs::FS &fs, const char *path, const char *ssidToFind);
  static bool existFile(fs::FS &fs, const char *path);

private:
  static const char *DEBUG_TAG;
};

#endif
