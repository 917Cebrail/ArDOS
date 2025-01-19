#include <EEPROM.h>

#define DISK_SIZE 512
#define A_START 0
#define B_START 512

enum Disk { A_DISK, B_DISK };

const char FILE_START = 'F';

int Var;
int ram[32];

void writeToDisk(Disk disk, const char* data) {
  int start = (disk == A_DISK) ? A_START : B_START;
  int length = strlen(data);

  bool spaceFound = false;
  for (int i = start; i < start + DISK_SIZE; i++) {
    if (EEPROM.read(i) != FILE_START) {
      if (i + length + 1 < start + DISK_SIZE) {
        EEPROM.write(i, FILE_START);
        for (int j = 0; j < length; j++) {
          EEPROM.write(i + 1 + j, data[j]);
        }
        spaceFound = true;
        break;
      } else {
        Serial.println("Insufficient space.");
        return;
      }
    }
  }

  if (spaceFound) {
    Serial.print("Veri ");
    Serial.print((disk == A_DISK) ? "A" : "B");
    Serial.println(" written to disk.");
  } else {
    Serial.println("Insufficient space or other error.");
  }
}

void clearDisk(Disk disk) {
  int start = (disk == A_DISK) ? A_START : B_START;
  for (int i = 0; i < DISK_SIZE; i++) {
    EEPROM.write(start + i, 0);
  }

  Serial.print((disk == A_DISK) ? "A" : "B");
  Serial.println(" The disk has been cleaned.");
}
void readDisk(Disk disk) {
  int start = (disk == A_DISK) ? A_START : B_START;

  Serial.print((disk == A_DISK) ? "A" : "B");
  Serial.println(" Files on the disk:");

  bool fileFound = false;
  for (int i = start; i < start + DISK_SIZE; i++) {
    if (EEPROM.read(i) == FILE_START) {
      Serial.print("File : ");

      for (int j = i + 1; j < start + DISK_SIZE && EEPROM.read(j) != 0; j++) {
        char c = EEPROM.read(j);
        Serial.print(c);
      }
      Serial.println();
      fileFound = true;
    }
  }

  if (!fileFound) {
    Serial.println("No files found.");
  }
}

void readFile(Disk disk, int fileNumber) {
  int start = (disk == A_DISK) ? A_START : B_START;

  int fileIndex = 1;
  bool fileFound = false;
  for (int i = start; i < start + DISK_SIZE; i++) {
    if (EEPROM.read(i) == FILE_START) {
      if (fileIndex == fileNumber) {
        Serial.print("Dosya ");
        Serial.print(fileNumber);
        Serial.print(": ");

        for (int j = i + 1; j < i + DISK_SIZE && EEPROM.read(j) != 0; j++) {
          char c = EEPROM.read(j);
          Serial.print(c);
        }
        Serial.println();
        fileFound = true;
        break;
      }
      fileIndex++;
    }
  }

  if (!fileFound) {
    Serial.println("Invalid file number.");
  }
}

/*

  Add the all code line ended a ' ' (space) .

  PAINT Hello, World!         // Print "Hello, World!"

  VAR B = 15 , ADDRESS : 20   // Set variable B to 15 at address 20
  ARG ADDRESS : 10 , ADDRESS : 20 , ADD   // Add values at addresses 10 and 20 (42 + 15), should print 57
  ARG ADDRESS : 10 , ADDRESS : 20 , SUB   // Subtract values at addresses 10 and 20 (42 - 15), should print 27
  ARG ADDRESS : 10 , ADDRESS : 20 , MUL   // Multiply values at addresses 10 and 20 (42 * 15), should print 630
  ARG ADDRESS : 10 , ADDRESS : 20 , DIV   // Divide values at addresses 10 and 20 (42 / 15), should print 2 (integer division)

  VAR A = 42 , ADDRESS : 10   // Set variable A to 42 at address 10
  PRINT 10                    // Print value at address 10 (should print 42)

*/
void Compiler(char *code) {
  if (strncmp(code, "VAR", 3) == 0) {
    int add, _Var;
    char ch;
    if (sscanf(code, "VAR %c = %d , ADDRESS : %d", &ch, &_Var, &add) == 3) {
      Var = _Var;
      if (add >= 0 && add < 128) {
        ram[add] = Var;
      } else {
        Serial.println("ADDRESS OUT OF RANGE");
      }
    } else {
      Serial.println("SYNTAX ERROR");
    }
  }
  else if (strncmp(code, "PAINT", 5) == 0) {
    char m[256];
    if (sscanf(code, "PAINT %s", m) == 1) {
      Serial.println(m);
    } else {
      Serial.println("SYNTAX ERROR");
    }
  }
  else if (strncmp(code, "PRINT", 5) == 0) {
    int add;
    if (sscanf(code, "PRINT %d", &add) == 1) {
      if (add >= 0 && add < 128) {
        Serial.println(ram[add]);
      } else {
        Serial.println("ADDRESS OUT OF RANGE");
      }
    } else {
      Serial.println("SYNTAX ERROR");
    }
  }
  else if (strncmp(code, "ARG", 3) == 0) {
    int add1, add2;
    char op[4];
    if (sscanf(code, "ARG ADDRESS : %d , ADDRESS : %d , %3s", &add1, &add2, op) == 3) {
      int s1 = (add1 >= 0 && add1 < 128) ? ram[add1] : 0;
      int s2 = (add2 >= 0 && add2 < 128) ? ram[add2] : 0;
      if (strcmp(op, "ADD") == 0) {
        Serial.println(s1 + s2);
      } else if (strcmp(op, "SUB") == 0) {
        Serial.println(s1 - s2);
      } else if (strcmp(op, "MUL") == 0) {
        Serial.println(s1 * s2);
      } else if (strcmp(op, "DIV") == 0) {
        if (s2 != 0) {
          Serial.println(s1 / s2);
        } else {
          Serial.println("DIVISION BY ZERO");
        }
      } else {
        Serial.println("INVALID OPERATOR");
      }
    } else {
      Serial.println("SYNTAX ERROR");
    }
  }
  else if (strncmp(code, "EXIT", 4) == 0) {
    Serial.print("EXIT");
  }
  else {
    Serial.println("INVALID COMMAND");
  }
}

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Arduino DOS");
  Serial.println("1 > Read disk A");
  Serial.println("2 > Read disk B");
  Serial.println("3 > Write disk A");
  Serial.println("4 > Write disk B");
  Serial.println("5 > Cleaning Disk A");
  Serial.println("6 > Cleaning Disk B");
  Serial.println("7 > Enter Code");
  Serial.println("0 > Cancel the operation");
}

void loop() {
  if (Serial.available() > 0) {
    char input = Serial.read();

    if (input == '1') {
      readDisk(A_DISK);
    }
    else if (input == '2') {
      readDisk(B_DISK);
    }
    else if (input == '3' || input == '4') {
      Disk selectedDisk = (input == '3') ? A_DISK : B_DISK;
      Serial.print("Seçilen disk: ");
      Serial.println((selectedDisk == A_DISK) ? "A" : "B");

      Serial.println("Write data to your disk (max 512 characters):");
      String data = "";
      bool isEmpty = true;

      while (isEmpty) {
        while (Serial.available() == 0);
        data = Serial.readStringUntil('\n');
        data.trim();
        if (data.length() == 0) {
          Serial.println("You cannot enter empty data.");
        } else {
          isEmpty = false;
        }
      }

      Serial.println("Data writed...");
      writeToDisk(selectedDisk, data.c_str());
    }
    else if (input == '5') {
      clearDisk(A_DISK);
    }
    else if (input == '6') {
      clearDisk(B_DISK);
    }
    else if (input == '7') {
      Serial.println("The DOS has been entered into the code environment!");
      char receivedString[100];
      while (receivedString != "EXIT") {
        Serial.readBytesUntil('\n', receivedString, sizeof(receivedString) - 1);
        receivedString[sizeof(receivedString) - 1] = '\0';
        Compiler(receivedString);
      }
    }
    else if (input == '0') {
      Serial.println("The transaction has been cancelled.");
      return;
    }
    else {
      Serial.println("Invalid option. Enter : 1 > A Read, 2 > B Read, 3 > A Write, 4 > B Write, 5 > A Clear, 6 > B Clear, or 0.");
    }
  }
  Serial.read();
}
