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

      public:
        inline
        TVertexTransform (const mat4 & m) :
          m_transform (m)
        {
        }

        inline
        V operator() (const V &);
    };

    template <>
    inline
    BasicVertex TVertexTransform<BasicVertex>::operator() (const BasicVertex & v)
    {
      return BasicVertex ((vec3) (m_transform * vec4 (v.coords, 1.0f)));
    }

    template <>
    inline
    NormalVertex TVertexTransform<NormalVertex>::operator() (const NormalVertex & v)
    {
      return NormalVertex ((vec3) (m_transform * vec4 (v.coords, 1.0f)),
                           (vec3) (m_transform * vec4 (v.normal, 0.0f)));
    }

  } // namespace Mesh
} // namespace Aa

#endif // AA_MESH_TRANSFORM__H

