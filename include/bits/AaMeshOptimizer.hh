#ifndef AA_MESH_OPTIMIZER__H
#define AA_MESH_OPTIMIZER__H

#include <map>

namespace Aa
{
  namespace Mesh
  {

////////////////////////////////////////////////////////////////////////////////
// Aa::Mesh::TMeshVertexCopy<M> ////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    template <class M>
    class TMeshVertexCopy
    {
      public:
        typedef typename M::Vertex Vertex;
        typedef /******/ AaUInt    Key;

      private:
        const M * m_mesh;

      public:
        inline
        TMeshVertexCopy (const M * m) :
          m_mesh (m)
        {
        }

        inline
        Vertex operator() (AaUInt k)
        {
          return m_mesh->vertex (k);
        }
    };

////////////////////////////////////////////////////////////////////////////////
// Aa::Mesh::TMeshOptimizer<M> /////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    template <class M>
    class TMeshOptimizer
    {
      public:
        typedef /******/ M         Mesh;
        typedef typename M::Vertex Vertex;
        typedef typename M::Face   Face;

        typedef TMeshVertexCopy<M> Generator;

        typedef TMeshHelper<Mesh, Generator> Helper;

      private:
        Helper m_helper;

      public:
        inline
        TMeshOptimizer (const M * input, M * output) :
          m_helper (output, Generator (input))
        {
        }

        inline
        AaUInt addFace (const Face &);

#if 0
        inline
        void addAll ()
        {
          const std::vector<Face> & faces = input->faces ();
          for (AaUInt i = 0; i < faces.size (); ++i) this->addFace (i);
        }
#endif
    };

    template <>
    inline
    AaUInt TMeshOptimizer<BasicMesh>::addFace (const BasicTriangle & t)
    {
      AaUInt i0 = m_helper.addVertex (t.indices [0]);
      AaUInt i1 = m_helper.addVertex (t.indices [1]);
      AaUInt i2 = m_helper.addVertex (t.indices [2]);
      return m_helper.addFace (BasicTriangle (vec (i0, i1, i2)));
    }

    template <>
    inline
    AaUInt TMeshOptimizer<NormalMesh>::addFace (const BasicTriangle & t)
    {
      AaUInt i0 = m_helper.addVertex (t.indices [0]);
      AaUInt i1 = m_helper.addVertex (t.indices [1]);
      AaUInt i2 = m_helper.addVertex (t.indices [2]);
      return m_helper.addFace (BasicTriangle (vec (i0, i1, i2)));
    }

  } // namespace Mesh
} // namespace Aa

#endif // AA_MESH_OPTIMIZER__H

