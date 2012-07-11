#ifndef AA_MESH_OPTIMIZER__H
#define AA_MESH_OPTIMIZER__H

#include <map>

namespace Aa
{

  namespace Mesh
  {

////////////////////////////////////////////////////////////////////////////////
// Aa::Mesh::MeshOptimizer<M> //////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    template <class M>
    class MeshOptimizer
    {
      public:
        typedef M                    Mesh;
        typedef typename M::Vertex   Vertex;
        typedef typename M::Triangle Triangle;

      private:
        M                      * m_input;
        M                      * m_output;
        std::map<AaUInt, AaUInt> m_index;

      public:
        MeshOptimizer (M * input, M * output) :
          m_input  (input),
          m_output (output),
          m_index  ()
        {
        }

        AaUInt addVertex (AaUInt k1)
        {
          std::map<AaUInt, AaUInt>::const_iterator found = m_index.find (k1);
          if (found == m_index.end ())
          {
            AaUInt k2 = m_output->addVertex (m_input->vertex (k1));
            found = m_index.insert (std::make_pair (k1, k2)).first;
          }
          return found->second;
        }

        AaUInt addTriangle (AaUInt k)
        {
          const Triangle & t = m_input->triangle (k);
          AaUInt i0 = this->addVertex (t.indices [0]);
          AaUInt i1 = this->addVertex (t.indices [1]);
          AaUInt i2 = this->addVertex (t.indices [2]);
          return m_output->addTriangle (Triangle (vec (i0, i1, i2)));
        }

        void addAll ()
        {
          const std::vector<Triangle> & triangles = m_input->triangles ();
          for (AaUInt i = 0; i < triangles.size (); ++i) this->addTriangle (i);
        }
    };

  } // namespace Mesh
} // namespace Aa

#endif // AA_MESH_OPTIMIZER__H

