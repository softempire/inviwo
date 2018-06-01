/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2013-2018 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *********************************************************************************/

#ifndef IVW_TYPEDMESH_H
#define IVW_TYPEDMESH_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>

#include <inviwo/core/datastructures/geometry/geometrytype.h>
#include <inviwo/core/datastructures/geometry/mesh.h>
#include <inviwo/core/datastructures/buffer/buffer.h>
#include <inviwo/core/datastructures/buffer/bufferram.h>
#include <inviwo/core/datastructures/buffer/bufferramprecision.h>

#include <tuple>

namespace inviwo {

/**
 * \defgroup typedmesh TypedMesh
 * \ingroup datastructures
 *
 * \copydetails TypedMesh
 */

/**
 * Namespace for buffer traits
 * \ingroup typedmesh
 */
namespace buffertraits {
/**
 * \ingroup typedmesh
 * Based class used to decorate TypedMesh with buffers. Handles creation of buffers and adds it to
 * the mesh. Also has convince functions to access the buffers and its data containers.
 * **Note on Template parameters**
 * The first two template parameters, `typename T, unsigned DIM` is used instead of
 * `Vector<DIM,T>` or glm::TvecX, this is due to compiler issues related to the clone function.
 * Similarly, The template parameter `int attrib` can't be a strongly typed enum
 * (inviwo::BufferType) and therefor it is an int.
 *
 * \see TypedMesh
 * \see typedmesh
 */
template <typename T, unsigned DIM, int attrib, int location = attrib>
class TypedMeshBufferBase {
public:
    using type = Vector<DIM, T>;
    static inviwo::Mesh::BufferInfo bi() {
        return {static_cast<inviwo::BufferType>(attrib), location};
    }

#if defined(_MSC_VER) && _MSC_VER <= 1900
    TypedMeshBufferBase(type t) : t(t) {}
    type t;
    operator type() const { return t; }
#endif

    TypedMeshBufferBase() {}
    virtual ~TypedMeshBufferBase() = default;

    TypedMeshBufferBase(Mesh &mesh) { mesh.addBuffer(bi(), buffer_); }

    // Convenience functions
    auto getTypedRAMRepresentation() const { return buffer_->getRAMRepresentation(); }
    auto getTypedEditableRAMRepresentation() { return buffer_->getEditableRAMRepresentation(); }
    auto &getTypedDataContainer() const { return getTypedRAMRepresentation()->getDataContainer(); }
    auto &getTypedDataContainer() {
        return getTypedEditableRAMRepresentation()->getDataContainer();
    }

    std::shared_ptr<Buffer<type>> buffer_{std::make_shared<Buffer<type>>()};
};

/**
 * \ingroup typedmesh
 * BufferTrait for Position buffers (glm::vec3)
 */
class PositionsBuffer
    : public TypedMeshBufferBase<float, 3, static_cast<int>(BufferType::PositionAttrib)> {
public:
    using Base = TypedMeshBufferBase<float, 3, static_cast<int>(BufferType::PositionAttrib)>;
    using Base::Base;

    std::shared_ptr<const Buffer<Base::type>> getVertices() const { return Base::buffer_; }
    std::shared_ptr<Buffer<Base::type>> getEditableVertices() { return Base::buffer_; }

    void setVertexPosition(size_t index, vec3 pos) {
        getEditableVertices()->getEditableRAMRepresentation()->set(index, pos);
    }
};

/**
 * \ingroup typedmesh
 * BufferTrait for Normal buffers (glm::vec3)
 */
class NormalBuffer
    : public TypedMeshBufferBase<float, 3, static_cast<int>(BufferType::NormalAttrib)> {
public:
    using Base = TypedMeshBufferBase<float, 3, static_cast<int>(BufferType::NormalAttrib)>;
    using Base::Base;

    std::shared_ptr<const Buffer<Base::type>> getNormals() const { return Base::buffer_; }
    std::shared_ptr<Buffer<Base::type>> getEditableNormals() { return Base::buffer_; }

    void setVertexNormal(size_t index, vec3 normal) {
        getEditableNormals()->getEditableRAMRepresentation()->set(index, normal);
    }
};

/**
 * \ingroup typedmesh
 * BufferTrait for Colors buffers (glm::vec4)
 */
class ColorsBuffer
    : public TypedMeshBufferBase<float, 4, static_cast<int>(BufferType::ColorAttrib)> {
public:
    using Base = TypedMeshBufferBase<float, 4, static_cast<int>(BufferType::ColorAttrib)>;
    using Base::Base;

    std::shared_ptr<const Buffer<Base::type>> getColors() const { return Base::buffer_; }
    std::shared_ptr<Buffer<Base::type>> getEditableColors() { return Base::buffer_; }

    void setVertexColor(size_t index, vec4 color) {
        getEditableColors()->getEditableRAMRepresentation()->set(index, color);
    }
};

/**
 * \ingroup typedmesh
 * BufferTrait for Texture Coordinate buffers
 */
template <unsigned DIMS = 3>
class TexcoordBuffer
    : public TypedMeshBufferBase<float, DIMS, static_cast<int>(BufferType::TexcoordAttrib)> {
public:
    using Base = TypedMeshBufferBase<float, DIMS, static_cast<int>(BufferType::TexcoordAttrib)>;
    using Base::Base;

    std::shared_ptr<const Buffer<typename Base::type>> getTexCoords() const {
        return Base::buffer_;
    }
    std::shared_ptr<Buffer<typename Base::type>> getEditableTexCoords() { return Base::buffer_; }

    void setVertexTexCoord(size_t index, typename Base::type texCoord) {
        getEditableTexCoords()->getEditableRAMRepresentation()->set(index, texCoord);
    }
};

/**
 * \ingroup typedmesh
 * BufferTrait for Curvature buffers (float)
 */
class CurvatureBuffer
    : public TypedMeshBufferBase<float, 1, static_cast<int>(BufferType::CurvatureAttrib)> {
public:
    using Base = TypedMeshBufferBase<float, 1, static_cast<int>(BufferType::CurvatureAttrib)>;
    using Base::Base;

    std::shared_ptr<const Buffer<Base::type>> getCurvatures() const { return Base::buffer_; }
    std::shared_ptr<Buffer<Base::type>> getEditableCurvatures() { return Base::buffer_; }

    void setVertexCurvature(size_t index, float curvature) {
        getEditableCurvatures()->getEditableRAMRepresentation()->set(index, curvature);
    }
};

/**
 * \ingroup typedmesh
 * BufferTrait for Uint32 buffers
 */
using IndexBuffer = TypedMeshBufferBase<uint32_t, 1, static_cast<int>(BufferType::IndexAttrib)>;

/**
 * \ingroup typedmesh
 * BufferTrait for radii buffers (float)
 * \see SphereMesh
 */
class RadiiBuffer
    : public TypedMeshBufferBase<float, 1, static_cast<int>(BufferType::NumberOfBufferTypes)> {
public:
    using Base = TypedMeshBufferBase<float, 1, static_cast<int>(BufferType::NumberOfBufferTypes)>;
    using Base::Base;

    std::shared_ptr<const Buffer<Base::type>> getRadii() const { return Base::buffer_; }
    std::shared_ptr<Buffer<Base::type>> getEditableRadii() { return Base::buffer_; }

    void setVertexRadius(size_t index, float radius) {
        getEditableRadii()->getEditableRAMRepresentation()->set(index, radius);
    }
};
}  // namespace buffertraits

/**
 * \class TypedMesh
 * \ingroup datastructures
 * \ingroup typedmesh
 *
 *
 * TypedMesh is a templated data structure for creating meshes with a custom amount of vertex
 * buffers. It uses a variadic set of BufferTraits to define its interface. For example, a Mesh with
 * a position and color per vertex would be defined as
 * `TypedMesh<PositionsBufferTrait,ColorsBufferTrait>`. Depending on the Traits specified in the
 * declaration the interface towards the class is updated.
 *
 * # Simple Usage
 *
 * \code{.cpp}
 * using MyMesh = TypedMesh<buffertraits::PositionsBuffer,buffertraits::ColorsBuffer>;
 * MyMesh mesh;
 * mesh.addVertex(vec3(0.0f), vec4(1,0,0,1) );
 * mesh.addVertex(vec3(1.0f), vec4(0,1,0,1) );
 * \endcode
 *
 * If texture coordinates are also needed for each vertex in the mesh then one could instead use:
 *
 * \code{.cpp}
 * using MyMesh = TypedMesh<buffertraits::PositionsBuffer, buffertraits::TexcoordBuffer,
 * buffertraits::ColorsBuffer>;
 * MyMesh mesh;
 * mesh.addVertex(vec3(0.0f), vec3(0.0f), vec4(1,0,0,1) );
 * mesh.addVertex(vec3(1.0f), vec3(1.0f), vec4(0,1,0,1) );
 * \endcode
 *
 * For meshes with more than a couple of vertices, it is more efficient to add all vertices to the
 * mesh at the same time. This can be done by using a std::vector containing
 * TypedMesh::Vertex instead, as described by the following example:
 *
 * \code{.cpp}
 * using MyMesh = TypedMesh<PositionsBufferTrait,ColorsBufferTrait>;
 * std::vector<MyMesh::Vertex> vertices;
 * vertices.emplace_back(vec3(0.0f), vec4(1,0,0,1));
 * vertices.emplace_back(vec3(1.0f), vec4(0,1,0,1));
 * MyMesh mesh;
 * mesh.addVertices(vertices);
 * \endcode
 *
 * When creating meshes it is very common to also have Index buffers, in addition to the vertex
 * buffers. To add a index buffer to the mesh you can use the function addIndexBuffer as
 * demonstrated by the following example.
 *
 * \code{.cpp}
 * using MyMesh = TypedMesh<buffertraits::PositionsBuffer, buffertraits::ColorsBuffer>;
 * MyMesh mesh;
 * // Add vertices as above
 * auto ib = mesh.addIndexBuffer(DrawType::Lines, ConnectivityType::None);
 * ib->add({0,1}); // Create a line between vertex 0 and 1
 * ib->add({1,2}); // Create another line between vertex 1 and 2
 * \endcode
 *
 * # Examples
 * ## Creating a bounding box with Adjacency information
 * The following code snippet uses a SimpleMesh2 to create bounding box for a given basisandoffset
 * matrix. It is the code used in meshutil::boundingBoxAdjacency
 *
 * \snippet modules/base/algorithm/meshutils.cpp Using PosTexColorMesh
 *
 *
 * ## Creating camera frustum
 * The following code snippet is another example where we create a camera frustum mesh for a given
 * camera. It is the code used in meshutil::cameraFrustum
 *
 * \snippet modules/base/algorithm/meshutils.cpp Using Colored Mesh
 *
 */
template <typename... BufferTraits>
class TypedMesh : public Mesh, public BufferTraits... {
public:
#if defined(_MSC_VER) && _MSC_VER <= 1900
    // On visual studio 2015 Alias templates is not supported
    // (https://blogs.msdn.microsoft.com/vcblog/2016/06/07/expression-sfinae-improvements-in-vs-2015-update-3/)
    using Vertex = std::tuple<BufferTraits...>;
#else
    template <typename T>
    using TypeAlias = typename T::type;
    using VertexTuple = std::tuple<TypeAlias<BufferTraits>...>;

#if defined(_MSC_VER) && _MSC_VER > 1900
    using Vertex = VertexTuple;
#else
    // On GCC 5.4 Vertex x = {...} does not compile. Used when for example creating a vector of
    // vertices from initializer lists.
    class Vertex : public VertexTuple {
    public:
        Vertex() = default;
        Vertex(TypeAlias<BufferTraits>... vals) : VertexTuple(vals...) {}
    };
#endif
#endif

    TypedMesh(DrawType dt = DrawType::Points, ConnectivityType ct = ConnectivityType::None)
        : Mesh(dt, ct), BufferTraits(*static_cast<Mesh *>(this))... {}

    TypedMesh(const TypedMesh &rhs) : Mesh(rhs) { copyConstrHelper<0, BufferTraits...>(); }
    TypedMesh &operator=(const TypedMesh &that) {
        if (this != &that) {
            Mesh::operator=(that);
            copyConstrHelper<0, BufferTraits...>();
        }
        return *this;
    }

    virtual TypedMesh *clone() const override { return new TypedMesh(*this); }

    virtual ~TypedMesh() = default;

    /**
     * \brief Append another TypedMesh to the current TypedMesh
     *
     * @see Mesh::append
     * @param mesh The mesh to append
     */
    void append(const TypedMesh *mesh) {
        if (mesh) Mesh::append(*mesh);
    }

    /**
     * \brief Adds a vector of vertices to the mesh
     *
     * This method is often faster than adding the vertices one at a time with
     * TypedMesh::addVertex()
     */
    void addVertices(const std::vector<Vertex> &vertices) {
        addVerticesImpl<0, BufferTraits...>(vertices);
    }

    uint32_t addVertex(const Vertex &vertex) {
        using BT = typename std::tuple_element<0, std::tuple<BufferTraits...>>::type;
        addVertexImplVertex<0>(vertex,
                               std::integral_constant<bool, 0 < std::tuple_size<Vertex>::value>());

        return static_cast<uint32_t>(getTypedBuffer<BT>()->getSize() - 1);
    }

    void setVertex(size_t index, const Vertex &vertex) {
        setVertexImplVertex<0>(index, vertex,
                               std::integral_constant<bool, 0 < std::tuple_size<Vertex>::value>());
    }

#if defined(_MSC_VER) && _MSC_VER <= 1900
    // On visual studio 2015 Alias templates is not supported
    // (https://blogs.msdn.microsoft.com/vcblog/2016/06/07/expression-sfinae-improvements-in-vs-2015-update-3/)
    uint32_t addVertex(BufferTraits... args) {
        using BT = typename std::tuple_element<0, std::tuple<BufferTraits...>>::type;
        addVertexImpl<0>(args...);
        return static_cast<uint32_t>(getTypedBuffer<BT>()->getSize() - 1);
    }

    void setVertex(size_t index, BufferTraits... args) { setVertexImpl<0>(index, args...); }
#else
    /**
     * \brief Adds a vertex
     *
     * Adds a vertex to the mesh. The arguments to the function are automatically determined by the
     * buffers of the mesh. For example:
     * \code{.cpp}
     * using MyMesh = TypedMesh<buffertraits::PositionsBuffer,buffertraits::ColorsBuffer>;
     * MyMesh mesh;
     * mesh.addVertex(vec3(0.0f), vec4(1,0,0,1) );
     * mesh.addVertex(vec3(1.0f), vec4(0,1,0,1) );
     * \endcode
     *
     * @param args the arguments, needs to match the buffers of the mesh
     * @return uint32_t the position of the new vertex is the buffers.
     */
    uint32_t addVertex(TypeAlias<BufferTraits>... args) {
        using BT = typename std::tuple_element<0, std::tuple<BufferTraits...>>::type;
        addVertexImpl<0>(args...);
        return static_cast<uint32_t>(getTypedBuffer<BT>()->getSize() - 1);
    }

    /**
     * \brief Sets a specific vertex.
     *
     * The arguments to the function are automatically determined by the buffers of the mesh.
     *
     * @see addVertex
     * @param index vertex index to update
     * @param args the arguments, needs to match the buffers of the mesh
     */
    void setVertex(size_t index, TypeAlias<BufferTraits>... args) {
        setVertexImpl<0>(index, args...);
    }
#endif

    /**
     * \brief Updates the a specific value in specific buffer
     *
     * The following example will set the color of vertex with index 5 to read:
     * \code{.cpp}
     * using MyMesh = TypedMesh<buffertraits::PositionsBuffer,buffertraits::ColorsBuffer>;
     * MyMesh mesh;
     * // ... add some vertices
     * mesh.setVertex<buffertraits::ColorsBuffer>(5 , vec4(1,0,0,1) );
     * \endcode
     *
     * @param index vertex index to update
     * @param v the new value
     */
    template <typename BT>
    void setVertex(size_t index, const typename BT::type &v) {
        getTypedDataContainer<BT>().at(index) = v;
    }

    /**
     * \brief Returns a specific buffer.
     * The following example shows how to get the color buffer:
     * \code{.cpp}
     * using MyMesh = TypedMesh<buffertraits::PositionsBuffer,buffertraits::ColorsBuffer>;
     * MyMesh mesh;
     * auto colorBuffer = mesh.getTypedBuffer<buffertraits::ColorsBuffer>();
     * \endcode
     */
    template <typename BT>
    auto getTypedBuffer() {
        return BT::buffer_;
    }

    /**
     * Returns a specific buffer
     * @see getTypedBuffer()
     */
    template <typename BT>
    auto getTypedBuffer() const {
        return BT::buffer_;
    }

    /**
     * Returns the RAM Representation of a specific buffer.
     * @see getTypedBuffer()
     */
    template <typename BT>
    auto getTypedRAMRepresentation() const {
        return BT::getTypedRAMRepresentation();
    }

    /**
     * Returns the Editable RAM Representation of a specific buffer.
     * @see getTypedBuffer()
     */
    template <typename BT>
    auto getTypedEditableRAMRepresentation() {
        return BT::getTypedEditableRAMRepresentation();
    }

    /**
     * Returns the Data Container of a specific buffer.
     * @see getTypedBuffer()
     */
    template <typename BT>
    auto &getTypedDataContainer() const {
        return BT::getTypedDataContainer();
    }

    /**
     * Returns the Data Container of a specific buffer.
     * @see getTypedBuffer()
     */
    template <typename BT>
    auto &getTypedDataContainer() {
        return BT::getTypedDataContainer();
    }

private:
    template <unsigned I, typename T>
    void addVertexImpl(T &t) {
        using BT = typename std::tuple_element<I, std::tuple<BufferTraits...>>::type;
        BT::getTypedEditableRAMRepresentation()->add(t);
    }

    template <unsigned I, typename T, typename... ARGS>
    void addVertexImpl(T &t, ARGS... args) {
        using BT = typename std::tuple_element<I, std::tuple<BufferTraits...>>::type;
        BT::getTypedEditableRAMRepresentation()->add(t);
        addVertexImpl<I + 1>(args...);
    }

    template <unsigned I>
    void addVertexImplVertex(const Vertex &, std::false_type) {}

    template <unsigned I>
    void addVertexImplVertex(const Vertex &v, std::true_type) {
        using BT = typename std::tuple_element<I, std::tuple<BufferTraits...>>::type;
        BT::getTypedEditableRAMRepresentation()->add(std::get<I>(v));
        addVertexImplVertex<I + 1>(
            v, std::integral_constant<bool, I + 1 < std::tuple_size<Vertex>::value>());
    }

    template <unsigned I, typename T>
    void setVertexImpl(size_t index, T &t) {
        using BT = typename std::tuple_element<I, std::tuple<BufferTraits...>>::type;
        BT::getTypedDataContainer().at(index) = t;
    }

    template <unsigned I, typename T, typename... ARGS>
    void setVertexImpl(size_t index, T &t, ARGS... args) {
        using BT = typename std::tuple_element<I, std::tuple<BufferTraits...>>::type;
        BT::getTypedDataContainer().at(index) = t;
        setVertexImpl<I + 1>(index, args...);
    }

    template <unsigned I>
    void setVertexImplVertex(size_t, const Vertex &, std::false_type) {}

    template <unsigned I>
    void setVertexImplVertex(size_t index, const Vertex &v, std::true_type) {
        using BT = typename std::tuple_element<I, std::tuple<BufferTraits...>>::type;
        BT::getTypedDataContainer().at(index) = std::get<I>(v);
        setVertexImplVertex<I + 1>(
            index, v, std::integral_constant<bool, I + 1 < std::tuple_size<Vertex>::value>());
    }

    template <unsigned I>
    void addVerticesImpl(const std::vector<Vertex> &) {}  // sink

    template <unsigned I, typename T, typename... ARGS>
    void addVerticesImpl(const std::vector<Vertex> &vertices) {

        auto &vec = getTypedDataContainer<T>();

        auto neededSize = vertices.size() + vec.size();
        if (vec.capacity() < neededSize) {
            vec.reserve(neededSize);
        }
        for (auto &v : vertices) {
            vec.push_back(std::get<I>(v));
        }

        addVerticesImpl<I + 1, ARGS...>(vertices);
    }

    template <unsigned I>
    void copyConstrHelper() {}  // sink

    template <unsigned I, typename T, typename... ARGS>
    void copyConstrHelper() {
        T::buffer_ = std::static_pointer_cast<Buffer<typename T::type>>(Mesh::buffers_[I].second);
        copyConstrHelper<I + 1, ARGS...>();
    }
};

/**
 * \ingroup typedmesh
 * Type definition of a TypedMesh useful for Spheres, consists of a vec3-buffer for position, a
 * float-buffer for radii and vec4 for colors.
 */
using SphereMesh =
    TypedMesh<buffertraits::PositionsBuffer, buffertraits::RadiiBuffer, buffertraits::ColorsBuffer>;

/**
 * \ingroup typedmesh
 * Type definition of a TypedMesh having only positions(vec3) and colors(vec4).
 * Example usage:
 * \snippet modules/base/algorithm/meshutils.cpp Using Colored Mesh
 */
using ColoredMesh = TypedMesh<buffertraits::PositionsBuffer, buffertraits::ColorsBuffer>;

/**
 * \ingroup typedmesh
 * Type definition of a TypedMesh having positions(vec3), normals(vec3), texture
 * coordinates(vec3) and colors(vec4).
 */
using BasicMesh = TypedMesh<buffertraits::PositionsBuffer, buffertraits::NormalBuffer,
                            buffertraits::TexcoordBuffer<3>, buffertraits::ColorsBuffer>;

/**
 * \ingroup typedmesh
 * Type definition of a TypedMesh having positions(vec3), texture
 * coordinates(vec3) and colors(vec4). Example usage:
 * \snippet modules/base/algorithm/meshutils.cpp Using PosTexColorMesh
 */
using PosTexColorMesh = TypedMesh<buffertraits::PositionsBuffer, buffertraits::TexcoordBuffer<3>,
                                  buffertraits::ColorsBuffer>;

}  // namespace inviwo

#endif  // IVW_TYPEDMESH_H
