#pragma once
#include <string>
#include <unordered_map>
#include "../../Math/Utility/MathDef.hpp"
#include "../../Math/Algebra/Vector3.hpp"
#include "../../Math/Algebra/Quaternion.hpp"

namespace CS460
{
    class BlackBoard
    {
    public:
        BlackBoard();
        ~BlackBoard();

        bool   HasKey(const std::string& key);
        void   Clear();
        size_t Size() const;

        void SetBoolean(const std::string& key, bool data);
        void SetInteger(const std::string& key, int data);
        void SetPositive(const std::string& key, size_t data);
        void SetReal(const std::string& key, Real data);
        void SetVector(const std::string& key, const Vector3& data);
        void SetQuaternion(const std::string& key, const Quaternion& data);
        void SetString(const std::string& key, const std::string& data);

        bool        GetBoolean(const std::string& key);
        int         GetInteger(const std::string& key);
        size_t      GetPositive(const std::string& key);
        Real        GetReal(const std::string& key);
        Vector3     GetVector(const std::string& key);
        Quaternion  GetQuaternion(const std::string& key);
        std::string GetString(const std::string& key);

        bool&        GetReferencedBoolean(const std::string& key);
        int&         GetReferencedInteger(const std::string& key);
        size_t&      GetReferencedPositive(const std::string& key);
        Real&        GetReferencedReal(const std::string& key);
        Vector3&     GetReferencedVector(const std::string& key);
        Quaternion&  GetReferencedQuaternion(const std::string& key);
        std::string& GetReferencedString(const std::string& key);

        bool HasBoolean(const std::string& key);
        bool HasInteger(const std::string& key);
        bool HasPositive(const std::string& key);
        bool HasReal(const std::string& key);
        bool HasVector(const std::string& key);
        bool HasQuaternion(const std::string& key);
        bool HasString(const std::string& key);

        size_t SizeOfBoolean() const;
        size_t SizeOfInteger() const;
        size_t SizeOfPositive() const;
        size_t SizeOfReal() const;
        size_t SizeOfVector() const;
        size_t SizeOfQuaternion() const;
        size_t SizeOfString() const;

    private:
        std::unordered_map<std::string, bool>        m_booleans;
        std::unordered_map<std::string, int>         m_integers;
        std::unordered_map<std::string, size_t>      m_positives;
        std::unordered_map<std::string, Real>        m_reals;
        std::unordered_map<std::string, Vector3>     m_vectors;
        std::unordered_map<std::string, Quaternion>  m_quaternions;
        std::unordered_map<std::string, std::string> m_strings;
    };
}
