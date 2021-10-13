/******************************************************************************/
/*!
\file   FileObject.hpp
\author Jason Nollan
\brief
  wrapper around file loading functions

  Copyright (C) 2011 DigiPen Institute of Technology.
  Reproduction or disclosure of this file or its contents without the
  prior written consent of DigiPen Institute of Technology is prohibited.

*/
/******************************************************************************/
#ifndef FILE_OBJECT_HPP
#define FILE_OBJECT_HPP

#include <cstdio>
#include <string>

class FileObject
{
public:
    static FileObject* OpenBinaryRead(const char* FileName);
    static FileObject* OpenBinaryWrite(const char* FileName);
    ~FileObject(void);

    template <class T>
    void Read(T& obj)
    {
        fread(&obj, sizeof(obj), 1, pFile_);
    }

    void Read(std::string& obj);

    template <class T>
    void Read(T& obj, unsigned int Count)
    {
        fread(&obj, sizeof(obj), Count, pFile_);
    }

    template <class T>
    void Write(T& obj)
    {
        fwrite(&obj, sizeof(obj), 1, pFile_);
    }

    void Write(std::string& obj);

    template <class T>
    void Write(T& obj, unsigned int Count)
    {
        fwrite(&obj, sizeof(obj), Count, pFile_);
    }

    void         Seek(int Offset) const;
    unsigned int StartChunk(unsigned int Mark);
    void         EndChunk(unsigned int Pos);
    bool         IsAtEndOfFile() const;
private:
    FileObject(void);

private: //Data
    FILE* pFile_;
};

#endif //#ifndef FILE_OBJECT_HPP
