#ifndef __AA_MESH__
#define __AA_MESH__

#include <vector>
#include <AaVector>
//#include <stdexcept>

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
        TMesh (int v = 0, int t = 0) {m_v.reserve (v); m_t.reserve (t);}

        // Destructor.
        virtual ~TMesh () {}

        // Clean-up.
        virtual void clear () {m_v.clear (); m_t.clear ();}

        // Vertices.
        const std::vector<Vertex> & vertices () const {return m_v;}
        /***/ std::vector<Vertex> & vertices () /***/ {return m_v;}

        // Access to a vertex.
        virtual const Vertex & vertex (int k) const {return m_v [k];}
        virtual /***/ Vertex & vertex (int k) /***/ {return m_v [k];}

        // Add a new vertex.
        virtual int addVertex (const Vertex & v) {int n = m_v.size (); m_v.push_back (v); return n;}

        // Triangles.
        const std::vector<Triangle> & triangles () const {return m_t;}
        /***/ std::vector<Triangle> & triangles () /***/ {return m_t;}

        // Access to a triangle.
        virtual const Triangle & triangle (int k) const {return m_t [k];}
        virtual /***/ Triangle & triangle (int k) /***/ {return m_t [k];}

        // Add a new triangle.
        virtual int addTriangle (const Triangle & t) {int n = m_t.size (); m_t.push_back (t); return n;}
    };

    /** BasicVertex */

    class BasicVertex
    {
      public:
        dvec3 coords;

      public:
        BasicVertex (const dvec3 & v) :
          coords (v)
        {
        }
        BasicVertex (double x, double y, double z) :
          coords (vec (x, y, z))
        {
        }
        virtual ~BasicVertex () {}
    };

    /** BasicTriangle */

    class BasicTriangle
    {
      public:
        ivec3 indices;

      public:
        BasicTriangle (const ivec3 & v) :
          indices (v)
        {
        }
        BasicTriangle (int a, int b, int c) :
          indices (vec (a, b, c))
        {
        }
        virtual ~BasicTriangle () {}
    };

    /** BasicMesh */

    typedef TMesh<BasicVertex, BasicTriangle> BasicMesh;
  } // namespace Mesh
} // namespace Aa

#endif // __AA_MESH__
