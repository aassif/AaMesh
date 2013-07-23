#ifndef AA_MESH__H
#define AA_MESH__H

#include <vector>
#include <AaVector>

namespace Aa 
{
  namespace Mesh
  {
    /** Mesh */

    template <class V, class F>
    class TMesh
    {
      public:
        typedef V Vertex;
        typedef F Face;

      protected:
        std::vector<Vertex> m_v; // vertices.
        std::vector<Face>   m_f; // faces.

      public:
        // Constructor.
        inline
        TMesh (AaUInt v = 0, AaUInt f = 0)
        {
          m_v.reserve (v);
          m_f.reserve (f);
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
          m_f.clear ();
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

        // Faces.
        inline const std::vector<Face> & faces () const {return m_f;}
        inline /***/ std::vector<Face> & faces () /***/ {return m_f;}

        // Access to a face.
        inline const Face & face (AaUInt k) const {return m_f [k];}
        inline /***/ Face & face (AaUInt k) /***/ {return m_f [k];}

        // Add a new face.
        inline virtual AaUInt addFace (const Face & f)
        {
          AaUInt n = m_f.size ();
          m_f.push_back (f);
          return n;
        }
    };

    /** BasicVertex */

    class BasicVertex
    {
      public:
        vec3 coords;

      public:
        inline
        BasicVertex (const vec3 & v) :
          coords (v)
        {
        }
    };

    /** BasicTriangle */

    class BasicTriangle
    {
      public:
        uvec3 indices;

      public:
        inline
        BasicTriangle (const uvec3 & v) :
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
        vec3 normal;

      public:
        inline
        NormalVertex (const vec3 & p, const vec3 & n = vec3 ()) :
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
