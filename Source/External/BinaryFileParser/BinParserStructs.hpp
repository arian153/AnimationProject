/******************************************************************************/
/*!
\file   BinParserStructs.hpp
\author Jason Nollan
\brief
  structs used in the simple bin parser

  Copyright (C) 2011 DigiPen Institute of Technology.
  Reproduction or disclosure of this file or its contents without the
  prior written consent of DigiPen Institute of Technology is prohibited.

*/
/******************************************************************************/
#ifndef BIN_PARSER_STRUCTS_HPP
#define BIN_PARSER_STRUCTS_HPP

#include <vector>
#include <string>

#pragma warning(disable : 4267)

namespace BinParser
{
    struct Vector2
    {
        float x;
        float y;
    };

    struct Vector3
    {
        float x;
        float y;
        float z;
    };

    struct Quaternion
    {
        float x;
        float y;
        float z;
        float s;
    };

    struct Vqs
    {
        Vector3    v;
        Quaternion q;
        float      s;
    };

    struct Vertex
    {
        Vector3                   Position_; //Vertex position in model space
        Vector3                   Normal_; //normal of the faces connected to this vertex
        Vector2                   UV_; //Texture Coordinates
        static const unsigned int sBonesPerVertex = 4; //number of bones that can have an
        //affect on a single vertex
        //this number is usually 4
        unsigned char BoneIndexes_[sBonesPerVertex]; //the 4 Bones that Influnce this Vert
        float         BoneWeights_[sBonesPerVertex]; //the amount of influnce each bone has
        //(the sum of all should be 1.0)
    };

    class Mesh
    {
    public:
        //get methods for Vertexes
        const std::vector<Vertex>& GetVertexes() const
        {
            return Vertexes_;
        }

        const Vertex& GetVertex(unsigned int Index) const
        {
            return Vertexes_[Index];
        }

        unsigned int GetVertexCount() const
        {
            return Vertexes_.size();
        }

        //get methods for Indexes
        const std::vector<unsigned int>& GetIndexes() const
        {
            return Indexes_;
        }

        unsigned int GetIndex(unsigned int Index) const
        {
            return Indexes_[Index];
        }

        unsigned int GetIndexCount() const
        {
            return Indexes_.size();
        }

    private: //data
        std::vector<Vertex>       Vertexes_;
        std::vector<unsigned int> Indexes_; //Indexes into the vertex array
        //every 3 represents a triangle

        friend class Model;
    };

    class Bone
    {
    public:
        const std::string& GetName() const
        {
            return Name_;
        }

        int GetParentID() const
        {
            return ParentID_;
        }

        const Vqs& GetToModelFromBone() const
        {
            return ToModelFromBone_;
        }

        const Vqs& GetToBoneFromModel() const
        {
            return ToBoneFromModel_;
        }

    private: //data
        std::string Name_;         //name assigned to this bone
        int         ParentID_;        //index of parent bone
        Vqs         ToModelFromBone_; //transformation that goes from Bone space in base pose
        //to Model space in base pose
        Vqs ToBoneFromModel_; //transformation that goes from Model space in base pose
        //to Bone space in base pose(inverse of ToModelFromBone_)
        friend class Model;
    };

    class Skeleton
    {
    public:
        //get methods for Bones
        const std::vector<Bone>& GetBones() const
        {
            return Bones_;
        }

        const Bone& GetBone(unsigned int Index) const
        {
            return Bones_[Index];
        }

        unsigned int GetBoneCount() const
        {
            return Bones_.size();
        }

    private: //data
        std::vector<Bone> Bones_; //array of all the bones in the skeleton

        friend class Model;
    };

    class KeyFrame
    {
    public:
        float GetTime() const
        {
            return Time_;
        }

        const Vqs& GetToParentFromBone() const
        {
            return ToParentFromBone_;
        }

    private: //data
        float Time_; //Time (in seconds) where this keyfram takes place
        Vqs   ToParentFromBone_; //Transformation from BoneSpace to parent space of this animation
        //If bone for this keyframe has no parent then this
        //transforms from Bone Space to Model Space
        friend class Model;
    };

    class Track
    {
    public:
        //get methods for KeyFrames
        const std::vector<KeyFrame>& GetKeyFrames() const
        {
            return KeyFrames_;
        }

        const KeyFrame& GetKeyFrame(unsigned int Index) const
        {
            return KeyFrames_[Index];
        }

        unsigned int GetKeyFrameCount() const
        {
            return KeyFrames_.size();
        }

    private: //data
        std::vector<KeyFrame> KeyFrames_;

        friend class Model;
    };

    class Animation
    {
    public:
        float GetDuration() const
        {
            return Duration_;
        }

        //get methods for Tracks
        const std::vector<Track>& GetTracks() const
        {
            return Tracks_;
        }

        const Track& GetTrack(unsigned int Index) const
        {
            return Tracks_[Index];
        }

        unsigned int GetTrackCount() const
        {
            return Tracks_.size();
        }

    private: //data
        float              Duration_; //how long (in seconds) the animation lasts
        std::vector<Track> Tracks_; //Animation track for each bone
        //(Array size should be equal to Array size of bones)
        friend class Model;
    };
}//namespace BinParser

#endif //#ifndef BIN_PARSER_STRUCTS_HPP
