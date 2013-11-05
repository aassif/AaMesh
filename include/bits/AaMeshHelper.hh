#ifndef AA_MESH_HELPER__H
#define AA_MESH_HELPER__H

#include <map>

namespace Aa
{
  namespace Mesh
  {

    template <typename K, class V>
    class TVertexGenerator
    {
      public:
        typedef K Key;
        typedef V Vertex;

      public:
        inline
        Vertex operator() (Key);
    };

    template <class M, class G>
    class TMeshHelper
    {
      public:
        typedef /******/ M            Mesh;
        typedef typename M::Vertex    Vertex;
        typedef typename M::Face      Face;

        typedef /******/ G            Generator;
        typedef typename G::Key       Key;

        typedef std::map<Key, AaUInt> Index;

      private:
        Mesh    * m_mesh;
        Generator m_generator;
        Index     m_index;

      public:
        inline
        TMeshHelper (Mesh * m, const Generator & g) :
          m_mesh (m),
          m_generator (g),
          m_index ()
        {
        }

        inline
        Mesh * mesh () {return m_mesh;}

        inline
        AaUInt addVertex (Key k)
        {
          typename Index::const_iterator i = m_index.find (k);
          if (i != m_index.end ()) return i->second;

          AaUInt id = m_mesh->addVertex (m_generator (k));
          m_index.insert (std::make_pair (k, id));
          return id;
        }

        inline
        AaUInt addFace (const Face & f)
        {
          return m_mesh->addFace (f);
        }
    };

  } // namespace Mesh
} // namespace Aa

#endif // AA_MESH_HELPER__H

