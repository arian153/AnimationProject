/******************************************************************************/
/*!
\file   BinParserModel.hpp
\author Jason Nollan
\brief
  class that can load binary models exported from Chris Peters' converter

  Copyright (C) 2011 DigiPen Institute of Technology.
  Reproduction or disclosure of this file or its contents without the
  prior written consent of DigiPen Institute of Technology is prohibited.

*/
/******************************************************************************/
#ifndef BIN_PARSER_MODEL_HPP
#define BIN_PARSER_MODEL_HPP

#pragma warning(disable : 4267)
#include "BinParserStructs.hpp"
#include <vector>
#include "FileObject.hpp"

namespace BinParser
{
    class Model
    {
    public:
        bool LoadFile(const char* FileName);

        const Mesh& GetMesh() const
        {
            return Mesh_;
        }

        const Skeleton& GetSkeleton() const
        {
            return Skeleton_;
        }

        //get methods for Animations
        const std::vector<Animation>& GetAnimations() const
        {
            return Animations_;
        }

        const Animation& GetAnimation(unsigned int Index) const
        {
            return Animations_[Index];
        }

        unsigned int GetAnimationCount() const
        {
            return Animations_.size();
        }

    private:
        bool ReadMesh(FileObject* pFile);
        void ReadSkeleton(FileObject* pFile);
        void ReadAnimation(FileObject* pFile);
        void ReadVqs(FileObject* pFile, Vqs& vqs);
    private: //Data
        Mesh                   Mesh_; //contains the visible portion of the object
        Skeleton               Skeleton_;
        std::vector<Animation> Animations_;
    };
} //namespace BinParser

#endif //#ifndef BIN_PARSER_MODEL_HPP
