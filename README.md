# ArDOS
We can now perform EEPROM operations and write programs via Arduino serial communication.
# Arduino EEPROM-Based File System User Guide

## Overview
This program provides a simple file system for Arduino, utilizing the EEPROM memory for data storage. The system supports two virtual disks, basic file management, and RAM variable storage. Below, you'll find details on the setup, functionality, and usage of this system.

---

## Features

### 1. Two Virtual Disks
- **`A_DISK`** and **`B_DISK`** represent two separate memory regions within the EEPROM.
- Each disk has a defined size (`DISK_SIZE`), and their start addresses are pre-configured.

### 2. File Storage
- Files are identified by a start marker (`FILE_START`) and stored sequentially in EEPROM.
- The system ensures files do not overlap and checks available space before writing.

### 3. RAM Variables
- A small array (`ram[32]`) is used for temporary variable storage during execution.

### 4. Dynamic Writing and Reading
- Functions are available for adding data to disks and retrieving it efficiently.

---

## Functionality

### Key Constants and Variables
- **`DISK_SIZE`**: Defines the size of each disk.
- **`A_START`, `B_START`**: EEPROM start addresses for `A_DISK` and `B_DISK`.
- **`FILE_START`**: Special marker indicating the start of a file in EEPROM.

### Important Functions

#### 1. `writeToDisk(Disk disk, const char* data)`
Writes a string (`data`) to the specified disk (`A_DISK` or `B_DISK`).
- Checks for sufficient free space and avoids overwriting existing data.
- Adds a marker to signify the start of the file.

#### 2. `readFromDisk(Disk disk, char* buffer, int bufferSize)`
Reads stored data from the specified disk.
- Outputs the result into the provided `buffer` for further use.

#### 3. `clearDisk(Disk disk)`
Erases all data on the specified disk by resetting it.

---

## Setup Instructions

### Hardware Requirements
- Arduino board with sufficient EEPROM (e.g., Arduino Uno).
- USB cable and Arduino IDE installed on your system.

### Uploading the Code
1. Open the `.ino` file in the Arduino IDE.
2. Connect your Arduino board via USB.
3. Verify and upload the code to the board.

### Testing the File System
- Open the **Serial Monitor** from the Arduino IDE.
- Use predefined functions (like `writeToDisk` and `readFromDisk`) to interact with the system.

---

## Usage

### Writing Data to a Disk
```cpp
writeToDisk(A_DISK, "Hello World");
```
- Writes the string `"Hello World"` to `A_DISK`.
- Automatically checks and prevents overwriting other data.

### Reading Data from a Disk
```cpp
char buffer[100];
readFromDisk(A_DISK, buffer, 100);
Serial.println(buffer);
```
- Reads data stored in `A_DISK` into a buffer.
- Prints the data to the Serial Monitor.

### Clearing a Disk
```cpp
clearDisk(B_DISK);
```
- Erases all stored data on `B_DISK`.

---

## Limitations

### 1. EEPROM Wear
- EEPROM has a limited write cycle (~100,000 cycles per memory location). Avoid excessive write operations.

### 2. Limited Storage
- Storage capacity is constrained by EEPROM size and `DISK_SIZE` settings.

### 3. No File Metadata
- Files are stored sequentially without additional metadata like names or timestamps.

---

## Customization

### Adjust Disk Sizes
- Modify `DISK_SIZE`, `A_START`, or `B_START` to suit your application needs.

### Expand Functionality
- Add functions for metadata handling, multiple file markers, or advanced error checking.

---

## Troubleshooting

### File Overlap
- Ensure adequate space is available before writing data.
- Use `clearDisk` to reset a disk if overlapping occurs.

### Unreadable Data
- Check if `readFromDisk` is using a sufficiently large buffer.
- Ensure the disk wasn’t cleared or corrupted.
