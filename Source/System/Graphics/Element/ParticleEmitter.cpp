#include "ParticleEmitter.hpp"
#include "../DataType/Particle.hpp"
#include "../../Math/Utility/Random.hpp"
#include "../../Math/Algebra/Matrix44.hpp"
#include "../../../Manager/Component/EngineComponent/ParticleEmitterComponent.hpp"
#include "../../Math/Structure/Transform.hpp"
#include "../../Math/Utility/Utility.hpp"
#include "../../Math/Utility/MatrixUtility.hpp"
#include <algorithm>

#include "../Common/Buffer2/ConstantBufferCommon.hpp"
#include "../Common/Buffer2/IndexBufferCommon.hpp"
#include "../Common/Buffer2/InstanceBufferCommon.hpp"
#include "../Common/Buffer2/VertexBufferCommon.hpp"
#include "../Common/Renderer/RendererCommon.hpp"

namespace CS460
{
    ParticleEmitter::ParticleEmitter()
    {
    }

    ParticleEmitter::~ParticleEmitter()
    {
    }

    void ParticleEmitter::Initialize()
    {
        m_active_amount = 0;
    }

    void ParticleEmitter::Update(Real dt)
    {
        KillParticles();
        SortParticle();
        m_elapsed_time += dt;
        if (m_elapsed_time > m_emission_rate)
        {
            m_elapsed_time = 0.0f;
            EmitParticles();
        }
        m_instances.clear();
        m_instances.reserve(m_active_amount);
        Matrix44 orientation = m_transform != nullptr ? Math::Matrix44::Rotation(m_transform->orientation.Inverse()) : Matrix44();

        Matrix44 world;
        if (m_transform != nullptr)
        {
            world = m_transform->LocalToWorldMatrix();
        }

        for (size_t i = 0; i < m_active_amount; ++i)
        {
            m_particles[i].position += m_particles[i].velocity * dt;
            m_particles[i].life -= m_base_particle.life * m_life_decay_rate * dt;
            m_particles[i].scale -= m_base_particle.scale * m_scale_decay_rate * dt;

            auto               local = ParticleToWorldMatrix(m_particles[i], orientation);
            InstanceBufferData data;
            data.model   = (world * local);
            data.ambient = m_particles[i].color;
            data.diffuse = m_particles[i].color;
            m_instances.push_back(data);
        }
        m_instance_buffer->Update(m_instances);
    }

    void ParticleEmitter::Bind() const
    {
        m_vertex_buffer->Bind(0, m_instance_buffer);
        m_index_buffer->Bind(0);
        m_textures.Bind();
        m_texture_buffer->Bind();
    }

    void ParticleEmitter::Draw() const
    {
        m_index_buffer->Draw((U32)m_active_amount);
    }

    void ParticleEmitter::Shutdown()
    {
        if (m_component != nullptr)
        {
            m_component->m_emitter = nullptr;
        }
        if (m_instance_buffer != nullptr)
        {
            m_instance_buffer->Shutdown();
            delete m_instance_buffer;
            m_instance_buffer = nullptr;
        }

        if (m_vertex_buffer != nullptr)
        {
            m_vertex_buffer->Shutdown();
            delete m_vertex_buffer;
            m_vertex_buffer = nullptr;
        }

        if (m_index_buffer != nullptr)
        {
            m_index_buffer->Shutdown();
            delete m_index_buffer;
            m_index_buffer = nullptr;
        }

        if (m_texture_buffer != nullptr)
        {
            m_texture_buffer->Shutdown();
            delete m_texture_buffer;
            m_texture_buffer = nullptr;
        }

        m_instances.clear();
        m_particles.clear();
        m_textures.Clear();
    }

    void ParticleEmitter::AddParticle(const Particle& particle)
    {
        size_t index       = GetFreeIndex();
        m_particles[index] = particle;
    }

    void ParticleEmitter::AddParticle(const Vector3& pos, const Vector3& vel, const Color& color, Real life, Real scale)
    {
        size_t    index   = GetFreeIndex();
        Particle& created = m_particles[index];
        created.position  = pos;
        created.velocity  = vel;
        created.color     = color;
        created.life      = life;
        created.scale     = scale;
    }

    void ParticleEmitter::SetRenderer(RendererCommon* renderer)
    {
        m_renderer = renderer;
    }

    void ParticleEmitter::SetBuffer()
    {
        if (m_vertex_buffer == nullptr)
        {
            m_vertex_buffer = new VertexBufferCommon();
            //vertices
            std::vector<TextureVertexCommon> vertices;
            vertices.emplace_back(-0.5f, 0.5f, 0.0f, 0.0f, 0.0f);
            vertices.emplace_back(0.5f, -0.5f, 0.0f, 1.0f, 1.0f);
            vertices.emplace_back(-0.5f, -0.5f, 0.0f, 0.0f, 1.0f);
            vertices.emplace_back(0.5f, 0.5f, 0.0f, 1.0f, 0.0f);

            m_vertex_buffer->Init(m_renderer, vertices, false);

            m_stride = sizeof(TextureVertexCommon);
        }

        if (m_index_buffer == nullptr)
        {
            m_index_buffer = new IndexBufferCommon();

            //indices
            std::vector<U32> indices = {
                0, 1, 2,
                0, 3, 1,
                0, 2, 1,
                0, 1, 3
            };

            m_index_buffer->Init(m_renderer, indices);
        }

        if (m_instance_buffer == nullptr)
        {
            m_instance_buffer = new InstanceBufferCommon();
        }
        if (m_texture_buffer == nullptr)
        {
            m_texture_buffer = new ConstantBufferCommon();
            m_texture_buffer->Init(m_renderer, eBindingStage::PixelShader, sizeof(TextureBufferData), 0);
            TextureBufferData data;
            data.diff_type = 5;
            data.spec_type = 0;
            data.norm_type = 0;
            //E5_TODO : need to update user gamma setting
            data.gamma = 2.2f;

            m_texture_buffer->Update(data);
        }
    }

    void ParticleEmitter::SetTexture(TextureCommon* texture, TextureCommon* default_texture)
    {
        m_texture = texture;
        m_textures.Clear();

        if (texture != nullptr)
        {
            m_textures.PushBack(texture); //diffuse 0
        }
        else
        {
            m_textures.PushBack(default_texture); //diffuse 0
        }

        m_textures.PushBack(default_texture); //diffuse 1
        m_textures.PushBack(default_texture); //diffuse 2
        m_textures.PushBack(default_texture); //specular 0
        m_textures.PushBack(default_texture); //normal map 0
    }

    void ParticleEmitter::SetTransform(Transform* transform)
    {
        m_transform = transform;
    }

    void ParticleEmitter::SetBillboardPosition(const Vector3& position)
    {
        m_billboard_position = position;
    }

    void ParticleEmitter::SetParticleAmount(const size_t& amount)
    {
        m_max_amount = amount;
        m_particles.clear();
        m_particles.resize(m_max_amount);
        m_instances.resize(m_max_amount);
        m_instance_buffer->Init(m_renderer, m_instances);
        m_active_amount = 0;
        m_free_particle = 0;
    }

    void ParticleEmitter::SetEmissionAmount(const size_t& amount)
    {
        m_emission_amount = amount;
    }

    void ParticleEmitter::SetEmissionRate(const Real& rate)
    {
        m_emission_rate = rate;
    }

    void ParticleEmitter::SetLifeDecayRate(const Real& rate)
    {
        if (rate >= 0.0f)
        {
            m_life_decay_rate = rate;
        }
    }

    void ParticleEmitter::SetScaleDecayRate(const Real& rate)
    {
        m_scale_decay_rate = rate;
    }

    void ParticleEmitter::SetBaseParticle(const Particle& particle)
    {
        m_base_particle = particle;
    }

    void ParticleEmitter::SetBasePosition(const Vector3& position)
    {
        m_base_particle.position = position;
    }

    void ParticleEmitter::SetBaseVelocity(const Vector3& velocity)
    {
        m_base_particle.velocity = velocity;
    }

    void ParticleEmitter::SetBaseColor(const Color& color)
    {
        m_base_particle.color = color;
    }

    void ParticleEmitter::SetBaseLife(const Real& life_time)
    {
        m_base_particle.life = life_time;
    }

    void ParticleEmitter::SetBaseScale(const Real& scale)
    {
        m_base_particle.scale = scale;
    }

    void ParticleEmitter::SetPositionVariance(const Vector3& variance)
    {
        m_position_variance = variance;
    }

    void ParticleEmitter::SetVelocityVariance(const Vector3& variance)
    {
        m_velocity_variance = variance;
    }

    void ParticleEmitter::SetColorVariance(const Color& variance)
    {
        m_color_variance = variance;
    }

    void ParticleEmitter::SetScaleVariance(const Real& variance)
    {
        m_scale_variance = variance;
    }

    void ParticleEmitter::SetLifeVariance(const Real& variance)
    {
        m_life_variance = variance;
    }

    TextureCommon* ParticleEmitter::GetTexture() const
    {
        return m_texture;
    }

    Matrix44 ParticleEmitter::GetModelMatrix() const
    {
        if (m_transform != nullptr)
        {
            return m_transform->LocalToWorldMatrix();
        }
        return Matrix44();
    }

    size_t ParticleEmitter::GetFreeIndex()
    {
        for (size_t i = m_free_particle; i < m_max_amount; ++i)
        {
            if (m_particles[i].b_active == false)
            {
                m_free_particle = i;
                return m_free_particle;
            }
        }
        for (size_t i = 0; i < m_free_particle; ++i)
        {
            if (m_particles[i].b_active == false)
            {
                m_free_particle = i;
                return m_free_particle;
            }
        }
        m_free_particle++;
        if (m_free_particle >= m_max_amount)
        {
            m_free_particle = 0;
        }
        return m_free_particle;
    }

    void ParticleEmitter::EmitParticles()
    {
        Random random;
        for (size_t i = 0; i < m_emission_amount; ++i)
        {
            Particle particle;
            //position
            particle.position.x = random.GetRangedRandomReal(-m_position_variance.x, m_position_variance.x);
            particle.position.y = random.GetRangedRandomReal(-m_position_variance.y, m_position_variance.y);
            particle.position.z = random.GetRangedRandomReal(-m_position_variance.z, m_position_variance.z);
            //velocity
            particle.velocity.x = random.GetRangedRandomReal(-m_velocity_variance.x, m_velocity_variance.x);
            particle.velocity.y = random.GetRangedRandomReal(-m_velocity_variance.y, m_velocity_variance.y);
            particle.velocity.z = random.GetRangedRandomReal(-m_velocity_variance.z, m_velocity_variance.z);
            //color
            particle.color.r = random.GetRangedRandomReal(0.0f, m_color_variance.r);
            particle.color.g = random.GetRangedRandomReal(0.0f, m_color_variance.g);
            particle.color.b = random.GetRangedRandomReal(0.0f, m_color_variance.b);
            particle.color.a = random.GetRangedRandomReal(0.0f, m_color_variance.a);
            //life
            particle.life = random.GetRangedRandomReal(0.0f, m_life_variance);
            //scale
            particle.scale = random.GetRangedRandomReal(0.0f, m_scale_variance);
            //apply default data
            particle.Merge(m_base_particle);
            particle.b_active = true;
            AddParticle(particle);
            m_active_amount++;
            if (m_active_amount >= m_max_amount)
            {
                m_active_amount = m_max_amount;
            }
        }
    }

    void ParticleEmitter::KillParticles()
    {
        Matrix44 transform = m_transform != nullptr ? m_transform->LocalToWorldMatrix() : Matrix44();
        for (size_t i = 0; i < m_active_amount;)
        {
            if (m_particles[i].b_active == true && m_particles[i].IsAlive() == false)
            {
                m_particles[i].b_active = false;
                m_active_amount--;
                m_free_particle = m_active_amount;
                size_t count    = m_max_amount - 1 - i;
                memcpy(&m_particles[i], &m_particles[i + 1], sizeof(Particle) * count);
            }
            else
            {
                ++i;
            }
            if (i >= m_max_amount)
            {
                break;
            }
            Vector3 position      = (transform * Vector4(m_particles[i].position, 1.0f)).GrepVec3(0, 1, 2);
            m_particles[i].factor = position.DistanceTo(m_billboard_position);
        }
    }

    void ParticleEmitter::SortParticle()
    {
        std::sort(
                  m_particles.begin(), m_particles.begin() + m_active_amount, [](const Particle& lhs, const Particle& rhs)
                  {
                      return lhs.factor > rhs.factor;
                  });
    }

    Matrix44 ParticleEmitter::ParticleToWorldMatrix(const Particle& particle, const Matrix44& orientation) const
    {
        Matrix44 result;
        result.SetDiagonal(particle.scale, particle.scale, particle.scale, 1.0f);
        Real angle = atan2f(particle.position.x - m_billboard_position.x, particle.position.z - m_billboard_position.z) * (180.0f / Math::PI);
        //Quaternion quat(Math::Vector3::Z_AXIS, (particle.position - m_billboard_position).Unit());
        Real rotation = Math::DegreesToRadians(angle);
        result        = Math::Matrix44::RotationY(rotation) * result;
        //result = Math::Matrix44::Rotation(quat) * result;
        result = orientation * result;
        result.AddVectorColumn(3, particle.position);
        return result;
    }
}
