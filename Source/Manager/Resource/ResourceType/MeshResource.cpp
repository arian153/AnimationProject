#include "MeshResource.hpp"
#include <fstream>
#include <sstream>

#include "../../../System/Graphics/DataType/MeshInfo.hpp"
#include "../../../System/Math/Algebra/Vector3.hpp"
#include "../../../System/Math/Algebra/Vector2.hpp"
#include "../../../System/Math/Primitive/ConvexHull2D/Triangle.hpp"

namespace CS460
{
    MeshResource::MeshResource(const std::wstring& path)
        : Resource(path)
    {
        m_type = eResourceType::Mesh;
    }

    MeshResource::~MeshResource()
    {
    }

    void MeshResource::Initialize()
    {
        std::ifstream file_stream;
        // Open the model file.
        file_stream.open(m_file_path_w);
        // If it could not open the file then exit.
        if (file_stream.fail())
        {
            return;
        }
        m_mesh_data.b_resource = true;
        CheckMeshType();
        switch (m_mesh_type)
        {
        case eMeshType::Invalid:
            break;
        case eMeshType::CustomTXT:
            LoadCustomTXT(file_stream);
            m_b_loaded = true;
            break;
        case eMeshType::WaveFrontOBJ:
            LoadOBJOnlyPos(file_stream);
            m_b_loaded = true;
            break;
        default:
            break;
        }
        file_stream.close();
    }

    void MeshResource::Shutdown()
    {
        m_b_unloaded = true;
    }

    void MeshResource::LoadWaveFrontOBJ(std::ifstream& file)
    {
        // Read in the vertices, texture coordinates, and normals into the data structures.
        // Important: Also convert to left hand coordinate system since Maya uses right hand coordinate system.
        size_t                         vertex_index = 0;
        size_t                         uv_index     = 0;
        size_t                         normal_index = 0;
        size_t                         face_index   = 0;
        char                           input, input2;
        std::vector<Vector3>           points;
        std::vector<Vector3>           normals;
        std::vector<Vector2>           uvs;
        std::vector<MeshFaceIndexInfo> faces;
        file.get(input);
        while (!file.eof())
        {
            if (input == 'v')
            {
                file.get(input);
                // Read in the vertices.
                if (input == ' ')
                {
                    Vector3 vertex;
                    file >> vertex.x >> vertex.y >> vertex.z;
                    // Invert the Z vertex to change to left hand system.
                    vertex.z = vertex.z * -1.0f;
                    vertex_index++;
                    points.push_back(vertex);
                }
                // Read in the texture uv coordinates.
                if (input == 't')
                {
                    Vector2 uv;
                    file >> uv.x >> uv.y;
                    // Invert the V texture coordinates to left hand system.
                    uv.y = 1.0f - uv.y;
                    uv_index++;
                    uvs.push_back(uv);
                }
                // Read in the normals.
                if (input == 'n')
                {
                    Vector3 normal;
                    file >> normal.x >> normal.y >> normal.z;
                    // Invert the Z normal to change to left hand system.
                    normal.z = normal.z * -1.0f;
                    normal_index++;
                    normals.push_back(normal);
                }
            }
            // Read in the faces.
            if (input == 'f')
            {
                file.get(input);
                if (input == ' ')
                {
                    MeshFaceIndexInfo face;
                    // Read the face data in backwards to convert it to a left hand system from right hand system.
                    file >> face.point_index_c;
                    file.get(input2);
                    if (input2 == ' ')
                    {
                        file >> face.point_index_b;
                        file >> face.point_index_a;
                    }
                    else if (input2 == '/')
                    {
                        file.get(input2);
                        if (input2 != '/')
                        {
                            file.unget();
                            file >> face.uv_index_c;
                            file.get(input2);
                            if (input2 == ' ')
                            {
                                file >> face.point_index_b;
                                file.get(input2);
                                file >> face.uv_index_b;
                                file >> face.point_index_a;
                                file.get(input2);
                                file >> face.uv_index_a;
                            }
                            else if (input2 == '/')
                            {
                                file >> face.normal_index_c;
                                file >> face.point_index_b;
                                file.get(input2);
                                file >> face.uv_index_b;
                                file.get(input2);
                                file >> face.normal_index_b;
                                file >> face.point_index_a;
                                file.get(input2);
                                file >> face.uv_index_a;
                                file.get(input2);
                                file >> face.normal_index_a;
                            }
                        }
                        else
                        {
                            file >> face.normal_index_c;
                            file >> face.point_index_b;
                            file.get(input2);
                            file.get(input2);
                            file >> face.normal_index_b;
                            file >> face.point_index_a;
                            file.get(input2);
                            file.get(input2);
                            file >> face.normal_index_a;
                        }
                    }
                    face_index++;
                    faces.push_back(face);
                }
            }
            // Read in the remainder of the line.
            while (input != '\n')
            {
                file.get(input);
            }
            // Start reading the beginning of the next line.
            file.get(input);
        }
        bool   b_uv     = !uvs.empty();
        bool   b_normal = !normals.empty();
        U32    index    = 0;
        size_t count    = faces.size() * 3;
        m_mesh_data.vertices.reserve(count);
        m_mesh_data.indices.reserve(count);
        for (auto& face : faces)
        {
            VertexCommon vertex_a, vertex_b, vertex_c;
            vertex_a.SetPosition(points[face.point_index_a - 1]);
            vertex_b.SetPosition(points[face.point_index_b - 1]);
            vertex_c.SetPosition(points[face.point_index_c - 1]);
            if (b_uv)
            {
                vertex_a.SetUV(uvs[face.uv_index_a - 1]);
                vertex_b.SetUV(uvs[face.uv_index_b - 1]);
                vertex_c.SetUV(uvs[face.uv_index_c - 1]);
            }
            if (b_normal)
            {
                vertex_a.SetNormal(normals[face.normal_index_a - 1]);
                vertex_b.SetNormal(normals[face.normal_index_b - 1]);
                vertex_c.SetNormal(normals[face.normal_index_c - 1]);
            }
            else
            {
                Vector3 normal = Triangle::Normal(vertex_a.GetPosition(), vertex_b.GetPosition(), vertex_c.GetPosition());
                vertex_a.SetNormal(normal);
                vertex_b.SetNormal(normal);
                vertex_c.SetNormal(normal);
            }
            vertex_a.CalculateTangentAndBinormal();
            vertex_b.CalculateTangentAndBinormal();
            vertex_c.CalculateTangentAndBinormal();
            m_mesh_data.vertices.push_back(vertex_a);
            m_mesh_data.vertices.push_back(vertex_b);
            m_mesh_data.vertices.push_back(vertex_c);
            m_mesh_data.indices.push_back(index);
            m_mesh_data.indices.push_back(index + 1);
            m_mesh_data.indices.push_back(index + 2);
            index += 3;
        }
    }

    void MeshResource::LoadGeneralOBJ(std::ifstream& file)
    {
        std::string name = m_file_name_m;
        //indices
        U32    point_index  = 0;
        size_t uv_index     = 0;
        size_t normal_index = 0;
        size_t face_index   = 0;
        //container
        std::vector<Vector3>           points;
        std::vector<Vector3>           normals;
        std::vector<Vector2>           uvs;
        std::vector<MeshFaceIndexInfo> faces;
        //line, char
        String line;
        char   next_input;
        while (std::getline(file, line))
        {
            String             text;
            std::istringstream string_stream(line);
            string_stream >> text;
            // Read points.
            if (text == "v")
            {
                Vector3 point;
                string_stream >> point.x >> point.y >> point.z;
                point.z = point.z * -1.0f;
                points.push_back(point);
                point_index++;
            }
            // Read texture uv coordinates.
            if (text == "vt")
            {
                Vector2 uv;
                string_stream >> uv.x >> uv.y;
                uv.y = 1.0f - uv.y;
                uv_index++;
                uvs.push_back(uv);
            }
            // Read normals.
            if (text == "vn")
            {
                Vector3 normal;
                string_stream >> normal.x >> normal.y >> normal.z;
                normal.z = normal.z * -1.0f;
                normal_index++;
                normals.push_back(normal);
            }
            // Read faces.
            if (text == "f")
            {
                size_t index_count = std::count(line.begin(), line.end(), ' ');
                size_t slash_count = std::count(line.begin(), line.end(), '/');

                std::vector<MeshVertexIndexInfo> model_indices;
                size_t                           line_of_faces_size = index_count - 2;
                model_indices.resize(index_count);
                eOBJFaceType type = eOBJFaceType::Point;
                if (slash_count == 0)
                {
                    type = eOBJFaceType::Point;
                }
                else if (slash_count == index_count)
                {
                    type = eOBJFaceType::PointTexture;
                }
                else if (slash_count == index_count * 2)
                {
                    if (line.find("//") != String::npos)
                    {
                        type = eOBJFaceType::PointNormal;
                    }
                    else
                    {
                        type = eOBJFaceType::PointTextureNormal;
                    }
                }
                else
                {
                    return;
                }

                for (size_t i = 0; i < index_count; ++i)
                {
                    if (type == eOBJFaceType::Point)
                    {
                        string_stream >> model_indices[i].point_index;
                    }
                    else if (type == eOBJFaceType::PointTexture)
                    {
                        string_stream >> model_indices[i].point_index;
                        string_stream.get(next_input);
                        string_stream >> model_indices[i].uv_index;
                    }
                    else if (type == eOBJFaceType::PointNormal)
                    {
                        string_stream >> model_indices[i].point_index;
                        string_stream.get(next_input);
                        string_stream.get(next_input);
                        string_stream >> model_indices[i].normal_index;
                    }
                    else if (type == eOBJFaceType::PointTextureNormal)
                    {
                        string_stream >> model_indices[i].point_index;
                        string_stream.get(next_input);
                        string_stream >> model_indices[i].uv_index;
                        string_stream.get(next_input);
                        string_stream >> model_indices[i].normal_index;
                    }
                }
                for (size_t i = 0; i < line_of_faces_size; ++i)
                {
                    faces.emplace_back(model_indices[i + 2], model_indices[i + 1], model_indices[0]);
                }
                face_index += line_of_faces_size;
            }
        }
        bool b_uv     = !uvs.empty();
        bool b_normal = !normals.empty();
        U32  index    = 0;
        for (auto& face : faces)
        {
            VertexCommon vertex_a, vertex_b, vertex_c;
            vertex_a.SetPosition(points[face.point_index_a - 1]);
            vertex_b.SetPosition(points[face.point_index_b - 1]);
            vertex_c.SetPosition(points[face.point_index_c - 1]);
            if (b_uv)
            {
                if (face.uv_index_a > 0)
                    vertex_a.SetUV(uvs[face.uv_index_a - 1]);
                if (face.uv_index_b > 0)
                    vertex_b.SetUV(uvs[face.uv_index_b - 1]);
                if (face.uv_index_c > 0)
                    vertex_c.SetUV(uvs[face.uv_index_c - 1]);
            }
            if (b_normal)
            {
                if (face.normal_index_a > 0)
                    vertex_a.SetNormal(normals[face.normal_index_a - 1]);
                if (face.normal_index_b > 0)
                    vertex_b.SetNormal(normals[face.normal_index_b - 1]);
                if (face.normal_index_c > 0)
                    vertex_c.SetNormal(normals[face.normal_index_c - 1]);
            }
            //Add vertex info into mesh data
            m_mesh_data.vertices.push_back(vertex_a);
            m_mesh_data.vertices.push_back(vertex_b);
            m_mesh_data.vertices.push_back(vertex_c);

            //Add face info and index info into mesh data
            m_mesh_data.faces.emplace_back(index, index + 1, index + 2);
            m_mesh_data.indices.push_back(index);
            m_mesh_data.indices.push_back(index + 1);
            m_mesh_data.indices.push_back(index + 2);
            index += 3;
        }

        //process point based calculation
    }

    void MeshResource::LoadOBJOnlyPos(std::ifstream& file)
    {
        std::string name = m_file_name_m;
        //indices
        U32 point_index = 0;
        U32 face_index  = 0;
        //container
        std::vector<MeshFaceIndexInfo>  faces;
        std::vector<GeometryPointIndex> point_indices;
        //line, char
        String line;
        char   next_input;
        while (std::getline(file, line))
        {
            String             text;
            std::istringstream string_stream(line);
            string_stream >> text;
            // Read points.
            if (text == "v")
            {
                Vector3 point;
                string_stream >> point.x >> point.y >> point.z;
                point.z = point.z * -1.0f;
                m_mesh_data.vertices.emplace_back(point);
                point_indices.emplace_back(point_index);
                point_index++;
            }
            // Read faces.
            if (text == "f")
            {
                U32 index_count = (U32)std::count(line.begin(), line.end(), ' ');
                U32 slash_count = (U32)std::count(line.begin(), line.end(), '/');

                std::vector<MeshVertexIndexInfo> model_indices;
                U32                              line_of_faces_size = index_count - 2;
                model_indices.resize(index_count);
                eOBJFaceType type = eOBJFaceType::Point;
                if (slash_count == 0)
                {
                    type = eOBJFaceType::Point;
                }
                else if (slash_count == index_count)
                {
                    type = eOBJFaceType::PointTexture;
                }
                else if (slash_count == index_count * 2)
                {
                    if (line.find("//") != String::npos)
                    {
                        type = eOBJFaceType::PointNormal;
                    }
                    else
                    {
                        type = eOBJFaceType::PointTextureNormal;
                    }
                }
                else
                {
                    return;
                }

                for (U32 i = 0; i < index_count; ++i)
                {
                    if (type == eOBJFaceType::Point)
                    {
                        string_stream >> model_indices[i].point_index;
                    }
                    else if (type == eOBJFaceType::PointTexture)
                    {
                        string_stream >> model_indices[i].point_index;
                        string_stream.get(next_input);
                        string_stream >> model_indices[i].uv_index;
                    }
                    else if (type == eOBJFaceType::PointNormal)
                    {
                        string_stream >> model_indices[i].point_index;
                        string_stream.get(next_input);
                        string_stream.get(next_input);
                        string_stream >> model_indices[i].normal_index;
                    }
                    else if (type == eOBJFaceType::PointTextureNormal)
                    {
                        string_stream >> model_indices[i].point_index;
                        string_stream.get(next_input);
                        string_stream >> model_indices[i].uv_index;
                        string_stream.get(next_input);
                        string_stream >> model_indices[i].normal_index;
                    }
                }
                for (size_t i = 0; i < line_of_faces_size; ++i)
                {
                    faces.emplace_back(model_indices[i + 2], model_indices[i + 1], model_indices[0]);
                }
                face_index += line_of_faces_size;
            }
        }
        U32         index     = 0;
        std::string test_name = m_file_name_m;
        for (auto& face : faces)
        {
            //set index start from 0.
            U32 point_a = face.point_index_a - 1;
            U32 point_b = face.point_index_b - 1;
            U32 point_c = face.point_index_c - 1;
            //add adjacent faces
            point_indices[point_a].faces.emplace_back(point_a, point_b, point_c);
            point_indices[point_b].faces.emplace_back(point_a, point_b, point_c);
            point_indices[point_c].faces.emplace_back(point_a, point_b, point_c);
            //Add face info and index info into mesh data
            m_mesh_data.faces.emplace_back(point_a, point_b, point_c);
            m_mesh_data.indices.push_back(point_a);
            m_mesh_data.indices.push_back(point_b);
            m_mesh_data.indices.push_back(point_c);
            index += 3;
        }
        Vector3 min, max;
        m_mesh_data.Normalize(min, max);

        //Calculate Vertex Normal
        std::vector<Vector3> normals;
        Vector3              accumulated_normal;
        for (auto& point : point_indices)
        {
            normals.clear();
            accumulated_normal.SetZero();
            for (auto& face : point.faces)
            {
                Vector3 normal = m_mesh_data.GetFaceNormal(face.a, face.b, face.c);
                auto    found  = std::find(normals.begin(), normals.end(), normal);
                if (found == normals.end())
                {
                    accumulated_normal += normal;
                    normals.push_back(normal);
                }
            }
            m_mesh_data.vertices[point.index].SetNormal(accumulated_normal.Normalize());
            m_mesh_data.vertices[point.index].CalculateTangentAndBinormal();
        }

        //Calculate Planar UV Mapping
        size_t size = m_mesh_data.vertices.size();
        for (size_t i = 0; i < size; ++i)
        {
            Vector2 uv;
            Vector3 point   = m_mesh_data.vertices[i].GetPosition();
            Vector3 abs_vec = point.Absolute();
            // +-X
            if (abs_vec.x >= abs_vec.y && abs_vec.x >= abs_vec.z)
            {
                if (point.x < 0.0)
                {
                    uv.x = point.z / abs_vec.x;
                }
                else
                {
                    uv.x = -point.z / abs_vec.x;
                }
                uv.y = point.y / abs_vec.x;
            }
            // +-Y
            if (abs_vec.y >= abs_vec.x && abs_vec.y >= abs_vec.z)
            {
                if (point.y < 0.0)
                {
                    uv.y = point.x / abs_vec.y;
                }
                else
                {
                    uv.y = -point.x / abs_vec.y;
                }
                uv.x = -point.z / abs_vec.y;
            }
            // +-Z
            if (abs_vec.z >= abs_vec.x && abs_vec.z >= abs_vec.y)
            {
                if (point.z < 0.0)
                {
                    uv.x = -point.x / abs_vec.z;
                }
                else
                {
                    uv.x = point.x / abs_vec.z;
                }
                uv.y = point.y / abs_vec.z;
            }
            Vector2 tex_coords = 0.5f * (uv + Vector2(1.0, 1.0));
            m_mesh_data.vertices[i].SetUV(tex_coords);
        }
    }

    void MeshResource::LoadCustomTXT(std::ifstream& file)
    {
        char input;
        // Read up to the value of vertex count.
        file.get(input);
        while (input != ':')
        {
            file.get(input);
        }
        // Read in the vertex count.
        U32 count = 0;
        file >> count;
        if (count == 0)
        {
            return;
        }
        // Create the model using the vertex count that was read in.
        m_mesh_data.vertices.reserve(count);
        m_mesh_data.indices.reserve(count);
        // Read up to the beginning of the data.
        file.get(input);
        while (input != ':')
        {
            file.get(input);
        }
        file.get(input);
        file.get(input);
        // Read in the vertex data.
        for (U32 i = 0; i < count; i++)
        {
            VertexCommon vertex;
            file >> vertex.position.x >> vertex.position.y >> vertex.position.z;
            file >> vertex.uv.x >> vertex.uv.y;
            file >> vertex.normal.x >> vertex.normal.y >> vertex.normal.z;
            m_mesh_data.vertices.push_back(vertex);
            m_mesh_data.indices.push_back(i);
        }
    }

    void MeshResource::CheckMeshType()
    {
        if (m_file_type_w == L".obj")
        {
            m_mesh_type = eMeshType::WaveFrontOBJ;
        }
        else if (m_file_type_w == L".txtmdl")
        {
            m_mesh_type = eMeshType::CustomTXT;
        }
    }

    MeshData* MeshResource::GetMeshData()
    {
        return &m_mesh_data;
    }
}
