#include "ColliderPolyhedron.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../../BroadPhase/BoundingAABB.hpp"
#include <list>
#include "../../../../Manager/Resource/ResourceType/JsonResource.hpp"
#include "../../../../External/JSONCPP/json/json.h"

namespace CS460
{
    ColliderPolyhedron::ColliderPolyhedron()
    {
        m_type = eColliderType::Polyhedron;
    }

    ColliderPolyhedron::~ColliderPolyhedron()
    {
    }

    void ColliderPolyhedron::Initialize()
    {
        if (m_vertices == nullptr)
        {
            m_vertices = new std::vector<Vector3>();
        }
        if (m_scaled_vertices == nullptr)
        {
            m_scaled_vertices = new std::vector<Vector3>();
        }
        if (m_edges == nullptr)
        {
            m_edges = new std::vector<ColliderEdge>();
        }
        if (m_faces == nullptr)
        {
            m_faces = new std::vector<ColliderFace>();
        }
        UpdatePrimitive();
    }

    void ColliderPolyhedron::Shutdown()
    {
        if (m_vertices != nullptr)
        {
            m_vertices->clear();
            delete m_vertices;
            m_vertices = nullptr;
        }
        if (m_scaled_vertices != nullptr)
        {
            m_scaled_vertices->clear();
            delete m_scaled_vertices;
            m_scaled_vertices = nullptr;
        }
        if (m_edges != nullptr)
        {
            m_edges->clear();
            delete m_edges;
            m_edges = nullptr;
        }
        if (m_faces != nullptr)
        {
            m_faces->clear();
            delete m_faces;
            m_faces = nullptr;
        }
    }

    Vector3 ColliderPolyhedron::Support(const Vector3& direction)
    {
        Real                  p = Math::REAL_NEGATIVE_MAX;
        Vector3               result;
        std::vector<Vector3>* vertices;
        if (m_collider_set != nullptr)
        {
            vertices = m_scaled_vertices;
        }
        else
        {
            vertices = m_vertices;
        }
        size_t size = vertices->size();
        for (size_t i = 0; i < size; ++i)
        {
            Real projection = vertices->at(i).DotProduct(direction);
            if (projection > p)
            {
                result = vertices->at(i);
                p      = projection;
            }
        }
        return result;
    }

    bool ColliderPolyhedron::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        bool b_first   = true;
        int  hit_count = 0;
        minimum_t      = -1.0f;
        maximum_t      = -1.0f;
        for (auto& face : *m_faces)
        {
            Real t = -1.0f;
            if (IntersectRayFace(local_ray, face, t) == true)
            {
                if (b_first == true)
                {
                    minimum_t = t;
                    maximum_t = t;
                    b_first   = false;
                }
                else
                {
                    if (t > maximum_t)
                    {
                        maximum_t = t;
                    }
                    if (t < minimum_t)
                    {
                        minimum_t = t;
                    }
                }
                hit_count++;
            }
        }
        if (hit_count > 0)
        {
            if (minimum_t < 0.0f && maximum_t < 0.0f)
            {
                return false;
            }
            if (minimum_t <= 0.0f)
            {
                minimum_t = 0.0f;
            }
            return true;
        }
        return false;
    }

    Vector3 ColliderPolyhedron::GetNormal(const Vector3& local_point_on_collider)
    {
        Vector3 normal;
        for (auto& face : *m_faces)
        {
            if (this->IsFaceContainPoint(face, local_point_on_collider, normal) == true)
            {
                return normal;
            }
        }
        return normal;
    }

    void ColliderPolyhedron::SetMassData(Real density)
    {
        if (Math::IsNotEqual(m_material.density, density))
        {
            m_material.density = density;
            m_material.type    = Physics::eMaterial::UserType;
        }
        m_centroid = 0.0f;
        m_local_inertia_tensor.SetZero();
        m_mass            = 0.0f;
        Vector3 ref_point = Math::Vector3::ORIGIN;
        for (auto& face : *m_faces)
        {
            auto sub_data = CalculateTetrahedronMassData(ref_point, Vertex(face.a), Vertex(face.b), Vertex(face.c), density);
            m_centroid += sub_data.mass * sub_data.centroid;
            m_mass += sub_data.mass;
            m_local_inertia_tensor += sub_data.inertia;
        }
        if (Math::IsZero(m_mass) == false)
        {
            m_centroid /= m_mass;
        }
        else
        {
            m_centroid = 0.0f;
        }
        m_local_inertia_tensor = TranslateInertia(m_local_inertia_tensor, m_centroid, m_mass, ref_point - m_centroid);
    }

    Real ColliderPolyhedron::GetVolume()
    {
        Real    volume    = 0.0f;
        Vector3 ref_point = Math::Vector3::ORIGIN;
        for (auto& face : *m_faces)
        {
            Matrix33 tetrahedron_matrix;
            tetrahedron_matrix.SetColumns(Vertex(face.a) - ref_point, Vertex(face.b) - ref_point, Vertex(face.c) - ref_point);
            volume += tetrahedron_matrix.Determinant() / 6.0f;
        }
        return volume;
    }

    void ColliderPolyhedron::SetScaleData(const Vector3& scale)
    {
        m_scaled_vertices->clear();
        size_t size = m_vertices->size();
        m_scaled_vertices->reserve(size);
        for (size_t i = 0; i < size; ++i)
        {
            m_scaled_vertices->push_back(m_vertices->at(i).HadamardProduct(scale.HadamardProduct(m_local.scale)));
            for (size_t j = 0; j < 3; ++j)
            {
                if (m_max_bound[j] < m_scaled_vertices->at(i)[j])
                {
                    m_max_bound[j] = m_scaled_vertices->at(i)[j];
                }
                if (m_min_bound[j] > m_scaled_vertices->at(i)[j])
                {
                    m_min_bound[j] = m_scaled_vertices->at(i)[j];
                }
            }
        }
    }

    void ColliderPolyhedron::SetUnit()
    {
        CalculateMinMaxBound();
        auto   scale_factor = (m_max_bound - m_min_bound).Inverse();
        size_t size         = m_vertices->size();
        for (size_t i = 0; i < size; ++i)
        {
            m_scaled_vertices->at(i) = m_vertices->at(i).HadamardProduct(scale_factor);
        }
        UpdatePrimitive();
    }

    void ColliderPolyhedron::UpdateBoundingVolume()
    {
        Real    bounding_factor = (m_max_bound - m_min_bound).Length();
        Vector3 pos             = m_rigid_body != nullptr ? m_rigid_body->LocalToWorldPoint(m_local.position) : m_local.position;
        Vector3 min_max(bounding_factor, bounding_factor, bounding_factor);
        m_bounding_volume->Set(-min_max + pos, min_max + pos);

        Vector3 obb_vertices[8];
        obb_vertices[0].Set(m_max_bound.x, m_max_bound.y, m_max_bound.z);
        obb_vertices[1].Set(m_max_bound.x, m_max_bound.y, m_min_bound.z);
        obb_vertices[2].Set(m_max_bound.x, m_min_bound.y, m_max_bound.z);
        obb_vertices[3].Set(m_max_bound.x, m_min_bound.y, m_min_bound.z);
        obb_vertices[4].Set(m_min_bound.x, m_max_bound.y, m_max_bound.z);
        obb_vertices[5].Set(m_min_bound.x, m_max_bound.y, m_min_bound.z);
        obb_vertices[6].Set(m_min_bound.x, m_min_bound.y, m_max_bound.z);
        obb_vertices[7].Set(m_min_bound.x, m_min_bound.y, m_min_bound.z);

        bool has_body = m_rigid_body != nullptr;

        Vector3 min = has_body
                          ? m_rigid_body->LocalToWorldPoint(m_local.LocalToWorldPoint(obb_vertices[0]))
                          : m_local.LocalToWorldPoint(obb_vertices[0]);
        Vector3 max = min;

        if (has_body)
        {
            for (int i = 1; i < 8; ++i)
            {
                Vector3 vertex = m_rigid_body->LocalToWorldPoint(m_local.LocalToWorldPoint(obb_vertices[i]));
                min.x          = Math::Min(min.x, vertex.x);
                min.y          = Math::Min(min.y, vertex.y);
                min.z          = Math::Min(min.z, vertex.z);
                max.x          = Math::Max(max.x, vertex.x);
                max.y          = Math::Max(max.y, vertex.y);
                max.z          = Math::Max(max.z, vertex.z);
            }
        }
        else
        {
            for (int i = 1; i < 8; ++i)
            {
                Vector3 vertex = m_local.LocalToWorldPoint(obb_vertices[i]);
                min.x          = Math::Min(min.x, vertex.x);
                min.y          = Math::Min(min.y, vertex.y);
                min.z          = Math::Min(min.z, vertex.z);
                max.x          = Math::Max(max.x, vertex.x);
                max.y          = Math::Max(max.y, vertex.y);
                max.z          = Math::Max(max.z, vertex.z);
            }
        }

        m_bounding_volume->Set(min, max);
    }

    void ColliderPolyhedron::Draw(PrimitiveRenderer* renderer, eRenderingMode mode, const Color& color) const
    {
        I32                   index = static_cast<I32>(renderer->VerticesSize(mode));
        std::vector<Vector3>* vertices;
        if (m_collider_set != nullptr)
        {
            vertices = m_scaled_vertices;
        }
        else
        {
            vertices = m_vertices;
        }
        size_t size = vertices->size();
        renderer->ReserveVertices(size, mode);
        Transform* body_transform = GetBodyTransform();
        for (auto& vertex : *vertices)
        {
            //collider local space to object space(body local)
            vertex = LocalToWorldPoint(vertex);
            vertex = body_transform->LocalToWorldPoint(vertex);
            //push to renderer
            renderer->PushVertex(vertex, mode, color);
        }
        //add indices
        if (mode == eRenderingMode::Dot)
        {
            for (I32 i = 0; i < size; ++i)
            {
                renderer->PushIndex(index + i, mode);
            }
        }
        else if (mode == eRenderingMode::Line)
        {
            for (auto& face : *m_faces)
            {
                renderer->PushLineIndices(index + (I32)face.a, index + (I32)face.b);
                renderer->PushLineIndices(index + (I32)face.b, index + (I32)face.c);
                renderer->PushLineIndices(index + (I32)face.c, index + (I32)face.a);
            }
        }
        else if (mode == eRenderingMode::Face)
        {
            for (auto& face : *m_faces)
            {
                renderer->PushFaceIndices(index + (I32)face.a, index + (I32)face.b, index + (I32)face.c);
            }
        }
    }

    Vector3 ColliderPolyhedron::Vertex(size_t i) const
    {
        if (m_collider_set != nullptr)
        {
            return m_scaled_vertices->at(i);
        }
        return m_vertices->at(i);
    }

    size_t ColliderPolyhedron::Size() const
    {
        if (m_collider_set != nullptr)
        {
            return m_scaled_vertices->size();
        }
        return m_vertices->size();
    }

    bool ColliderPolyhedron::SetPolyhedron(const std::vector<Vector3>& input_vertices)
    {
        //0. Early quit when not enough vertices.
        size_t size = input_vertices.size();
        if (size < 4)
        {
            return false;
        }
        //1. Allocate a Polyhedron which will be our return value.
        if (m_vertices == nullptr)
        {
            m_vertices = new std::vector<Vector3>();
            m_vertices->reserve(size);
        }
        else
        {
            m_vertices->clear();
            m_vertices->reserve(size);
        }
        std::vector<Vector3> vertices = input_vertices;
        //2. Find a maximal simplex of the current dimension.In our case, 
        //find 4 points which define maximal tetrahedron, and create the tetrahedron which will be our seed partial answer.
        //CREATE_SIMPLEX(Polyhedron, listVertices).
        //If this returns a dimension lower than 3, report the degeneracy and quit.
        size_t dimension = CreateSimplex(vertices);
        if (dimension < 3)
        {
            return false;
        }
        //3. Divide the points of the cloud into the outside sets of the four faces of the tetrahedron.
        //This is done by calling ADD_TO_OUTSIDE_SET(face, listVertices) on each of the 4 faces in turn.
        //Points that could be put in multiple sets will be randomly placed in one of them, and it does not matter which one.
        //Any points which are inside or on all of the planes do not contribute to the convex hull.
        //These points are removed from the rest of the calculation.
        std::list<OutsideSetFace> outside_set_list;
        for (auto face : *m_faces)
        {
            OutsideSetFace outside_set(face);
            AddToOutsideSet(vertices, outside_set);
            outside_set_list.push_back(outside_set);
        }
        //4. While(There exists currFace = a face on the current convex hull which has a non - empty outside set of vertices)
        while (!outside_set_list.empty())
        {
            //1. Find the point in currFace's outside set which is farthest away from the plane of the currFace. 
            //This is the eyePoint.
            //2. Compute the horizon of the current polyhedron as seen from this eyePoint.
            //CALCULATE_HORIZON(eyePoint, NULL, currFace, listHorizonEdges, listUnclaimedVertices).
            //This will mark all of the visible faces as not on the convex hull and place all of their outside set points on the listUnclaimedVertices.
            //It is creates the listHorizonEdges which is a counterclockwise ordered list of edges around the horizon contour of the polyhedron as viewed from the eyePoint.
            //3. Construct a cone from the eye point to all of the edges of the horizon.
            //Before a triangle of this cone is created and added to the polyhedron a test to see if it coplanar to the face on the other side of the horizon edge is applied.
            //If the faces are coplanar then they are merged together and that horizon edge is marked as not on the convex hull.Then a similar check must also be applied to edges which cross the horizon to see if they are collinear.
            //If so then the vertex on the horizon which is on this line must be marked as not on the horizon.
            //4. Divide the points of the listUnclaimedVertices into the outside sets of the new cone faces and the faces which were merged with what would be new cone faces.
            //This is done by calling ADD_TO_OUTSIDE_SET(face, listUnclaimedVertices) on each of these faces in turn.
            //Points that could be put in multiple sets will be randomly placed in one of them, and it does not matter which one.
            //Any points which are inside or on all of the planes do not contribute to the convex hull.
            //These points are marked as not on the convex hull.
            //5. Remove all faces, edges, and vertices which have been marked as not on the convex hull.
            //These faces, edges, and vertices have all been enclosed by the new cone emanating from the eyePoint.
            //6. Continue the loop
        }
        //5. When all the faces with outside points have been processed we are done.
        //The polyhedron should a clean convex hull where all coplanar faces have been merged.
        //All collinear edges have been merged.
        //And all coincident vertices have had their DualFace pointers merged.
        return true;
    }

    void ColliderPolyhedron::Clone(ColliderPrimitive* origin)
    {
        if (origin != this && origin != nullptr && origin->Type() == m_type)
        {
            ColliderPolyhedron* polyhedron = static_cast<ColliderPolyhedron*>(origin);
            //collider local space data
            m_local = polyhedron->m_local;
            //collider mass data
            m_centroid             = polyhedron->m_centroid;
            m_mass                 = polyhedron->m_mass;
            m_local_inertia_tensor = polyhedron->m_local_inertia_tensor;
            m_material             = polyhedron->m_material;
            //polyhedron
            *m_vertices        = *polyhedron->m_vertices;
            *m_scaled_vertices = *polyhedron->m_scaled_vertices;
            *m_edges           = *polyhedron->m_edges;
            *m_faces           = *polyhedron->m_faces;
            m_min_bound        = polyhedron->m_min_bound;
            m_max_bound        = polyhedron->m_max_bound;
        }
    }

    void ColliderPolyhedron::Load(const Json::Value& data)
    {
        LoadTransform(data);
        //polyhedron data
        if (JsonResource::HasMember(data, "Vertices") && data["Vertices"].isArray() && data["Vertices"].size() >= 4)
        {
            m_vertices->clear();
            m_scaled_vertices->clear();
            for (auto it = data["Vertices"].begin(); it != data["Vertices"].end(); ++it)
            {
                if (JsonResource::IsVector3(*it))
                {
                    Vector3 vertex = JsonResource::AsVector3(*it);
                    m_vertices->push_back(vertex);
                }
            }
        }
        if (JsonResource::HasMember(data, "Edges") && data["Edges"].isArray())
        {
            m_edges->clear();
            for (auto it = data["Edges"].begin(); it != data["Edges"].end(); ++it)
            {
                if (JsonResource::IsVector2(*it))
                {
                    m_edges->emplace_back((size_t)data[0].asUInt64(), (size_t)data[1].asUInt64());
                }
            }
        }
        if (JsonResource::HasMember(data, "Faces") && data["Faces"].isArray())
        {
            m_faces->clear();
            for (auto it = data["Faces"].begin(); it != data["Faces"].end(); ++it)
            {
                if (JsonResource::IsVector3(*it))
                {
                    m_faces->emplace_back((size_t)data[0].asUInt64(), (size_t)data[1].asUInt64(), (size_t)data[2].asUInt64());
                }
            }
        }
        SetPolyhedron(*m_vertices);
        LoadMaterial(data);
        LoadMass(data);
    }

    void ColliderPolyhedron::Save(const Json::Value& data)
    {
    }

    void ColliderPolyhedron::EditPrimitive(CommandRegistry* registry)
    {
    }

    bool ColliderPolyhedron::IntersectRayFace(const Ray& ray, const ColliderFace& face, Real& t) const
    {
        std::vector<Vector3>* vertices;
        if (m_collider_set != nullptr)
        {
            vertices = m_scaled_vertices;
        }
        else
        {
            vertices = m_vertices;
        }
        Vector3 edge1 = (*vertices)[face.b] - (*vertices)[face.a];
        Vector3 edge2 = (*vertices)[face.c] - (*vertices)[face.a];
        Vector3 h     = ray.direction.CrossProduct(edge2);
        Real    a     = edge1.DotProduct(h);
        t             = -1.0f;
        if (Math::IsZero(a))
        {
            return false;
        }
        Real    f = 1.0f / a;
        Vector3 s = ray.position - (*vertices)[face.a];
        Real    u = f * (s.DotProduct(h));
        if (u < 0.0f || u > 1.0f)
        {
            return false;
        }
        Vector3 q = s.CrossProduct(edge1);
        Real    v = f * ray.direction.DotProduct(q);
        if (v < 0.0f || u + v > 1.0f)
        {
            return false;
        }
        // At this stage we can compute t to find out where the intersection point is on the line.
        t = f * edge2.DotProduct(q);
        if (t > Math::EPSILON) // ray intersection
        {
            return true;
        }
        // intersect back side of ray.
        return false;
    }

    bool ColliderPolyhedron::IsFaceContainPoint(const ColliderFace& face, const Vector3& point, Vector3& normal) const
    {
        std::vector<Vector3>* vertices;
        if (m_collider_set != nullptr)
        {
            vertices = m_scaled_vertices;
        }
        else
        {
            vertices = m_vertices;
        }
        Vector3 v0     = vertices->at(face.a);
        Vector3 v1     = vertices->at(face.b);
        Vector3 v2     = vertices->at(face.c);
        Vector3 edge01 = v1 - v0;
        Vector3 edge12 = v2 - v1;
        normal         = edge01.CrossProduct(edge12);
        Vector3 w_test = edge01.CrossProduct(point - v0);
        if (w_test.DotProduct(normal) < 0.0f)
        {
            return false;
        }
        w_test = edge12.CrossProduct(point - v1);
        if (w_test.DotProduct(normal) < 0.0f)
        {
            return false;
        }
        Vector3 edge3 = v0 - v2;
        w_test        = edge3.CrossProduct(point - v2);
        if (w_test.DotProduct(normal) < 0.0f)
        {
            return false;
        }
        normal.SetNormalize();
        return true;
    }

    Matrix33 ColliderPolyhedron::TranslateInertia(const Matrix33& input, const Vector3& centroid, Real mass, const Vector3& offset) const
    {
        return input + mass * (offset.OuterProduct(centroid) + centroid.OuterProduct(offset) + offset.OuterProduct(offset));
    }

    ColliderPolyhedron::SubMassData ColliderPolyhedron::CalculateTetrahedronMassData(const Vector3& ref, const Vector3& v1, const Vector3& v2, const Vector3& v3, Real density) const
    {
        SubMassData result;
        //canonical moment of inertia data.
        Real     axis = 1.0f / 60.0f;
        Real     prod = 1.0f / 120.0f;
        Matrix33 canonical_matrix(axis, prod, prod, prod, axis, prod, prod, prod, axis);
        Matrix33 transformation_matrix;
        transformation_matrix.SetColumns(v1 - ref, v2 - ref, v3 - ref);
        //calculate sub inertia
        result.inertia = transformation_matrix.Determinant() * transformation_matrix * canonical_matrix * transformation_matrix.Transpose();
        //volume is 1 / 6 of triple product, that is 1/6 det of transformation matrix.
        result.mass = density * transformation_matrix.Determinant() / 6.0f;
        //The center-of-mass is just the mean of the four vertex coordinates. 
        result.centroid = (ref + v1 + v2 + v3) * 0.25f;
        if (ref.IsZero() == false)
        {
            result.inertia = TranslateInertia(result.inertia, result.centroid, result.mass, ref);
        }
        return result;
    }

    void ColliderPolyhedron::CalculateMinMaxBound()
    {
        m_max_bound = Math::REAL_NEGATIVE_MAX;
        m_min_bound = Math::REAL_POSITIVE_MAX;
        size_t size = m_scaled_vertices->size();
        for (size_t i = 0; i < size; ++i)
        {
            for (size_t j = 0; j < 3; ++j)
            {
                Real value = m_scaled_vertices->at(i)[j];
                if (m_max_bound[j] < value)
                {
                    m_max_bound[j] = value;
                }
                if (m_min_bound[j] > value)
                {
                    m_min_bound[j] = value;
                }
            }
        }
    }

    size_t ColliderPolyhedron::CreateSimplex(const std::vector<Vector3>& vertices) const
    {
        //1. For each principle direction(X - axis, Y - axis, Z - axis)
        Vector3 min_vertex, max_vertex;
        bool    b_succeed = false;
        for (size_t i = 0; i < 3; ++i)
        {
            //  1. Find the minimum and maximum point in that dimension
            Real min = Math::REAL_POSITIVE_MAX;
            Real max = Math::REAL_NEGATIVE_MAX;
            for (auto& vertex : vertices)
            {
                if (vertex[i] < min)
                {
                    min        = vertex[i];
                    min_vertex = vertex;
                }
                if (vertex[i] > max)
                {
                    max        = vertex[i];
                    max_vertex = vertex;
                }
            }
            //  2. If min != max then break out of loop with success
            if (min != max)
            {
                b_succeed = true;
                break;
            }
            //  3. Continue the loop
        }
        //2. If the loop finished without success return and report that we have a degenerate point cloud, either 1D or 2D.
        if (b_succeed == false)
        {
            return 0;
        }
        //3. Find the point which is furthest distant from the line defined by the first two points.If this maximum distance is zero, then our point cloud is 1D, so report the degeneracy.
        Segment segment(min_vertex, max_vertex);
        Real    max_distance = 0.0f;
        Real    t            = 0.0f;
        Vector3 max_triangle;
        for (auto& vertex : vertices)
        {
            Real distance = segment.DistanceSquared(vertex, t);
            if (distance > max_distance)
            {
                max_distance = distance;
                max_triangle = vertex;
            }
        }
        if (Math::IsZero(max_distance))
        {
            return 1;
        }
        //4. Find the point which has the largest absolute distance from the plane defined by the first three points.If this maximum distance is zero, then our point cloud is 2D, so report the the degeneracy.
        max_distance = 0.0f;
        Vector3 max_tetrahedron;
        for (auto& vertex : vertices)
        {
            Real distance = Triangle::DistanceSquared(vertex, min_vertex, max_vertex, max_triangle);
            if (distance > max_distance)
            {
                max_distance    = distance;
                max_tetrahedron = vertex;
            }
        }
        if (Math::IsZero(max_distance))
        {
            return 2;
        }
        //5. Create the tetrahedron polyhedron.Remember that if the distance from the plane of the fourth point was negative, then the order of the first three vertices must be reversed.
        Plane plane;
        plane.Set(min_vertex, max_vertex, max_triangle);
        Real order = plane.PlaneTest(max_tetrahedron);
        if (order > 0.0f)
        {
            m_vertices->push_back(min_vertex);
            m_vertices->push_back(max_vertex);
            m_vertices->push_back(max_triangle);
            m_vertices->push_back(max_tetrahedron);
        }
        else
        {
            m_vertices->push_back(max_triangle);
            m_vertices->push_back(max_vertex);
            m_vertices->push_back(min_vertex);
            m_vertices->push_back(max_tetrahedron);
        }
        //6. Return the fact that we created a non - degenerate tetrahedron of dimension 3.
        m_faces->emplace_back(0, 1, 2);
        m_faces->emplace_back(1, 2, 3);
        m_faces->emplace_back(2, 3, 0);
        m_faces->emplace_back(3, 0, 1);
        return 3;
    }

    void ColliderPolyhedron::AddToOutsideSet(std::vector<Vector3>& vertices, OutsideSetFace& result) const
    {
        Vector3                                   face_a(m_vertices->at(result.a));
        Vector3                                   face_b(m_vertices->at(result.b));
        Vector3                                   face_c(m_vertices->at(result.c));
        Plane                                     plane(face_a, face_b, face_c);
        auto                                      begin = vertices.begin();
        auto                                      end   = vertices.end();
        std::list<std::vector<Vector3>::iterator> erase_list;
        //1. For each vertex in the listVertices
        for (auto it = begin; it != end; ++it)
        {
            //1. distance = Distance from the plane of the face to the vertex
            Real distance = plane.Distance(*it);
            //2. If the vertex is in the plane of the face(i.e.distance == 0)
            if (Math::IsZero(distance))
            {
                //3. Then check to see if the vertex is coincident with any vertices of the face, and if so merge it into that face vertex.
                if (Triangle::IsContainPoint(*it, face_a, face_b, face_c))
                {
                    //merge
                    erase_list.push_back(it);
                }
            }
                //4. Else If distance > 0
            else if (distance > 0.0f)
            {
                //5. Then claim the vertex by removing it from this list and adding it the face's set of outside vertices
                result.outside_vertices.push_back(*it);
                erase_list.push_back(it);
            }
            //6. Continue the loop
        }
        for (auto& it : erase_list)
        {
            vertices.erase(it);
        }
    }

    void ColliderPolyhedron::CalculateHorizon()
    {
        //If the currFace is not on the convex hull
        //Mark the crossedEdge as not on the convex hull and return.
        //If the currFace is visible from the eyePoint
        //Mark the currFace as not on the convex hull.
        //Remove all vertices from the currFace's outside set, and add them to the listUnclaimedVertices. c. 
        //If the crossedEdge != NULL (only for the first face) then mark the crossedEdge as not on the convex hull d. 
        //Cross each of the edges of currFace which are still on the convex hull 
        //in counterclockwise order starting from the edge after the crossedEdge 
        //(in the case of the first face, pick any edge to start with). 
        //For each currEdge recurse with the call.
    }
}
