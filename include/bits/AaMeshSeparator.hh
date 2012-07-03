#ifndef __AASSIF_MESH_SEPARATOR__
#define __AASSIF_MESH_SEPARATOR__

#include <set>
#include <map>
#include <list>

#include <iostream>

#ifdef DEBUG
  #define ASSERT(x) assert (x)
#else
  #define ASSERT(x)
#endif

namespace Aa
{
  namespace Mesh
  {
    template <class M>
    class MeshSeparator
    {
    private:
      typedef typename M::Vertex   Vertex;
      typedef typename M::Triangle Triangle;
      typedef std::set<int> VertexSet;
      typedef std::map<int, VertexSet> AdjacencyMap;
      typedef enum {NOT_CONNECTED, CONNECTED, DONE} State;
      typedef std::vector<State> StateTable;
      typedef std::map<int, int> VertexMap;

    private:
      const M * m_mesh;
      AdjacencyMap m_adjVertices;
      StateTable m_states;
      VertexMap m_index;
      std::vector<M *> m_result;

      // Start point look-up.
      int findStart () const
      {
        // Look for the 1st not connected vertex.
        for (unsigned int i = 0; i < m_states.size (); ++i)
        {
          //std::cout << m_states [i] << std::endl;
          if (m_states [i] == NOT_CONNECTED) return i;
        }
        // If there is no such vertex, return -1.
        return -1;
      }

      // Connectivity detection.
      void setConnectivity (int x)
      {
#if 0
        m_states [x] = CONNECTED;

        const VertexSet &
          neighbourhood = m_adjVertices [x];

        for (VertexSet::const_iterator
               i = neighbourhood.begin (),
               e = neighbourhood.end   (); i != e;)
        {
          int neighbour = *(i++);
          if (m_states [neighbour] == NOT_CONNECTED)
          {
            //m_states [neighbour] = CONNECTED;
            this->setConnectivity (neighbour);
          }
        }
#else
        std::vector<int> q;
        q.reserve (m_states.size ());

        m_states [x] = CONNECTED;
        q.push_back (x);

        for (unsigned int i = 0; i < q.size (); ++i)
        {
          const VertexSet &
            neighbourhood = m_adjVertices [q[i]];

          for (VertexSet::const_iterator
                 i = neighbourhood.begin (),
                 e = neighbourhood.end   (); i != e;)
          {
            int neighbour = *(i++);
            if (m_states [neighbour] == NOT_CONNECTED)
            {
              m_states [neighbour] = CONNECTED;
              q.push_back (neighbour);
            }
          }
        }
#endif
      }

      // Registration of a vertex.
      int registerVertex (M * m, int src)
      {
        VertexMap::iterator found = m_index.find (src);
        if (found == m_index.end ())
        {
          int idx = m->addVertex (m_mesh->vertex (src));
          found = m_index.insert (m_index.end (), VertexMap::value_type (src, idx));
        }
        return found->second;
      }

      // Registration of a triangle.
      void registerTriangle (M * m, int a, int b, int c)
      {
        int ma = this->registerVertex (m, a);
        int mb = this->registerVertex (m, b);
        int mc = this->registerVertex (m, c);
        m->addTriangle (typename M::Triangle (vec (ma, mb, mc)));
      }

      // Connected component separation.
      void finishComponent ()
      {
        M * m = new M ();
        m_index.clear ();

        const std::vector<Vertex>   & vertices  = m_mesh->vertices ();
        const std::vector<Triangle> & triangles = m_mesh->triangles ();

        for (typename std::vector<Triangle>::const_iterator
               i = triangles.begin (), e = triangles.end (); i != e; ++i)
        {
          int
            a = i->indices [0],
            b = i->indices [1],
            c = i->indices [2];

          if (m_states [a] == CONNECTED)
          {
            ASSERT (m_states [b] == CONNECTED);
            ASSERT (m_states [c] == CONNECTED);
            this->registerTriangle (m, a, b, c);
          }
        }

        for (unsigned int i = 0; i < vertices.size (); ++i)
          if (m_states [i] == CONNECTED)
          {
            m_states [i] = DONE;
            m_adjVertices [i].clear ();
          }

        m_index.clear ();
        m_result.push_back (m);
      }

      // Mesh separation.
      std::vector<M *> separate (const M * mesh)
      {
        // Here comes a new challenger!
        m_mesh = mesh;

        // Initialization.
        const std::vector<Vertex>   & vertices  = m_mesh->vertices  ();
        const std::vector<Triangle> & triangles = m_mesh->triangles ();

        m_states.clear ();
        m_states.resize (vertices.size (), NOT_CONNECTED);

        for (unsigned int i = 0; i < triangles.size (); ++i)
        {
          const Triangle & t = triangles [i];
          int a = t.indices [0];
          int b = t.indices [1];
          int c = t.indices [2];
          m_adjVertices [a].insert (b); m_adjVertices [a].insert (c);
          m_adjVertices [b].insert (a); m_adjVertices [b].insert (c);
          m_adjVertices [c].insert (a); m_adjVertices [c].insert (b);
        }

        // Let's go!
        for (int start = this->findStart (); start != -1; start = this->findStart ())
        {
          //std::cout << "start = " << start << std::endl;
          this->setConnectivity (start);
          this->finishComponent ();
        }

        // Free memory.
        m_mesh = NULL;
        m_adjVertices.clear ();
        m_states.clear ();
        // Pfff !
        std::vector<M *> r;
        r.swap (m_result);
        return r;
      }

    public:
      // Constructor.
      MeshSeparator () :
        m_mesh (NULL),
        m_adjVertices (),
        m_states (),
        m_index (),
        m_result ()
      {
      }

      // Main method.
      std::vector<M *> operator() (const M * mesh)
      {
        return this->separate (mesh);
      }
    }; // class MeshSeparator
  } // namespace msh
} // namespace aassif

#endif // __AASSIF_MESH_SEPARATOR__
