#ifndef AA_MESH_TRANSFORM__H
#define AA_MESH_TRANSFORM__H

#include <AaMatrix>

namespace Aa
{
  namespace Mesh
  {

////////////////////////////////////////////////////////////////////////////////
// Aa::Mesh::TVertexTransform<V> ///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    template <class V>
    class TVertexTransform
    {
      private:
        mat4 m_transform;
        mat3 m_normal_matrix;

      public:
        inline
        TVertexTransform (const mat4 & m) :
          m_transform (m),
          m_normal_matrix (Aa::NormalMatrix (m))
        {
        }

        //inline
        V operator() (const V &) const;
    };

    template <>
    inline
    BasicVertex TVertexTransform<BasicVertex>::operator() (const BasicVertex & v) const
    {
      return BasicVertex (m_transform * vec4 (v.coords, 1.0f));
    }

    template <>
    inline
    NormalVertex TVertexTransform<NormalVertex>::operator() (const NormalVertex & v) const
    {
      return NormalVertex (m_transform * vec4 (v.coords, 1.0f),
                           (m_normal_matrix * v.normal).normalize ());
    }

////////////////////////////////////////////////////////////////////////////////
// Aa::Mesh::TMeshTransform<M> /////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    template <class M>
    class TMeshTransform
    {
      public:
        typedef /******/ M         Mesh;
        typedef typename M::Vertex Vertex;
        typedef typename M::Face   Face;

      private:
        TVertexTransform<Vertex> m_transform;

      public:
        inline
        TMeshTransform (const mat4 & m) :
          m_transform (m)
        {
        }

        inline
        void operator() (const M * input, M * output) const
        {
          const std::vector<Vertex> & v = input->vertices ();

          for (AaUInt i = 0; i < v.size (); ++i)
            output->addVertex (m_transform (v [i]));

          output->faces () = input->faces ();
        }

        inline
        M * operator() (const M * input) const
        {
          M * output = new M;
          this->operator() (input, output);
          return output;
        }
    };

  } // namespace Mesh
} // namespace Aa

#endif // AA_MESH_TRANSFORM__H

