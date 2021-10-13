/******************************************************************************/
/*!
\file   BinParserModel.cpp
\author Jason Nollan
\brief
  class that can load binary models exported from Chris Peters' converter

  Copyright (C) 2011 DigiPen Institute of Technology.
  Reproduction or disclosure of this file or its contents without the
  prior written consent of DigiPen Institute of Technology is prohibited.

*/
/******************************************************************************/

#include "BinParserModel.hpp"

namespace BinParser
{
    bool Model::LoadFile(const char* FileName)
    {
        //try opening the file
        FileObject* pFile = FileObject::OpenBinaryRead(FileName);
        if (!pFile)
        {
            printf("Invalid File Name: %s\n", FileName);
            return false;
        }

        //read in file version
        unsigned int FileVersion;
        pFile->Read(FileVersion);
        if (FileVersion != 'dpm2')
        {
            printf("File Version Mismatch\n");
            delete pFile;
            return false;
        }
        unsigned int SectionType;
        unsigned int SectionSize;
        pFile->Read(SectionSize);

        while (true)
        {
            //read in the section type and size
            pFile->Read(SectionType);
            pFile->Read(SectionSize);
            if (pFile->IsAtEndOfFile())
                break;

            switch (SectionType)
            {
            case 'mesh':
                {
                    if (!ReadMesh(pFile)) //make sure mesh loaded properly
                    {
                        delete pFile;
                        return false;
                    }
                    //4 bytes for section type
                    //4 bytes for section size
                    //pFile->Seek(SectionSize - 8);
                }
                break;
            case 'skel':
                {
                    ReadSkeleton(pFile);
                }
                break;
            case 'anim':
                {
                    ReadAnimation(pFile);
                }
                break;
            default:
                {
                    printf("Unknown Section Type\n");
                    delete pFile;
                    return false;
                }
            }
        }

        //done reading in file
        delete pFile;

        return true;
    }

    bool Model::ReadMesh(FileObject* pFile)
    {
        //read the vertex type from the file
        unsigned int VertexType;
        pFile->Read(VertexType);
        switch (VertexType)
        {
        case 0: //VertexTypeDefault
            printf("Model has no skeleton\n");
            return false;
        case 1: //VertexTypeSkin
            break;
        default:
            printf("Invalid Vertex Type\n");
            return false;
        }

        //read in Indices
        unsigned int IndexCount;
        pFile->Read(IndexCount);
        Mesh_.Indexes_.resize(IndexCount);
        pFile->Read(Mesh_.Indexes_[0], IndexCount);

        //read in verts
        unsigned int VertCount;
        pFile->Read(VertCount);
        Mesh_.Vertexes_.resize(VertCount);

        for (unsigned int i = 0; i < VertCount; ++i)
        {
            Vertex& vert = Mesh_.Vertexes_[i];

            pFile->Read(vert.Position_.x, 3); //read position
            pFile->Read(vert.Normal_.x, 3); //read normal
            pFile->Read(vert.UV_.x, 2); //read UV data
            pFile->Read(vert.BoneWeights_[0], 4); //read bone weights
            pFile->Read(vert.BoneIndexes_[0], 4); //read bone Indexes
        } //finished read vertexes
        return true;
    }

    void Model::ReadSkeleton(FileObject* pFile)
    {
        //read in number of bones
        unsigned int BoneCount;
        pFile->Read(BoneCount);
        Skeleton_.Bones_.resize(BoneCount);

        //loop through bones
        for (unsigned int i = 0; i < BoneCount; ++i)
        {
            Bone& bone = Skeleton_.Bones_[i];
            pFile->Read(bone.Name_); //read in Bone Name
            pFile->Read(bone.ParentID_); //read in parent index
            ReadVqs(pFile, bone.ToModelFromBone_); //read in Bind VQS
            ReadVqs(pFile, bone.ToBoneFromModel_); //read in inverse bind VQS
        } //done reading in skeleton
    }

    void Model::ReadAnimation(FileObject* pFile)
    {
        Animations_.push_back(Animation());
        Animation& anim = Animations_.back();
        //read in animation duration
        pFile->Read(anim.Duration_);
        //read in nubmer of tracks
        unsigned int TrackCount;
        pFile->Read(TrackCount);
        anim.Tracks_.resize(TrackCount);
        //loop through and read in tracks
        for (unsigned int i = 0; i < TrackCount; ++i)
        {
            Track& track = anim.Tracks_[i];
            //read in number of key frames
            unsigned int KeyFrameCount;
            pFile->Read(KeyFrameCount);
            track.KeyFrames_.resize(KeyFrameCount);
            //loop through and read in key frames
            for (unsigned int f = 0; f < KeyFrameCount; ++f)
            {
                KeyFrame& frame = track.KeyFrames_[f];
                pFile->Read(frame.Time_); //read in frame time
                //read in bone to parent transform
                ReadVqs(pFile, frame.ToParentFromBone_);
            } //end reading in key frames
        } //end reading in tracks
    }

    void Model::ReadVqs(FileObject* pFile, Vqs& vqs)
    {
        pFile->Read(vqs.v.x, 3); //read in Position
        //read in Rotation (quaternion) (guessing x,y,z,s)
        pFile->Read(vqs.q.x, 4);
        vqs.s = 1.0f; //set scale to identity (1)
    }
}//namespace BinParser
