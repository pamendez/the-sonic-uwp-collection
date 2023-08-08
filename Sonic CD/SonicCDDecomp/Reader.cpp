#include "RetroEngine.hpp"
#include <string>

char rsdkName[0x400];

char fileName[0x100];
byte fileBuffer[0x2000];
int fileSize;
int vFileSize;
int readPos;
int readSize;
int bufferPosition;
int virtualFileOffset;
byte eStringPosA;
byte eStringPosB;
byte eStringNo;
byte eNybbleSwap;
char encryptionStringA[] = { "4RaS9D7KaEbxcp2o5r6t" };
char encryptionStringB[] = { "3tRaUxLmEaSn" };

FileIO *cFileHandle = nullptr;
FileIO *cFileHandleStream = nullptr;

bool CheckRSDKFile(const char *filePath)
{
    FileInfo info;

    Engine.usingDataFile = false;
    Engine.usingBytecode = false;

    cFileHandle = fOpen(filePath, "rb");
    if (cFileHandle) {
        Engine.usingDataFile = true;
        StrCopy(rsdkName, filePath);
        fClose(cFileHandle);
        cFileHandle = NULL;
        if (LoadFile("Data/Scripts/ByteCode/GlobalCode.bin", &info)) {
            Engine.usingBytecode = true;
            Engine.bytecodeMode  = BYTECODE_MOBILE;
            CloseFile();
        }
        else if (LoadFile("Data/Scripts/ByteCode/GS000.bin", &info)) {
            Engine.usingBytecode = true;
            Engine.bytecodeMode  = BYTECODE_PC;
            CloseFile();
        }
        return true;
    }
    else {
        Engine.usingDataFile = false;
        cFileHandle = NULL;
        if (LoadFile("Data/Scripts/ByteCode/GlobalCode.bin", &info)) {
            Engine.usingBytecode = true;
            Engine.bytecodeMode  = BYTECODE_MOBILE;
            CloseFile();
        }
        else if (LoadFile("Data/Scripts/ByteCode/GS000.bin", &info)) {
            Engine.usingBytecode = true;
            Engine.bytecodeMode  = BYTECODE_PC;
            CloseFile();
        }
        return false;
    }

    return false;
}

bool LoadFile(const char *filePath, FileInfo *fileInfo)
{
    MEM_ZEROP(fileInfo);
    StrCopy(fileInfo->fileName, filePath);
    StrCopy(fileName, filePath);

    if (cFileHandle)
        fClose(cFileHandle);

    cFileHandle = NULL;

    if (Engine.usingDataFile) {
        cFileHandle = fOpen(rsdkName, "rb");
        fSeek(cFileHandle, 0, SEEK_END);
        fileSize       = (int)fTell(cFileHandle);
        bufferPosition = 0;
        readSize       = 0;
        readPos        = 0;
        if (!ParseVirtualFileSystem(fileInfo)) {
            fClose(cFileHandle);
            cFileHandle = NULL;
            printLog("Couldn't load file '%s'", filePath);
            return false;
        }
        fileInfo->readPos           = readPos;
        fileInfo->fileSize          = vFileSize;
        fileInfo->virtualFileOffset = virtualFileOffset;
        fileInfo->eStringNo         = eStringNo;
        fileInfo->eStringPosB       = eStringPosB;
        fileInfo->eStringPosA       = eStringPosA;
        fileInfo->eNybbleSwap       = eNybbleSwap;
        fileInfo->bufferPosition    = bufferPosition;
    }
    else {
        cFileHandle = fOpen(fileInfo->fileName, "rb");
        if (!cFileHandle) {
            printLog("Couldn't load file '%s'", filePath);
            return false;
        }
        virtualFileOffset = 0;
        fSeek(cFileHandle, 0, SEEK_END);
        fileInfo->fileSize = (int)fTell(cFileHandle);
        fileSize           = fileInfo->fileSize;
        fSeek(cFileHandle, 0, SEEK_SET);
        readPos = 0;
        fileInfo->readPos           = readPos;
        fileInfo->virtualFileOffset = 0;
        fileInfo->eStringNo         = 0;
        fileInfo->eStringPosB       = 0;
        fileInfo->eStringPosA       = 0;
        fileInfo->eNybbleSwap       = 0;
        fileInfo->bufferPosition    = 0;
    }
    bufferPosition = 0;
    readSize       = 0;

    printLog("Loaded File '%s'", filePath);

    return true;
}

bool ParseVirtualFileSystem(FileInfo *fileInfo)
{
    char filename[0x50];
    char fullFilename[0x50];
    char stringBuffer[0x50];
    ushort dirCount = 0;
    int fileOffset  = 0;
    int fNamePos    = 0;
    int headerSize  = 0;
    int i           = 0;
    byte fileBuffer = 0;

    int j             = 0;
    virtualFileOffset = 0;
    for (int i = 0; fileInfo->fileName[i]; i++) {
        if (fileInfo->fileName[i] == '/') {
            fNamePos = i;
            j        = 0;
        }
        else {
            ++j;
        }
        fullFilename[i] = fileInfo->fileName[i];
    }
    ++fNamePos;
    for (i = 0; i < j; ++i) filename[i] = fileInfo->fileName[i + fNamePos];
    filename[j]            = 0;
    fullFilename[fNamePos] = 0;

    fSeek(cFileHandle, 0, SEEK_SET);
    Engine.usingDataFile = false;
    bufferPosition       = 0;
    readSize             = 0;
    readPos              = 0;

    FileRead(&fileBuffer, 1);
    headerSize = fileBuffer;
    FileRead(&fileBuffer, 1);
    headerSize += fileBuffer << 8;
    FileRead(&fileBuffer, 1);
    headerSize += fileBuffer << 16;
    FileRead(&fileBuffer, 1);
    headerSize += fileBuffer << 24;

    FileRead(&fileBuffer, 1);
    dirCount = fileBuffer;
    FileRead(&fileBuffer, 1);
    dirCount += fileBuffer << 8;

    i          = 0;
    fileOffset = 0;
    int nextFileOffset = 0;
    while (i < dirCount) {
        FileRead(&fileBuffer, 1);
        for (j = 0; j < fileBuffer; ++j) {
            FileRead(&stringBuffer[j], 1);
            stringBuffer[j] ^= -1 - fileBuffer;
        }
        stringBuffer[j] = 0;

        if (StrComp(fullFilename, stringBuffer)) {
            FileRead(&fileBuffer, 1);
            fileOffset = fileBuffer;
            FileRead(&fileBuffer, 1);
            fileOffset += fileBuffer << 8;
            FileRead(&fileBuffer, 1);
            fileOffset += fileBuffer << 16;
            FileRead(&fileBuffer, 1);
            fileOffset += fileBuffer << 24;

            //Grab info for next dir to know when we've found an error
            //Ignore dir name we dont care
            if (i == dirCount - 1) {
                nextFileOffset = fileSize - headerSize; //There is no next dir, so just make this the EOF
            }
            else {
                FileRead(&fileBuffer, 1);
                for (j = 0; j < fileBuffer; ++j) {
                    FileRead(&stringBuffer[j], 1);
                    stringBuffer[j] ^= -1 - fileBuffer;
                }
                stringBuffer[j] = 0;

                FileRead(&fileBuffer, 1);
                nextFileOffset = fileBuffer;
                FileRead(&fileBuffer, 1);
                nextFileOffset += fileBuffer << 8;
                FileRead(&fileBuffer, 1);
                nextFileOffset += fileBuffer << 16;
                FileRead(&fileBuffer, 1);
                nextFileOffset += fileBuffer << 24;
            }

            i = dirCount;
        }
        else {
            fileOffset = -1;
            FileRead(&fileBuffer, 1);
            FileRead(&fileBuffer, 1);
            FileRead(&fileBuffer, 1);
            FileRead(&fileBuffer, 1);
            ++i;
        }
    }

    if (fileOffset == -1) {
        Engine.usingDataFile = true;
        return false;
    }
    else {
        fSeek(cFileHandle, fileOffset + headerSize, SEEK_SET);
        bufferPosition    = 0;
        readSize          = 0;
        readPos           = 0;
        virtualFileOffset = fileOffset + headerSize;
        i                 = 0;
        while (i < 1) {
            FileRead(&fileBuffer, 1);
            ++virtualFileOffset;
            j = 0;
            while (j < fileBuffer) {
                FileRead(&stringBuffer[j], 1);
                stringBuffer[j] = ~stringBuffer[j];
                ++j;
                ++virtualFileOffset;
            }
            stringBuffer[j] = 0;

            if (StrComp(filename, stringBuffer)) {
                i = 1;
                FileRead(&fileBuffer, 1);
                j = fileBuffer;
                FileRead(&fileBuffer, 1);
                j += fileBuffer << 8;
                FileRead(&fileBuffer, 1);
                j += fileBuffer << 16;
                FileRead(&fileBuffer, 1);
                j += fileBuffer << 24;
                virtualFileOffset += 4;
                vFileSize = j;
            }
            else {
                FileRead(&fileBuffer, 1);
                j = fileBuffer;
                FileRead(&fileBuffer, 1);
                j += fileBuffer << 8;
                FileRead(&fileBuffer, 1);
                j += fileBuffer << 16;
                FileRead(&fileBuffer, 1);
                j += fileBuffer << 24;
                virtualFileOffset += 4;
                virtualFileOffset += j;
            }

            //No File has been found (next file would be in a new dir)
            if (virtualFileOffset >= nextFileOffset + headerSize) {
                Engine.usingDataFile = true;
                return false;
            }
            fSeek(cFileHandle, virtualFileOffset, SEEK_SET);
            bufferPosition = 0;
            readSize       = 0;
            readPos        = virtualFileOffset;
        }
        eStringNo            = (vFileSize & 0x1FCu) >> 2;
        eStringPosB          = (eStringNo % 9) + 1;
        eStringPosA          = (eStringNo % eStringPosB) + 1;
        eNybbleSwap          = false;
        Engine.usingDataFile = true;
        return true;
    }
    //Engine.usingDataFile = true;
    return false;
}

void FileRead(void *dest, int size)
{
    byte *data = (byte *)dest;

    if (readPos <= fileSize) {
        if (Engine.usingDataFile) {
            while (size > 0) {
                if (bufferPosition == readSize)
                    FillFileBuffer();

                *data = encryptionStringB[eStringPosB] ^ eStringNo ^ fileBuffer[bufferPosition++];
                if (eNybbleSwap)
                    *data = 16 * (*data & 0xF) + ((signed int)*data >> 4);
                *data ^= encryptionStringA[eStringPosA++];
                ++eStringPosB;
                if (eStringPosA <= 19 || eStringPosB <= 11) {
                    if (eStringPosA > 19) {
                        eStringPosA = 1;
                        eNybbleSwap ^= 1u;
                    }
                    if (eStringPosB > 11) {
                        eStringPosB = 1;
                        eNybbleSwap ^= 1u;
                    }
                }
                else {
                    ++eStringNo;
                    eStringNo &= 0x7Fu;
                    if (eNybbleSwap) {
                        eNybbleSwap = 0;
                        eStringPosA = (eStringNo % 12) + 6;
                        eStringPosB = (eStringNo % 5) + 4;
                    }
                    else {
                        eNybbleSwap = 1;
                        eStringPosA = (eStringNo % 15) + 3;
                        eStringPosB = (eStringNo % 7) + 1;
                    }
                }
                ++data;
                --size;
            }
        }
        else {
            while (size > 0) {
                if (bufferPosition == readSize)
                    FillFileBuffer();

                *data++ = fileBuffer[bufferPosition++];
                size--;
            }
        }
    }
}

void SetFileInfo(FileInfo *fileInfo)
{
    if (Engine.usingDataFile) {
        cFileHandle       = fOpen(rsdkName, "rb");
        virtualFileOffset = fileInfo->virtualFileOffset;
        vFileSize         = fileInfo->fileSize;
        fSeek(cFileHandle, 0, SEEK_END);
        fileSize = (int)fTell(cFileHandle);
        readPos  = fileInfo->readPos;
        fSeek(cFileHandle, readPos, SEEK_SET);
        FillFileBuffer();
        bufferPosition = fileInfo->bufferPosition;
        eStringPosA    = fileInfo->eStringPosA;
        eStringPosB    = fileInfo->eStringPosB;
        eStringNo      = fileInfo->eStringNo;
        eNybbleSwap    = fileInfo->eNybbleSwap;
    }
    else {
        StrCopy(fileName, fileInfo->fileName);
        cFileHandle       = fOpen(fileInfo->fileName, "rb");
        virtualFileOffset = 0;
        fileSize          = fileInfo->fileSize;
        readPos           = fileInfo->readPos;
        fSeek(cFileHandle, readPos, SEEK_SET);
        FillFileBuffer();
        bufferPosition = fileInfo->bufferPosition;
        eStringPosA    = 0;
        eStringPosB    = 0;
        eStringNo      = 0;
        eNybbleSwap    = 0;
    }
}

size_t GetFilePosition()
{
    if (Engine.usingDataFile)
        return bufferPosition + readPos - readSize - virtualFileOffset;
    else
        return bufferPosition + readPos - readSize;
}

void SetFilePosition(int newPos)
{
    if (Engine.usingDataFile) {
        readPos     = virtualFileOffset + newPos;
        eStringNo   = (vFileSize & 0x1FCu) >> 2;
        eStringPosB = (eStringNo % 9) + 1;
        eStringPosA = (eStringNo % eStringPosB) + 1;
        eNybbleSwap = false;
        while (newPos) {
            ++eStringPosA;
            ++eStringPosB;
            if (eStringPosA <= 19 || eStringPosB <= 11) {
                if (eStringPosA > 19) {
                    eStringPosA = 1;
                    eNybbleSwap ^= 1u;
                }
                if (eStringPosB > 11) {
                    eStringPosB = 1;
                    eNybbleSwap ^= 1u;
                }
            }
            else {
                ++eStringNo;
                eStringNo &= 0x7Fu;
                if (eNybbleSwap) {
                    eNybbleSwap = false;
                    eStringPosA = (eStringNo % 12) + 6;
                    eStringPosB = (eStringNo % 5) + 4;
                }
                else {
                    eNybbleSwap = true;
                    eStringPosA = (eStringNo % 15) + 3;
                    eStringPosB = (eStringNo % 7) + 1;
                }
            }
            --newPos;
        }
    }
    else {
        readPos = newPos;
    }
    fSeek(cFileHandle, readPos, SEEK_SET);
    FillFileBuffer();
}

bool ReachedEndOfFile()
{
    if (Engine.usingDataFile)
        return bufferPosition + readPos - readSize - virtualFileOffset >= vFileSize;
    else
        return bufferPosition + readPos - readSize >= fileSize;
}

size_t FileRead2(FileInfo *info, void *dest, int size)
{
    byte *data = (byte *)dest;
    int rPos   = (int)GetFilePosition2(info);
    memset(data, 0, size);

    if (rPos <= info->fileSize) {
        if (Engine.usingDataFile) {
            int rSize = 0;
            if (rPos + size <= info->fileSize)
                rSize = size;
            else
                rSize = info->fileSize - rPos;

            size_t result = fRead(data, 1u, rSize, cFileHandleStream);
            info->readPos += rSize;
            info->bufferPosition = 0;

            while (size > 0) {
                *data = encryptionStringB[info->eStringPosB] ^ info->eStringNo ^ *data;
                if (info->eNybbleSwap)
                    *data = 16 * (*data & 0xF) + (*data >> 4);
                *data ^= encryptionStringA[info->eStringPosA++];
                ++info->eStringPosB;
                if (info->eStringPosA <= 19 || info->eStringPosB <= 11) {
                    if (info->eStringPosA > 19) {
                        info->eStringPosA = 1;
                        info->eNybbleSwap ^= 1u;
                    }
                    if (info->eStringPosB > 11) {
                        info->eStringPosB = 1;
                        info->eNybbleSwap ^= 1u;
                    }
                }
                else {
                    ++info->eStringNo;
                    info->eStringNo &= 0x7Fu;
                    if (info->eNybbleSwap) {
                        info->eNybbleSwap = false;
                        info->eStringPosA = (info->eStringNo % 12) + 6;
                        info->eStringPosB = (info->eStringNo % 5) + 4;
                    }
                    else {
                        info->eNybbleSwap = true;
                        info->eStringPosA = (info->eStringNo % 15) + 3;
                        info->eStringPosB = (info->eStringNo % 7) + 1;
                    }
                }
                ++data;
                --size;
            }
            return result;
        }
        else {
            int rSize = 0;
            if (rPos + size <= info->fileSize)
                rSize = size;
            else
                rSize = info->fileSize - rPos;

            size_t result = fRead(data, 1u, rSize, cFileHandleStream);
            info->readPos += rSize;
            info->bufferPosition = 0;
            return result;
        }
    }
    return 0;
}

size_t GetFilePosition2(FileInfo* info)
{
    if (Engine.usingDataFile)
        return info->bufferPosition + info->readPos - info->virtualFileOffset;
    else
        return info->bufferPosition + info->readPos;
}

void SetFilePosition2(FileInfo *info, int newPos)
{
    if (Engine.usingDataFile) {
        info->readPos     = info->virtualFileOffset + newPos;
        info->eStringNo   = (info->fileSize & 0x1FCu) >> 2;
        info->eStringPosB = (info->eStringNo % 9) + 1;
        info->eStringPosA = (info->eStringNo % info->eStringPosB) + 1;
        info->eNybbleSwap = false;
        while (newPos) {
            ++info->eStringPosA;
            ++info->eStringPosB;
            if (info->eStringPosA <= 19 || info->eStringPosB <= 11) {
                if (info->eStringPosA > 19) {
                    info->eStringPosA = 1;
                    info->eNybbleSwap ^= 1u;
                }
                if (info->eStringPosB > 11) {
                    info->eStringPosB = 1;
                    info->eNybbleSwap ^= 1u;
                }
            }
            else {
                ++info->eStringNo;
                info->eStringNo &= 0x7Fu;
                if (info->eNybbleSwap) {
                    info->eNybbleSwap = false;
                    info->eStringPosA = (info->eStringNo % 12) + 6;
                    info->eStringPosB = (info->eStringNo % 5) + 4;
                }
                else {
                    info->eNybbleSwap = true;
                    info->eStringPosA = (info->eStringNo % 15) + 3;
                    info->eStringPosB = (info->eStringNo % 7) + 1;
                }
            }
            --newPos;
        }
    }
    else {
        info->readPos = newPos;
    }
    fSeek(cFileHandleStream, info->readPos, SEEK_SET);
}
