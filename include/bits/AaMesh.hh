#ifndef AA_MESH__H
#define AA_MESH__H

#include <vector>
#include <AaVector>

namespace Aa 
{
  namespace Mesh
  {
    /** Mesh */

    template <class V, class T>
    class TMesh
    {
      public:
        typedef V Vertex;
        typedef T Triangle;

      protected:
        std::vector<Vertex>   m_v; // vertices.
        std::vector<Triangle> m_t; // triangles.

      public:
        // Constructor.
        inline
        TMesh (AaUInt v = 0, AaUInt t = 0)
        {
          m_v.reserve (v);
          m_t.reserve (t);
        }

        // Destructor.
        inline
        virtual ~TMesh ()
        {
        }

        // Clean-up.
        inline
        virtual void clear ()
        {
          m_v.clear ();
          m_t.clear ();
        }

        // Vertices.
        inline const std::vector<Vertex> & vertices () const {return m_v;}
        inline /***/ std::vector<Vertex> & vertices () /***/ {return m_v;}

        // Access to a vertex.
        inline const Vertex & vertex (AaUInt k) const {return m_v [k];}
        inline /***/ Vertex & vertex (AaUInt k) /***/ {return m_v [k];}

        // Add a new vertex.
        inline virtual AaUInt addVertex (const Vertex & v)
        {
          AaUInt n = m_v.size ();
          m_v.push_back (v);
          return n;
        }

        // Triangles.
        inline const std::vector<Triangle> & triangles () const {return m_t;}
        inline /***/ std::vector<Triangle> & triangles () /***/ {return m_t;}

        // Access to a triangle.
        inline const Triangle & triangle (AaUInt k) const {return m_t [k];}
        inline /***/ Triangle & triangle (AaUInt k) /***/ {return m_t [k];}

        // Add a new triangle.
        inline virtual AaUInt addTriangle (const Triangle & t)
        {
          AaUInt n = m_t.size ();
          m_t.push_back (t);
          return n;
        }
    };

    /** BasicVertex */

    class BasicVertex
    {
      public:
        dvec3 coords;

      public:
        inline
        BasicVertex (const dvec3 & v) :
          coords (v)
        {
        }
    };

    /** BasicTriangle */

    class BasicTriangle
    {
      public:
        ivec3 indices;

      public:
        inline
        BasicTriangle (const ivec3 & v) :
          indices (v)
        {
        }
    };

    /** BasicMesh */

    typedef TMesh<BasicVertex, BasicTriangle> BasicMesh;

    /** NormalVertex */

    class NormalVertex : public BasicVertex
    {
      public:
        dvec3 normal;

      public:
        inline
        NormalVertex (const dvec3 & p, const dvec3 & n) :
          BasicVertex (p),
          normal (n)
        {
        }
    };

    /** NormalMesh */

    typedef TMesh<NormalVertex, BasicTriangle> NormalMesh;


  } // namespace Mesh
} // namespace Aa

#endif // AA_MESH__H
