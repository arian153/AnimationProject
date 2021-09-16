#include "MaterialManager.hpp"

namespace CS460
{
    MaterialManager::MaterialManager()
    {
    }

    MaterialManager::~MaterialManager()
    {
    }

    void MaterialManager::Initialize()
    {
    }

    void MaterialManager::Shutdown()
    {
        m_data.clear();
    }

    size_t MaterialManager::GetID(const MaterialIdentifier& material)
    {
        size_t id      = m_data.size();
        bool   b_found = false;
        for (size_t i = 0; i < id; ++i)
        {
            if (m_data[i] == material)
            {
                b_found = true;
                id      = i;
                break;
            }
        }

        if (b_found == false)
        {
            m_data.push_back(material);
        }
        return id;
    }

    MaterialIdentifier MaterialManager::GetMaterial(size_t id)
    {
        return m_data[id];
    }
}
