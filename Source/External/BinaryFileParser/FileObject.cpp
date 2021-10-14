/******************************************************************************/
/*!
\file   FileObject.cpp
\author Jason Nollan
\brief
  wrapper around file loading functions

  Copyright (C) 2011 DigiPen Institute of Technology.
  Reproduction or disclosure of this file or its contents without the
  prior written consent of DigiPen Institute of Technology is prohibited.

*/
/******************************************************************************/
#pragma once

#pragma warning(disable : 4244)

#include "FileObject.hpp"
#include <string>

FileObject* FileObject::OpenBinaryRead(const char* FileName)
{
    FILE* pFile = fopen(FileName, "rb");
    if (!pFile)
        return nullptr;
    FileObject* pFileObject = new FileObject;
    pFileObject->pFile_     = pFile;
    return pFileObject;
}

FileObject* FileObject::OpenBinaryWrite(const char* FileName)
{
    FILE* pFile = fopen(FileName, "wb");
    if (!pFile)
        return nullptr;
    FileObject* pFileObject = new FileObject;
    pFileObject->pFile_     = pFile;
    return pFileObject;
}

FileObject::~FileObject(void)
{
    fclose(pFile_);
}

void FileObject::Read(std::string& obj)
{
    char          String[256];
    unsigned char StringLen;
    this->Read(StringLen);
    this->Read(String[0], StringLen);
    obj.assign(String, StringLen);
}

void FileObject::Write(std::string& obj)
{
    unsigned char StringLen = static_cast<unsigned int>(obj.length());
    this->Write(StringLen);
    this->Write(obj.c_str()[0], StringLen);
}

bool FileObject::IsAtEndOfFile() const
{
    return (feof(pFile_) != 0);
}

void FileObject::Seek(int Offset) const
{
    fseek(pFile_, Offset, SEEK_CUR);
}

unsigned int FileObject::StartChunk(unsigned int Mark)
{
    unsigned int ChunkStartPos = ftell(pFile_);
    this->Write(Mark);
    this->Write(Mark);
    return ChunkStartPos;
}

void FileObject::EndChunk(unsigned int Pos)
{
    unsigned int CurPos = ftell(pFile_);
    unsigned int Size   = CurPos - Pos;
    fseek(pFile_, Pos + 4, SEEK_SET);
    fwrite(&Size, sizeof(unsigned int), 1, pFile_);
    fseek(pFile_, CurPos, SEEK_SET);
}

FileObject::FileObject(void)
    : pFile_(NULL)
{
}
