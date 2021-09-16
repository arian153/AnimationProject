#pragma warning( disable : 26451)

#include "MeshGenerator.hpp"
#include <algorithm>
#include "../../Core/Utility/CoreUtility.hpp"
#include "../../Math/Algebra/Vector3.hpp"
#include "../DataType/MeshData.hpp"
#include "../../Math/Algebra/Vector2.hpp"
#include "../../Math/Utility/VectorDef.hpp"

namespace CS460
{
    MeshGenerator::MeshGenerator()
    {
    }

    MeshGenerator::~MeshGenerator()
    {
    }

    MeshData* MeshGenerator::CreateBox(Real width, Real height, Real depth, U32 num_subdivisions) const
    {
        MeshData*    mesh_data = new MeshData();
        VertexCommon v[24];
        Real         w2 = 0.5f * width;
        Real         h2 = 0.5f * height;
        Real         d2 = 0.5f * depth;
        // Fill in the front face vertex data.
        v[0] = VertexCommon(-w2, -h2, -d2, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f);
        v[1] = VertexCommon(-w2, +h2, -d2, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f);
        v[2] = VertexCommon(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f);
        v[3] = VertexCommon(+w2, -h2, -d2, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f);
        // Fill in the back face vertex data.
        v[4] = VertexCommon(-w2, -h2, +d2, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f);
        v[5] = VertexCommon(+w2, -h2, +d2, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f);
        v[6] = VertexCommon(+w2, +h2, +d2, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f);
        v[7] = VertexCommon(-w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f);
        // Fill in the top face vertex data.
        v[8]  = VertexCommon(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f);
        v[9]  = VertexCommon(-w2, +h2, +d2, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f);
        v[10] = VertexCommon(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f);
        v[11] = VertexCommon(+w2, +h2, -d2, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f);
        // Fill in the bottom face vertex data.
        v[12] = VertexCommon(-w2, -h2, -d2, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f);
        v[13] = VertexCommon(+w2, -h2, -d2, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f);
        v[14] = VertexCommon(+w2, -h2, +d2, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f);
        v[15] = VertexCommon(-w2, -h2, +d2, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f);
        // Fill in the left face vertex data.
        v[16] = VertexCommon(-w2, -h2, +d2, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f);
        v[17] = VertexCommon(-w2, +h2, +d2, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f);
        v[18] = VertexCommon(-w2, +h2, -d2, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f);
        v[19] = VertexCommon(-w2, -h2, -d2, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f);
        // Fill in the right face vertex data.
        v[20] = VertexCommon(+w2, -h2, -d2, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
        v[21] = VertexCommon(+w2, +h2, -d2, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
        v[22] = VertexCommon(+w2, +h2, +d2, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
        v[23] = VertexCommon(+w2, -h2, +d2, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
        mesh_data->vertices.assign(&v[0], &v[24]);
        // Create the indices.
        U32 i[36];
        // Fill in the front face index data
        i[0] = 0;
        i[1] = 1;
        i[2] = 2;
        i[3] = 0;
        i[4] = 2;
        i[5] = 3;
        // Fill in the back face index data
        i[6]  = 4;
        i[7]  = 5;
        i[8]  = 6;
        i[9]  = 4;
        i[10] = 6;
        i[11] = 7;
        // Fill in the top face index data
        i[12] = 8;
        i[13] = 9;
        i[14] = 10;
        i[15] = 8;
        i[16] = 10;
        i[17] = 11;
        // Fill in the bottom face index data
        i[18] = 12;
        i[19] = 13;
        i[20] = 14;
        i[21] = 12;
        i[22] = 14;
        i[23] = 15;
        // Fill in the left face index data
        i[24] = 16;
        i[25] = 17;
        i[26] = 18;
        i[27] = 16;
        i[28] = 18;
        i[29] = 19;
        // Fill in the right face index data
        i[30] = 20;
        i[31] = 21;
        i[32] = 22;
        i[33] = 20;
        i[34] = 22;
        i[35] = 23;
        mesh_data->indices.assign(&i[0], &i[36]);
        U32 subdivisions = std::min<U32>(num_subdivisions, 6u);
        for (U32 j = 0; j < subdivisions; ++j)
            Subdivide(*mesh_data);
        CalculateBinormal(*mesh_data);
        return mesh_data;
    }

    MeshData* MeshGenerator::CreateSphere(Real radius, U32 slice_count, U32 stack_count) const
    {
        MeshData*    mesh_data = new MeshData();
        VertexCommon top_vertex(0.0f, +radius, 0.0f, 0.0f, 0.0f, 0.0f, +1.0f, 0.0f, 1.0f, 0.0f, 0.0f);
        VertexCommon bottom_vertex(0.0f, -radius, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f);
        mesh_data->vertices.push_back(top_vertex);
        Real phi_step   = Math::PI / stack_count;
        Real theta_step = Math::TWO_PI / slice_count;
        // Compute vertices for each stack ring (do not count the poles as rings).
        for (U32 i = 1; i <= stack_count - 1; ++i)
        {
            Real phi = i * phi_step;
            // Vertices of ring.
            for (U32 j = 0; j <= slice_count; ++j)
            {
                Real    theta = j * theta_step;
                Vector3 position(radius * sinf(phi) * cosf(theta), radius * cosf(phi), radius * sinf(phi) * sinf(theta));
                Vector2 uv(theta / Math::TWO_PI, phi / Math::PI);
                Vector3 normal = position.Normalize();
                Vector3 tangent(-radius * sinf(phi) * sinf(theta), 0.0f, radius * sinf(phi) * cosf(theta));
                tangent.SetNormalize();
                mesh_data->vertices.emplace_back(position, uv, normal, tangent);
            }
        }
        mesh_data->vertices.push_back(bottom_vertex);
        // Compute indices for top stack.  The top stack was written first to the vertex buffer
        // and connects the top pole to the first ring.
        for (U32 i = 1; i <= slice_count; ++i)
        {
            mesh_data->indices.push_back(0);
            mesh_data->indices.push_back(i + 1);
            mesh_data->indices.push_back(i);
        }
        // Compute indices for inner stacks (not connected to poles).
        // Offset the indices to the index of the first vertex in the first ring.
        // This is just skipping the top pole vertex.
        U32 base_index        = 1;
        U32 ring_vertex_count = slice_count + 1;
        for (U32 i = 0; i < stack_count - 2; ++i)
        {
            for (U32 j = 0; j < slice_count; ++j)
            {
                mesh_data->indices.push_back(base_index + i * ring_vertex_count + j);
                mesh_data->indices.push_back(base_index + i * ring_vertex_count + j + 1);
                mesh_data->indices.push_back(base_index + (i + 1) * ring_vertex_count + j);
                mesh_data->indices.push_back(base_index + (i + 1) * ring_vertex_count + j);
                mesh_data->indices.push_back(base_index + i * ring_vertex_count + j + 1);
                mesh_data->indices.push_back(base_index + (i + 1) * ring_vertex_count + j + 1);
            }
        }
        // Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
        // and connects the bottom pole to the bottom ring.
        // South pole vertex was added last.
        U32 south_pole_index = (U32)mesh_data->vertices.size() - 1;
        // Offset the indices to the index of the first vertex in the last ring.
        base_index = south_pole_index - ring_vertex_count;
        for (U32 i = 0; i < slice_count; ++i)
        {
            mesh_data->indices.push_back(south_pole_index);
            mesh_data->indices.push_back(base_index + i);
            mesh_data->indices.push_back(base_index + i + 1);
        }
        CalculateBinormal(*mesh_data);
        return mesh_data;
    }

    MeshData* MeshGenerator::CreateGeodesicSphere(Real radius, U32 subdivisions) const
    {
        MeshData* mesh_data = new MeshData();
        // Put a cap on the number of subdivisions.
        U32 _subdivisions = std::min<U32>(subdivisions, 5);
        // Approximate a sphere by tessellating an icosahedron.
        const Real x            = 0.525731f;
        const Real z            = 0.850651f;
        Vector3    position[12] =
        {
            Vector3(-x, 0.0f, z), Vector3(x, 0.0f, z), Vector3(-x, 0.0f, -z), Vector3(x, 0.0f, -z), Vector3(0.0f, z, x), Vector3(0.0f, z, -x), Vector3(0.0f, -z, x), Vector3(0.0f, -z, -x), Vector3(z, x, 0.0f), Vector3(-z, x, 0.0f), Vector3(z, -x, 0.0f), Vector3(-z, -x, 0.0f)
        };
        U32 k[60] =
        {
            1, 4, 0, 4, 9, 0, 4, 5, 9, 8, 5, 4, 1, 8, 4, 1, 10, 8, 10, 3, 8, 8, 3, 5, 3, 2, 5, 3, 7, 2, 3, 10, 7, 10, 6, 7, 6, 11, 7, 6, 0, 11, 6, 1, 0, 10, 1, 6, 11, 0, 9, 2, 11, 9, 5, 2, 9, 11, 2, 7
        };
        mesh_data->vertices.resize(12);
        mesh_data->indices.assign(&k[0], &k[60]);
        for (U32 i = 0; i < 12; ++i)
        {
            mesh_data->vertices[i].SetPosition(position[i]);
        }
        for (U32 i = 0; i < _subdivisions; ++i)
        {
            Subdivide(*mesh_data);
        }
        // Project vertices onto sphere and scale.
        for (auto& vertex : mesh_data->vertices)
        {
            Vector3 vertex_pos = vertex.GetPosition();
            Vector3 n          = vertex_pos.Normalize();
            Vector3 p          = n.Scale(radius);
            Real    theta      = atan2f(p.z, p.x);
            theta              = theta < 0.0f ? theta + Math::TWO_PI : theta;
            Real    phi        = acosf(p.y / radius);
            Vector2 uv(theta / Math::TWO_PI, phi / Math::PI);
            Vector3 t = Vector3(-radius * sinf(phi) * sinf(theta), 0.0f, radius * sinf(phi) * cosf(theta)).Normalize();
            Vector3 b = CrossProduct(t, n).Normalize();
            vertex.SetPosition(p);
            vertex.SetUV(uv);
            vertex.SetNormal(n);
            vertex.SetTangent(t);
            vertex.SetBinormal(b);
        }
        return mesh_data;
    }

    MeshData* MeshGenerator::CreateCylinder(Real bottom_radius, Real top_radius, Real height, U32 slice_count, U32 stack_count) const
    {
        MeshData* mesh_data    = new MeshData();
        Real      stack_height = height / stack_count;
        // Amount to increment radius as we move up each stack level from bottom to top.
        Real radius_step = (top_radius - bottom_radius) / stack_count;
        U32  ring_count  = stack_count + 1;
        // Compute vertices for each stack ring starting at the bottom and moving up.
        for (U32 i = 0; i < ring_count; ++i)
        {
            Real y = -0.5f * height + i * stack_height;
            Real r = bottom_radius + i * radius_step;
            // vertices of ring
            Real d_theta = Math::TWO_PI / slice_count;
            for (U32 j = 0; j <= slice_count; ++j)
            {
                Real    c  = cosf(j * d_theta);
                Real    s  = sinf(j * d_theta);
                Real    dr = bottom_radius - top_radius;
                Vector3 p(r * c, y, r * s);
                Vector2 uv((Real)j / slice_count, 1.0f - (Real)i / stack_count);
                Vector3 t         = Vector3(-s, 0.0f, c).Normalize();
                Vector3 bitangent = Vector3(dr * c, -height, dr * s).Normalize();
                Vector3 n         = CrossProduct(t, bitangent).Normalize();
                Vector3 b         = CrossProduct(t, n).Normalize();
                mesh_data->vertices.emplace_back(p, uv, n, t, b);
            }
        }
        // Add one because we duplicate the first and last vertex per ring
        // since the texture coordinates are different.
        U32 ring_vertex_count = slice_count + 1;
        // Compute indices for each stack.
        for (U32 i = 0; i < stack_count; ++i)
        {
            for (U32 j = 0; j < slice_count; ++j)
            {
                mesh_data->indices.push_back(i * ring_vertex_count + j);
                mesh_data->indices.push_back((i + 1) * ring_vertex_count + j);
                mesh_data->indices.push_back((i + 1) * ring_vertex_count + j + 1);
                mesh_data->indices.push_back(i * ring_vertex_count + j);
                mesh_data->indices.push_back((i + 1) * ring_vertex_count + j + 1);
                mesh_data->indices.push_back(i * ring_vertex_count + j + 1);
            }
        }
        BuildCylinderTopCap(top_radius, height, slice_count, *mesh_data);
        BuildCylinderBottomCap(bottom_radius, height, slice_count, *mesh_data);
        return mesh_data;
    }

    MeshData* MeshGenerator::CreateGrid(Real width, Real depth, U32 m, U32 n) const
    {
        MeshData* mesh_data    = new MeshData();
        U32       vertex_count = m * n;
        U32       face_count   = (m - 1) * (n - 1) * 2;
        Real      half_width   = 0.5f * width;
        Real      half_depth   = 0.5f * depth;
        Real      dx           = width / (n - 1);
        Real      dz           = depth / (m - 1);
        Real      du           = 1.0f / (n - 1);
        Real      dv           = 1.0f / (m - 1);
        mesh_data->vertices.resize(vertex_count);
        for (U32 i = 0; i < m; ++i)
        {
            Real z = half_depth - i * dz;
            for (U32 j = 0; j < n; ++j)
            {
                Real    x        = -half_width + j * dx;
                Vector3 pos      = Vector3(x, 0.0f, z);
                Vector2 uv       = Vector2(j * du, i * dv);
                Vector3 normal   = Math::Vector3::Y_AXIS;
                Vector3 tangent  = Math::Vector3::X_AXIS;
                Vector3 binormal = CrossProduct(tangent, normal).Normalize();
                // Stretch texture over grid.
                mesh_data->vertices[i * n + j].SetPosition(pos);
                mesh_data->vertices[i * n + j].SetUV(uv);
                mesh_data->vertices[i * n + j].SetNormal(normal);
                mesh_data->vertices[i * n + j].SetTangent(tangent);
                mesh_data->vertices[i * n + j].SetBinormal(binormal);
            }
        }
        mesh_data->indices.resize(face_count * 3); // 3 indices per face
        U32 k = 0;
        for (U32 i = 0; i < m - 1; ++i)
        {
            for (U32 j = 0; j < n - 1; ++j)
            {
                mesh_data->indices[k]     = i * n + j;
                mesh_data->indices[k + 1] = i * n + j + 1;
                mesh_data->indices[k + 2] = (i + 1) * n + j;
                mesh_data->indices[k + 3] = (i + 1) * n + j;
                mesh_data->indices[k + 4] = i * n + j + 1;
                mesh_data->indices[k + 5] = (i + 1) * n + j + 1;
                k += 6; // next quad
            }
        }
        return mesh_data;
    }

    MeshData* MeshGenerator::CreateQuad(Real x, Real y, Real w, Real h, Real depth) const
    {
        MeshData* mesh_data = new MeshData();
        mesh_data->vertices.resize(4);
        mesh_data->indices.resize(6);
        // Position coordinates specified in NDC space.
        mesh_data->vertices[0] = VertexCommon(
                                              x, y - h, depth,
                                              0.0f, 1.0f,
                                              0.0f, 0.0f, -1.0f,
                                              1.0f, 0.0f, 0.0f
                                             );
        mesh_data->vertices[1] = VertexCommon(
                                              x, y, depth,
                                              0.0f, 0.0f,
                                              0.0f, 0.0f, -1.0f,
                                              1.0f, 0.0f, 0.0f
                                             );
        mesh_data->vertices[2] = VertexCommon(
                                              x + w, y, depth,
                                              1.0f, 0.0f,
                                              0.0f, 0.0f, -1.0f,
                                              1.0f, 0.0f, 0.0f
                                             );
        mesh_data->vertices[3] = VertexCommon(
                                              x + w, y - h, depth,
                                              1.0f, 1.0f,
                                              0.0f, 0.0f, -1.0f,
                                              1.0f, 0.0f, 0.0f
                                             );
        mesh_data->indices[0] = 0;
        mesh_data->indices[1] = 1;
        mesh_data->indices[2] = 2;
        mesh_data->indices[3] = 0;
        mesh_data->indices[4] = 2;
        mesh_data->indices[5] = 3;
        CalculateBinormal(*mesh_data);
        return mesh_data;
    }

    void MeshGenerator::CreateBox(Real width, Real height, Real depth, U32 num_subdivisions, MeshData& mesh_data) const
    {
        VertexCommon v[24];
        Real         w2 = 0.5f * width;
        Real         h2 = 0.5f * height;
        Real         d2 = 0.5f * depth;
        // Fill in the front face vertex data.
        v[0] = VertexCommon(-w2, -h2, -d2, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f);
        v[1] = VertexCommon(-w2, +h2, -d2, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f);
        v[2] = VertexCommon(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f);
        v[3] = VertexCommon(+w2, -h2, -d2, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f);
        // Fill in the back face vertex data.
        v[4] = VertexCommon(-w2, -h2, +d2, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f);
        v[5] = VertexCommon(+w2, -h2, +d2, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f);
        v[6] = VertexCommon(+w2, +h2, +d2, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f);
        v[7] = VertexCommon(-w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f);
        // Fill in the top face vertex data.
        v[8]  = VertexCommon(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f);
        v[9]  = VertexCommon(-w2, +h2, +d2, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f);
        v[10] = VertexCommon(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f);
        v[11] = VertexCommon(+w2, +h2, -d2, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f);
        // Fill in the bottom face vertex data.
        v[12] = VertexCommon(-w2, -h2, -d2, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f);
        v[13] = VertexCommon(+w2, -h2, -d2, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f);
        v[14] = VertexCommon(+w2, -h2, +d2, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f);
        v[15] = VertexCommon(-w2, -h2, +d2, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f);
        // Fill in the left face vertex data.
        v[16] = VertexCommon(-w2, -h2, +d2, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f);
        v[17] = VertexCommon(-w2, +h2, +d2, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f);
        v[18] = VertexCommon(-w2, +h2, -d2, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f);
        v[19] = VertexCommon(-w2, -h2, -d2, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f);
        // Fill in the right face vertex data.
        v[20] = VertexCommon(+w2, -h2, -d2, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
        v[21] = VertexCommon(+w2, +h2, -d2, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
        v[22] = VertexCommon(+w2, +h2, +d2, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
        v[23] = VertexCommon(+w2, -h2, +d2, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
        mesh_data.vertices.assign(&v[0], &v[24]);
        // Create the indices.
        U32 i[36];
        // Fill in the front face index data
        i[0] = 0;
        i[1] = 1;
        i[2] = 2;
        i[3] = 0;
        i[4] = 2;
        i[5] = 3;
        // Fill in the back face index data
        i[6]  = 4;
        i[7]  = 5;
        i[8]  = 6;
        i[9]  = 4;
        i[10] = 6;
        i[11] = 7;
        // Fill in the top face index data
        i[12] = 8;
        i[13] = 9;
        i[14] = 10;
        i[15] = 8;
        i[16] = 10;
        i[17] = 11;
        // Fill in the bottom face index data
        i[18] = 12;
        i[19] = 13;
        i[20] = 14;
        i[21] = 12;
        i[22] = 14;
        i[23] = 15;
        // Fill in the left face index data
        i[24] = 16;
        i[25] = 17;
        i[26] = 18;
        i[27] = 16;
        i[28] = 18;
        i[29] = 19;
        // Fill in the right face index data
        i[30] = 20;
        i[31] = 21;
        i[32] = 22;
        i[33] = 20;
        i[34] = 22;
        i[35] = 23;
        mesh_data.indices.assign(&i[0], &i[36]);
        U32 subdivisions = std::min<U32>(num_subdivisions, 6u);
        for (U32 j = 0; j < subdivisions; ++j)
            Subdivide(mesh_data);
        CalculateBinormal(mesh_data);
    }

    void MeshGenerator::CreateSphere(Real radius, U32 slice_count, U32 stack_count, MeshData& mesh_data) const
    {
        VertexCommon top_vertex(0.0f, +radius, 0.0f, 0.0f, 0.0f, 0.0f, +1.0f, 0.0f, 1.0f, 0.0f, 0.0f);
        VertexCommon bottom_vertex(0.0f, -radius, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f);
        mesh_data.vertices.push_back(top_vertex);
        Real phi_step   = Math::PI / stack_count;
        Real theta_step = Math::TWO_PI / slice_count;
        // Compute vertices for each stack ring (do not count the poles as rings).
        for (U32 i = 1; i <= stack_count - 1; ++i)
        {
            Real phi = i * phi_step;
            // Vertices of ring.
            for (U32 j = 0; j <= slice_count; ++j)
            {
                Real    theta = j * theta_step;
                Vector3 position(radius * sinf(phi) * cosf(theta), radius * cosf(phi), radius * sinf(phi) * sinf(theta));
                Vector2 uv(theta / Math::TWO_PI, phi / Math::PI);
                Vector3 normal = position.Normalize();
                Vector3 tangent(-radius * sinf(phi) * sinf(theta), 0.0f, radius * sinf(phi) * cosf(theta));
                tangent.SetNormalize();
                mesh_data.vertices.emplace_back(position, uv, normal, tangent);
            }
        }
        mesh_data.vertices.push_back(bottom_vertex);
        // Compute indices for top stack.  The top stack was written first to the vertex buffer
        // and connects the top pole to the first ring.
        for (U32 i = 1; i <= slice_count; ++i)
        {
            mesh_data.indices.push_back(0);
            mesh_data.indices.push_back(i + 1);
            mesh_data.indices.push_back(i);
        }
        // Compute indices for inner stacks (not connected to poles).
        // Offset the indices to the index of the first vertex in the first ring.
        // This is just skipping the top pole vertex.
        U32 base_index        = 1;
        U32 ring_vertex_count = slice_count + 1;
        for (U32 i = 0; i < stack_count - 2; ++i)
        {
            for (U32 j = 0; j < slice_count; ++j)
            {
                mesh_data.indices.push_back(base_index + i * ring_vertex_count + j);
                mesh_data.indices.push_back(base_index + i * ring_vertex_count + j + 1);
                mesh_data.indices.push_back(base_index + (i + 1) * ring_vertex_count + j);
                mesh_data.indices.push_back(base_index + (i + 1) * ring_vertex_count + j);
                mesh_data.indices.push_back(base_index + i * ring_vertex_count + j + 1);
                mesh_data.indices.push_back(base_index + (i + 1) * ring_vertex_count + j + 1);
            }
        }
        // Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
        // and connects the bottom pole to the bottom ring.
        // South pole vertex was added last.
        U32 south_pole_index = (U32)mesh_data.vertices.size() - 1;
        // Offset the indices to the index of the first vertex in the last ring.
        base_index = south_pole_index - ring_vertex_count;
        for (U32 i = 0; i < slice_count; ++i)
        {
            mesh_data.indices.push_back(south_pole_index);
            mesh_data.indices.push_back(base_index + i);
            mesh_data.indices.push_back(base_index + i + 1);
        }
        CalculateBinormal(mesh_data);
    }

    void MeshGenerator::CreateGeodesicSphere(Real radius, U32 subdivisions, MeshData& mesh_data) const
    {
        // Put a cap on the number of subdivisions.
        U32 _subdivisions = std::min<U32>(subdivisions, 5);
        // Approximate a sphere by tessellating an icosahedron.
        const Real x            = 0.525731f;
        const Real z            = 0.850651f;
        Vector3    position[12] =
        {
            Vector3(-x, 0.0f, z), Vector3(x, 0.0f, z), Vector3(-x, 0.0f, -z), Vector3(x, 0.0f, -z), Vector3(0.0f, z, x), Vector3(0.0f, z, -x), Vector3(0.0f, -z, x), Vector3(0.0f, -z, -x), Vector3(z, x, 0.0f), Vector3(-z, x, 0.0f), Vector3(z, -x, 0.0f), Vector3(-z, -x, 0.0f)
        };
        U32 k[60] =
        {
            1, 4, 0, 4, 9, 0, 4, 5, 9, 8, 5, 4, 1, 8, 4, 1, 10, 8, 10, 3, 8, 8, 3, 5, 3, 2, 5, 3, 7, 2, 3, 10, 7, 10, 6, 7, 6, 11, 7, 6, 0, 11, 6, 1, 0, 10, 1, 6, 11, 0, 9, 2, 11, 9, 5, 2, 9, 11, 2, 7
        };
        mesh_data.vertices.resize(12);
        mesh_data.indices.assign(&k[0], &k[60]);
        for (U32 i = 0; i < 12; ++i)
        {
            mesh_data.vertices[i].SetPosition(position[i]);
        }
        for (U32 i = 0; i < _subdivisions; ++i)
        {
            Subdivide(mesh_data);
        }
        // Project vertices onto sphere and scale.
        for (auto& vertex : mesh_data.vertices)
        {
            Vector3 vertex_pos = vertex.GetPosition();
            Vector3 n          = vertex_pos.Normalize();
            Vector3 p          = n.Scale(radius);
            Real    theta      = atan2f(p.z, p.x);
            theta              = theta < 0.0f ? theta + Math::TWO_PI : theta;
            Real    phi        = acosf(p.y / radius);
            Vector2 uv(theta / Math::TWO_PI, phi / Math::PI);
            Vector3 t = Vector3(-radius * sinf(phi) * sinf(theta), 0.0f, radius * sinf(phi) * cosf(theta)).Normalize();
            Vector3 b = CrossProduct(t, n).Normalize();
            vertex.SetPosition(p);
            vertex.SetUV(uv);
            vertex.SetNormal(n);
            vertex.SetTangent(t);
            vertex.SetBinormal(b);
        }
    }

    void MeshGenerator::CreateCylinder(Real bottom_radius, Real top_radius, Real height, U32 slice_count, U32 stack_count, MeshData& mesh_data) const
    {
        Real stack_height = height / stack_count;
        // Amount to increment radius as we move up each stack level from bottom to top.
        Real radius_step = (top_radius - bottom_radius) / stack_count;
        U32  ring_count  = stack_count + 1;
        // Compute vertices for each stack ring starting at the bottom and moving up.
        for (U32 i = 0; i < ring_count; ++i)
        {
            Real y = -0.5f * height + i * stack_height;
            Real r = bottom_radius + i * radius_step;
            // vertices of ring
            Real d_theta = Math::TWO_PI / slice_count;
            for (U32 j = 0; j <= slice_count; ++j)
            {
                Real    c  = cosf(j * d_theta);
                Real    s  = sinf(j * d_theta);
                Real    dr = bottom_radius - top_radius;
                Vector3 p(r * c, y, r * s);
                Vector2 uv((Real)j / slice_count, 1.0f - (Real)i / stack_count);
                Vector3 t         = Vector3(-s, 0.0f, c).Normalize();
                Vector3 bitangent = Vector3(dr * c, -height, dr * s).Normalize();
                Vector3 n         = CrossProduct(t, bitangent).Normalize();
                Vector3 b         = CrossProduct(t, n).Normalize();
                mesh_data.vertices.emplace_back(p, uv, n, t, b);
            }
        }
        // Add one because we duplicate the first and last vertex per ring
        // since the texture coordinates are different.
        U32 ring_vertex_count = slice_count + 1;
        // Compute indices for each stack.
        for (U32 i = 0; i < stack_count; ++i)
        {
            for (U32 j = 0; j < slice_count; ++j)
            {
                mesh_data.indices.push_back(i * ring_vertex_count + j);
                mesh_data.indices.push_back((i + 1) * ring_vertex_count + j);
                mesh_data.indices.push_back((i + 1) * ring_vertex_count + j + 1);
                mesh_data.indices.push_back(i * ring_vertex_count + j);
                mesh_data.indices.push_back((i + 1) * ring_vertex_count + j + 1);
                mesh_data.indices.push_back(i * ring_vertex_count + j + 1);
            }
        }
        BuildCylinderTopCap(top_radius, height, slice_count, mesh_data);
        BuildCylinderBottomCap(bottom_radius, height, slice_count, mesh_data);
    }

    void MeshGenerator::CreateGrid(Real width, Real depth, U32 m, U32 n, MeshData& mesh_data) const
    {
        U32  vertex_count = m * n;
        U32  face_count   = (m - 1) * (n - 1) * 2;
        Real half_width   = 0.5f * width;
        Real half_depth   = 0.5f * depth;
        Real dx           = width / (n - 1);
        Real dz           = depth / (m - 1);
        Real du           = 1.0f / (n - 1);
        Real dv           = 1.0f / (m - 1);
        mesh_data.vertices.resize(vertex_count);
        for (U32 i = 0; i < m; ++i)
        {
            Real z = half_depth - i * dz;
            for (U32 j = 0; j < n; ++j)
            {
                Real    x        = -half_width + j * dx;
                Vector3 pos      = Vector3(x, 0.0f, z);
                Vector2 uv       = Vector2(j * du, i * dv);
                Vector3 normal   = Math::Vector3::Y_AXIS;
                Vector3 tangent  = Math::Vector3::X_AXIS;
                Vector3 binormal = CrossProduct(tangent, normal).Normalize();
                // Stretch texture over grid.
                mesh_data.vertices[i * n + j].SetPosition(pos);
                mesh_data.vertices[i * n + j].SetUV(uv);
                mesh_data.vertices[i * n + j].SetNormal(normal);
                mesh_data.vertices[i * n + j].SetTangent(tangent);
                mesh_data.vertices[i * n + j].SetBinormal(binormal);
            }
        }
        mesh_data.indices.resize(face_count * 3); // 3 indices per face
        U32 k = 0;
        for (U32 i = 0; i < m - 1; ++i)
        {
            for (U32 j = 0; j < n - 1; ++j)
            {
                mesh_data.indices[k]     = i * n + j;
                mesh_data.indices[k + 1] = i * n + j + 1;
                mesh_data.indices[k + 2] = (i + 1) * n + j;
                mesh_data.indices[k + 3] = (i + 1) * n + j;
                mesh_data.indices[k + 4] = i * n + j + 1;
                mesh_data.indices[k + 5] = (i + 1) * n + j + 1;
                k += 6; // next quad
            }
        }
    }

    void MeshGenerator::CreateQuad(Real x, Real y, Real w, Real h, Real depth, MeshData& mesh_data) const
    {
        mesh_data.vertices.resize(4);
        mesh_data.indices.resize(6);
        // Position coordinates specified in NDC space.
        mesh_data.vertices[0] = VertexCommon(
                                             x, y - h, depth,
                                             0.0f, 1.0f,
                                             0.0f, 0.0f, -1.0f,
                                             1.0f, 0.0f, 0.0f
                                            );
        mesh_data.vertices[1] = VertexCommon(
                                             x, y, depth,
                                             0.0f, 0.0f,
                                             0.0f, 0.0f, -1.0f,
                                             1.0f, 0.0f, 0.0f
                                            );
        mesh_data.vertices[2] = VertexCommon(
                                             x + w, y, depth,
                                             1.0f, 0.0f,
                                             0.0f, 0.0f, -1.0f,
                                             1.0f, 0.0f, 0.0f
                                            );
        mesh_data.vertices[3] = VertexCommon(
                                             x + w, y - h, depth,
                                             1.0f, 1.0f,
                                             0.0f, 0.0f, -1.0f,
                                             1.0f, 0.0f, 0.0f
                                            );
        mesh_data.indices[0] = 0;
        mesh_data.indices[1] = 1;
        mesh_data.indices[2] = 2;
        mesh_data.indices[3] = 0;
        mesh_data.indices[4] = 2;
        mesh_data.indices[5] = 3;
        CalculateBinormal(mesh_data);
    }

    void MeshGenerator::Subdivide(MeshData& mesh_data) const
    {
        // Save a copy of the input geometry.
        MeshData input_data = mesh_data;
        mesh_data.vertices.resize(0);
        mesh_data.indices.resize(0);
        U32 tri_angle_count = (U32)input_data.indices.size() / 3;
        for (U32 i = 0; i < tri_angle_count; ++i)
        {
            VertexCommon v0 = input_data.vertices[input_data.indices[i * 3 + 0]];
            VertexCommon v1 = input_data.vertices[input_data.indices[i * 3 + 1]];
            VertexCommon v2 = input_data.vertices[input_data.indices[i * 3 + 2]];
            VertexCommon m0 = MidPoint(v0, v1);
            VertexCommon m1 = MidPoint(v1, v2);
            VertexCommon m2 = MidPoint(v0, v2);
            mesh_data.vertices.push_back(v0); // 0
            mesh_data.vertices.push_back(v1); // 1
            mesh_data.vertices.push_back(v2); // 2
            mesh_data.vertices.push_back(m0); // 3
            mesh_data.vertices.push_back(m1); // 4
            mesh_data.vertices.push_back(m2); // 5
            mesh_data.indices.push_back(i * 6 + 0);
            mesh_data.indices.push_back(i * 6 + 3);
            mesh_data.indices.push_back(i * 6 + 5);
            mesh_data.indices.push_back(i * 6 + 3);
            mesh_data.indices.push_back(i * 6 + 4);
            mesh_data.indices.push_back(i * 6 + 5);
            mesh_data.indices.push_back(i * 6 + 5);
            mesh_data.indices.push_back(i * 6 + 4);
            mesh_data.indices.push_back(i * 6 + 2);
            mesh_data.indices.push_back(i * 6 + 3);
            mesh_data.indices.push_back(i * 6 + 1);
            mesh_data.indices.push_back(i * 6 + 4);
        }
    }

    VertexCommon MeshGenerator::MidPoint(const VertexCommon& v0, const VertexCommon& v1) const
    {
        //vertex0
        Vector3 pos0 = v0.GetPosition();
        Vector2 uv0  = v0.GetUV();
        Vector3 nor0 = v0.GetNormal();
        Vector3 tan0 = v0.GetTangent();
        //vertex1
        Vector3 pos1 = v1.GetPosition();
        Vector2 uv1  = v1.GetUV();
        Vector3 nor1 = v1.GetNormal();
        Vector3 tan1 = v1.GetTangent();
        //mid vertex
        Vector3 pos = (pos0 + pos1).Half();
        Vector2 uv  = (uv0 + uv1).Half();
        Vector3 nor = (nor0 + nor1).Half().Normalize();
        Vector3 tan = (tan0 + tan1).Half().Normalize();
        Vector3 bin = CrossProduct(tan, nor).Normalize();
        return VertexCommon(pos, uv, nor, tan, bin);
    }

    void MeshGenerator::BuildCylinderTopCap(Real top_radius, Real height, U32 slice_count, MeshData& mesh_data) const
    {
        U32  base_index = (U32)mesh_data.vertices.size();
        Real y          = 0.5f * height;
        Real dtheta     = Math::TWO_PI / slice_count;
        // Duplicate cap ring vertices because the texture coordinates and normals differ.
        for (U32 i = 0; i <= slice_count; ++i)
        {
            Real x = top_radius * cosf(i * dtheta);
            Real z = top_radius * sinf(i * dtheta);
            // Scale down by the height to try and make top cap texture coordinate area
            // proportional to base.
            Real u = x / height + 0.5f;
            Real v = z / height + 0.5f;
            mesh_data.vertices.push_back(VertexCommon(x, y, z, u, v, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f));
        }
        // Cap center vertex.
        mesh_data.vertices.push_back(VertexCommon(0.0f, y, 0.0f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f));
        // Index of center vertex.
        U32 center_index = (U32)mesh_data.vertices.size() - 1;
        for (U32 i = 0; i < slice_count; ++i)
        {
            mesh_data.indices.push_back(center_index);
            mesh_data.indices.push_back(base_index + i + 1);
            mesh_data.indices.push_back(base_index + i);
        }
    }

    void MeshGenerator::BuildCylinderBottomCap(Real bottom_radius, Real height, U32 slice_count, MeshData& mesh_data) const
    {
        U32  base_index = (U32)mesh_data.vertices.size();
        Real y          = -0.5f * height;
        // vertices of ring
        Real dtheta = Math::TWO_PI / slice_count;
        for (U32 i = 0; i <= slice_count; ++i)
        {
            Real x = bottom_radius * cosf(i * dtheta);
            Real z = bottom_radius * sinf(i * dtheta);
            // Scale down by the height to try and make top cap texture coordinate area
            // proportional to base.
            Real u = x / height + 0.5f;
            Real v = z / height + 0.5f;
            mesh_data.vertices.push_back(VertexCommon(x, y, z, u, v, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f));
        }
        // Cap center vertex.
        mesh_data.vertices.push_back(VertexCommon(0.0f, y, 0.0f, 0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f));
        // Cache the index of center vertex.
        U32 center_index = (U32)mesh_data.vertices.size() - 1;
        for (U32 i = 0; i < slice_count; ++i)
        {
            mesh_data.indices.push_back(center_index);
            mesh_data.indices.push_back(base_index + i);
            mesh_data.indices.push_back(base_index + i + 1);
        }
    }

    void MeshGenerator::CalculateBinormal(MeshData& mesh_data) const
    {
        for (size_t i = 0; i < mesh_data.vertices.size(); ++i)
        {
            mesh_data.vertices[i].CalculateBinormal();
        }
    }
}
