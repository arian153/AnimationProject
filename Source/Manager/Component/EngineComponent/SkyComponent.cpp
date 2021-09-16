#include "SkyComponent.hpp"

#include "../../../External/JSONCPP/json/json.h"
#include "../../../System/Core/Utility/CoreUtility.hpp"
#include "../../../System/Graphics/Element/Scene.hpp"
#include "../../../System/Graphics/Element/CubeMapSky.hpp"
#include "../../../System/Graphics/Element/TextureSky.hpp"
#include "../../Resource/ResourceManager.hpp"
#include "../../Resource/ResourceType/JsonResource.hpp"
#include "../../Resource/ResourceType/TextureResource.hpp"
#include "../../Space/Space.hpp"

namespace CS460
{
    SkyComponent::~SkyComponent()
    {
    }

    void SkyComponent::Initialize()
    {
        if (m_sky_type != -1)
        {
            if (m_sky_type == 0 && m_cube_map_sky == nullptr)
            {
                m_cube_map_sky = new CubeMapSky();
                Subscribe();
                m_cube_map_sky->Initialize();
                m_cube_map_sky->SetTexture(m_sky_texture);
                m_cube_map_sky->m_component = this;
            }

            if (m_sky_type == 1 && m_texture_sky == nullptr)
            {
                m_texture_sky = new TextureSky();
                Subscribe();
                m_texture_sky->Initialize();
                m_texture_sky->m_component = this;
            }
        }
    }

    void SkyComponent::Update(Real dt)
    {
    }

    void SkyComponent::Shutdown()
    {
        Unsubscribe();
        if (m_cube_map_sky != nullptr)
        {
            m_cube_map_sky->Shutdown();
            delete m_cube_map_sky;
            m_cube_map_sky = nullptr;
        }

        if (m_texture_sky != nullptr)
        {
            m_texture_sky->Shutdown();
            delete m_texture_sky;
            m_texture_sky = nullptr;
        }
    }

    bool SkyComponent::Load(const Json::Value& data)
    {
        if (JsonResource::HasMember(data, "Type"))
        {
            if (data["Type"].isString())
            {
                std::string sky_type = data["Type"].asString();

                if (sky_type == "CubeMap")
                {
                    m_sky_type = 0;
                }
                else if (sky_type == "Texture")
                {
                    m_sky_type = 1;
                }
            }

            if (data["Type"].isInt())
            {
                int sky_type = data["Type"].asInt();
                sky_type %= 2;
                m_sky_type = sky_type;
            }
        }

        if (JsonResource::HasMember(data, "Texture"))
        {
            if (data["Texture"].isString())
            {
                std::string texture_path = data["Texture"].asString();
                auto        resource     = m_space->GetResourceManager()->GetTextureResource(ToWString(texture_path));
                m_sky_texture            = resource->GetTexture();
            }
        }

        Initialize();

        return true;
    }

    void SkyComponent::Save(Json::Value& data) const
    {
    }

    void SkyComponent::Edit(CommandRegistry* command_registry)
    {
    }

    void SkyComponent::Subscribe()
    {
        if (m_space != nullptr)
        {
            if (m_cube_map_sky != nullptr)
            {
                m_space->GetScene()->AddCubeMapSky(m_cube_map_sky);
            }

            if (m_texture_sky != nullptr)
            {
                m_space->GetScene()->AddSkyDome(m_texture_sky);
            }
        }
    }

    void SkyComponent::Unsubscribe()
    {
        if (m_space != nullptr)
        {
            if (m_cube_map_sky != nullptr)
            {
                m_space->GetScene()->RemoveCubeMapSky(m_cube_map_sky);
            }

            if (m_texture_sky != nullptr)
            {
                m_space->GetScene()->RemoveSkyDome(m_texture_sky);
            }
        }
    }

    SkyComponent::SkyComponent(Object* owner)
        : Component(owner)
    {
    }

    void SkyComponent::Clone(SkyComponent* origin)
    {
    }

    void SkyComponent::SwitchSkyType()
    {
        m_sky_type = (m_sky_type + 1) % 2;

        if (m_cube_map_sky != nullptr)
        {
            Unsubscribe();
            m_cube_map_sky->Shutdown();
            delete m_cube_map_sky;
            m_cube_map_sky = nullptr;
            m_texture_sky  = new TextureSky();
            m_texture_sky->Initialize();
            //E5_TODO : need to reselect texture.
            Subscribe();
        }
        else if (m_texture_sky != nullptr)
        {
            Unsubscribe();
            m_texture_sky->Shutdown();
            delete m_texture_sky;
            m_texture_sky  = nullptr;
            m_cube_map_sky = new CubeMapSky();
            m_cube_map_sky->Initialize();
            //E5_TODO : need to reselect texture.
            Subscribe();
        }
    }
}
