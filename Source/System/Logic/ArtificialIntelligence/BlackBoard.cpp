#include "BlackBoard.hpp"

namespace CS460
{
    BlackBoard::BlackBoard()
    {
    }

    BlackBoard::~BlackBoard()
    {
    }

    bool BlackBoard::HasKey(const std::string& key)
    {
        return m_booleans.find(key) != m_booleans.end() ||
                m_integers.find(key) != m_integers.end() ||
                m_positives.find(key) != m_positives.end() ||
                m_reals.find(key) != m_reals.end() ||
                m_vectors.find(key) != m_vectors.end() ||
                m_quaternions.find(key) != m_quaternions.end() ||
                m_strings.find(key) != m_strings.end();
    }

    void BlackBoard::Clear()
    {
        m_booleans.clear();
        m_integers.clear();
        m_positives.clear();
        m_reals.clear();
        m_vectors.clear();
        m_quaternions.clear();
        m_strings.clear();
    }

    size_t BlackBoard::Size() const
    {
        return m_booleans.size() +
                m_integers.size() +
                m_positives.size() +
                m_reals.size() +
                m_vectors.size() +
                m_quaternions.size() +
                m_strings.size();
    }

    void BlackBoard::SetBoolean(const std::string& key, bool data)
    {
        m_booleans[key] = data;
    }

    void BlackBoard::SetInteger(const std::string& key, int data)
    {
        m_integers[key] = data;
    }

    void BlackBoard::SetPositive(const std::string& key, size_t data)
    {
        m_positives[key] = data;
    }

    void BlackBoard::SetReal(const std::string& key, Real data)
    {
        m_reals[key] = data;
    }

    void BlackBoard::SetVector(const std::string& key, const Vector3& data)
    {
        m_vectors[key] = data;
    }

    void BlackBoard::SetQuaternion(const std::string& key, const Quaternion& data)
    {
        m_quaternions[key] = data;
    }

    void BlackBoard::SetString(const std::string& key, const std::string& data)
    {
        m_strings[key] = data;
    }

    bool BlackBoard::GetBoolean(const std::string& key)
    {
        if (m_booleans.find(key) == m_booleans.end())
        {
            m_booleans.emplace(key, false);
        }
        return m_booleans[key];
    }

    int BlackBoard::GetInteger(const std::string& key)
    {
        if (m_integers.find(key) == m_integers.end())
        {
            m_integers.emplace(key, 0);
        }
        return m_integers[key];
    }

    size_t BlackBoard::GetPositive(const std::string& key)
    {
        if (m_positives.find(key) == m_positives.end())
        {
            m_positives.emplace(key, 0);
        }
        return m_positives[key];
    }

    Real BlackBoard::GetReal(const std::string& key)
    {
        if (m_reals.find(key) == m_reals.end())
        {
            m_reals.emplace(key, 0.0f);
        }
        return m_reals[key];
    }

    Vector3 BlackBoard::GetVector(const std::string& key)
    {
        if (m_vectors.find(key) == m_vectors.end())
        {
            m_vectors.emplace(key, Vector3());
        }
        return m_vectors[key];
    }

    Quaternion BlackBoard::GetQuaternion(const std::string& key)
    {
        if (m_quaternions.find(key) == m_quaternions.end())
        {
            m_quaternions.emplace(key, Quaternion());
        }
        return m_quaternions[key];
    }

    std::string BlackBoard::GetString(const std::string& key)
    {
        if (m_strings.find(key) == m_strings.end())
        {
            m_strings.emplace(key, "");
        }
        return m_strings[key];
    }

    bool& BlackBoard::GetReferencedBoolean(const std::string& key)
    {
        if (m_booleans.find(key) == m_booleans.end())
        {
            m_booleans.emplace(key, false);
        }
        return m_booleans[key];
    }

    int& BlackBoard::GetReferencedInteger(const std::string& key)
    {
        if (m_integers.find(key) == m_integers.end())
        {
            m_integers.emplace(key, 0);
        }
        return m_integers[key];
    }

    size_t& BlackBoard::GetReferencedPositive(const std::string& key)
    {
        if (m_positives.find(key) == m_positives.end())
        {
            m_positives.emplace(key, 0);
        }
        return m_positives[key];
    }

    Real& BlackBoard::GetReferencedReal(const std::string& key)
    {
        if (m_reals.find(key) == m_reals.end())
        {
            m_reals.emplace(key, 0.0f);
        }
        return m_reals[key];
    }

    Vector3& BlackBoard::GetReferencedVector(const std::string& key)
    {
        if (m_vectors.find(key) == m_vectors.end())
        {
            m_vectors.emplace(key, Vector3());
        }
        return m_vectors[key];
    }

    Quaternion& BlackBoard::GetReferencedQuaternion(const std::string& key)
    {
        if (m_quaternions.find(key) == m_quaternions.end())
        {
            m_quaternions.emplace(key, Quaternion());
        }
        return m_quaternions[key];
    }

    std::string& BlackBoard::GetReferencedString(const std::string& key)
    {
        if (m_strings.find(key) == m_strings.end())
        {
            m_strings.emplace(key, "");
        }
        return m_strings[key];
    }

    bool BlackBoard::HasBoolean(const std::string& key)
    {
        return m_booleans.find(key) != m_booleans.end();
    }

    bool BlackBoard::HasInteger(const std::string& key)
    {
        return m_integers.find(key) != m_integers.end();
    }

    bool BlackBoard::HasPositive(const std::string& key)
    {
        return m_positives.find(key) != m_positives.end();
    }

    bool BlackBoard::HasReal(const std::string& key)
    {
        return m_reals.find(key) != m_reals.end();
    }

    bool BlackBoard::HasVector(const std::string& key)
    {
        return m_vectors.find(key) != m_vectors.end();
    }

    bool BlackBoard::HasQuaternion(const std::string& key)
    {
        return m_quaternions.find(key) != m_quaternions.end();
    }

    bool BlackBoard::HasString(const std::string& key)
    {
        return m_strings.find(key) != m_strings.end();
    }

    size_t BlackBoard::SizeOfBoolean() const
    {
        return m_booleans.size();
    }

    size_t BlackBoard::SizeOfInteger() const
    {
        return m_integers.size();
    }

    size_t BlackBoard::SizeOfPositive() const
    {
        return m_positives.size();
    }

    size_t BlackBoard::SizeOfReal() const
    {
        return m_reals.size();
    }

    size_t BlackBoard::SizeOfVector() const
    {
        return m_vectors.size();
    }

    size_t BlackBoard::SizeOfQuaternion() const
    {
        return m_quaternions.size();
    }

    size_t BlackBoard::SizeOfString() const
    {
        return m_strings.size();
    }
}
