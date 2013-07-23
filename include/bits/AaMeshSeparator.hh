#ifndef AA_MESH_SEPARATOR__H
#define AA_MESH_SEPARATOR__H

#include <set>
#include <map>
#include <list>

namespace Aa
{
  namespace Mesh
  {

    class MeshConnectivity
    {
      public:
        typedef std::set<AaUInt> VertexSet;
        typedef std::map<AaUInt, VertexSet> AdjacencyMap;

        typedef enum {READY = 0, CONNECTED = 1, DONE = 2} State;

      protected:
        AdjacencyMap       m_adjacency;
        std::vector<State> m_states;

      protected:
        template <class Face>
        inline
        void init (const Face &);

      public:
        template <class M>
        inline
        MeshConnectivity (const M * m) :
          m_adjacency (),
          m_states (m->vertices().size(), READY)
        {
          for (AaUInt i = 0; i < m->faces().size(); ++i)
            this->init (m->face (i));
        }

        inline
        void flood (AaUInt k1)
        {
          std::vector<AaUInt> q;
          q.reserve (m_states.size ());

          m_states [k1] = CONNECTED;
          q.push_back (k1);

          for (AaUInt j = 0; j < q.size (); ++j)
          {
            const VertexSet & v = m_adjacency [q[j]];
            for (VertexSet::const_iterator i = v.begin (), e = v.end (); i != e;)
            {
              AaUInt k2 = *(i++);
              if (m_states [k2] == READY)
              {
                m_states [k2] = CONNECTED;
                q.push_back (k2);
              }
            }
          }
        }

        inline
        bool vertex_ready (AaUInt k) const
        {
          return (m_states [k] == READY);
        }

        template <class F>
        inline
        bool face_connected (const F &) const;

        inline
        void finish ()
        {
          for (AaUInt i = 0; i < m_states.size (); ++i)
            if (m_states [i] == CONNECTED)
              m_states [i] = DONE;
        }
    };

    template <>
    inline
    void MeshConnectivity::init (const BasicTriangle & t)
    {
      AaUInt a = t.indices [0];
      AaUInt b = t.indices [1];
      AaUInt c = t.indices [2];

      m_adjacency [a].insert (b); m_adjacency [b].insert (c); m_adjacency [c].insert (a);
      m_adjacency [a].insert (c); m_adjacency [c].insert (b); m_adjacency [b].insert (a); 
    }

    template <>
    inline
    bool MeshConnectivity::face_connected (const BasicTriangle & t) const
    {
      AaUInt a = t.indices [0];
      return (m_states [a] == CONNECTED);
    }

    template <class M>
    class TMeshSeparator
    {
      private:
        typedef /******/ M         Mesh;
        typedef typename M::Vertex Vertex;
        typedef typename M::Face   Face;

      private:
        std::vector<Mesh *> separate (const Mesh * mesh) const
        {
          std::vector<Mesh *> v;

          const std::vector<Vertex> & vertices = mesh->vertices ();
          const std::vector<Face>   & faces    = mesh->faces    ();

          MeshConnectivity connectivity (mesh);

          for (AaUInt i = 0; i < vertices.size (); ++i)
          {
            if (connectivity.vertex_ready (i))
            {
              Mesh * m = new Mesh;

              TMeshOptimizer<M> optimizer (mesh, m);

              connectivity.flood (i);
              for (AaUInt i = 0; i < faces.size (); ++i)
              {
                const Face & f = faces [i];
                if (connectivity.face_connected (f))
                  optimizer.addFace (f);
              }
              connectivity.finish ();

              v.push_back (m);
            }
          }

          return v;
        }

      public:
        std::vector<Mesh *> operator() (const Mesh * mesh) const
        {
          return this->separate (mesh);
        }
    };

  } // namespace msh
} // namespace aassif

#endif // AA_MESH_SEPARATOR__H

