#include "sd_read_write.h"

const char *SDManager::DEBUG_TAG = "SD";

void SDManager::initSD() {
  Serial.println("setup SD");
  SD_MMC.setPins(SD_MMC_CLK, SD_MMC_CMD, SD_MMC_D0);
  if (!SD_MMC.begin("/sdcard", true, true, SDMMC_FREQ_DEFAULT, 5)) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD_MMC.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No SD_MMC card attached");
    return;
  }
  // create file if doesn't exist
  if (!existFile(SD_MMC, "/knownNetworks.csv")) {
    // non-existent file
    // create file
    writeFile(SD_MMC, "/knownNetworks.csv", "");
  }
}

void SDManager::listDir(fs::FS &fs, const char *dirname, uint8_t levels) {
  Serial.printf("[%s] Listing directory: %s\n", DEBUG_TAG, dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        listDir(fs, file.path(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void SDManager::createDir(fs::FS &fs, const char *path) {
  Serial.printf("[%s] Creating Dir: %s\n", DEBUG_TAG, path);
  if (fs.mkdir(path)) {
    Serial.println("Dir created");
  } else {
    Serial.println("mkdir failed");
  }
}

void SDManager::removeDir(fs::FS &fs, const char *path) {
  Serial.printf("[%s] Removing Dir: %s\n", DEBUG_TAG, path);
  if (fs.rmdir(path)) {
    Serial.println("Dir removed");
  } else {
    Serial.println("rmdir failed");
  }
}

String SDManager::readFile(fs::FS &fs, const char *path) {
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return "";  // Restituisci una stringa vuota in caso di errore
  }

  String fileContent = "";
  while (file.available()) {
    char nextChar = file.read();
    fileContent += nextChar;
  }

  return fileContent;
}

void SDManager::writeFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
}

void SDManager::appendFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
}

void SDManager::renameFile(fs::FS &fs, const char *path1, const char *path2) {
  Serial.printf("Renaming file %s to %s\n", path1, path2);
  if (fs.rename(path1, path2)) {
    Serial.println("File renamed");
  } else {
    Serial.println("Rename failed");
  }
}

void SDManager::deleteFile(fs::FS &fs, const char *path) {
  Serial.printf("Deleting file: %s\n", path);
  if (fs.remove(path)) {
    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
}

void SDManager::testFileIO(fs::FS &fs, const char *path) {
  File file = fs.open(path);
  static uint8_t buf[512];
  size_t len = 0;
  uint32_t start = millis();
  uint32_t end = start;
  if (file) {
    len = file.size();
    size_t flen = len;
    start = millis();
    while (len) {
      size_t toRead = len;
      if (toRead > 512) {
        toRead = 512;
      }
      file.read(buf, toRead);
      len -= toRead;
    }
    end = millis() - start;
    Serial.printf("%u bytes read for %u ms\r\n", flen, end);
    file.close();
  } else {
    Serial.println("Failed to open file for reading");
  }

  file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }

  size_t i;
  start = millis();
  for (i = 0; i < 2048; i++) {
    file.write(buf, 512);
  }
  end = millis() - start;
  Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
  file.close();
}

// Funzione per cercare l'SSID nel file
bool SDManager::findSSIDInFile(fs::FS &fs, const char *path, const char *ssidToFind) {
  File file = fs.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return false;
  }

  // Buffer per leggere il file riga per riga
  char buffer[256];
  String ssid(ssidToFind);

  // Leggi il file riga per riga
  while (file.available()) {
    file.readStringUntil('\n').toCharArray(buffer, sizeof(buffer));
    String line(buffer);

    // Dividi la riga in SSID e password
    int commaIndex = line.indexOf(',');
    if (commaIndex >= 0) {
      String lineSSID = line.substring(0, commaIndex);
      if (lineSSID.equals(ssid)) {
        // Trovato l'SSID
        Serial.println("SSID trovato nel file!");
        file.close();
        return true;
      }
    }
  }

  // L'SSID non è stato trovato nel file
  file.close();
  Serial.println("SSID non trovato nel file.");
  return false;
}

bool SDManager::existFile(fs::FS &fs, const char *path) {
  Serial.printf("Check exist file: %s\n", path);

  File file = fs.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return false;  // Restituisci una stringa vuota in caso di errore
  }
  return true;
}
