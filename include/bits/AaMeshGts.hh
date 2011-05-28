#ifndef __AA_MESH_GTS__
#define __AA_MESH_GTS__

#include <gts.h>
#include <map>
#include <AaMath>
#include "bits/AaMesh.hh"

namespace Aa
{
  namespace Mesh
  {
    template <class M>
    class GtsConverter
    {
    public:
      typedef typename M::Vertex   V;
      typedef typename M::Triangle T;
      typedef std::map<int, GtsVertex *> VertexMap;
      typedef std::pair<int, int> Edge;
      typedef std::map<Edge, GtsEdge *> EdgeMap;

      Edge make_edge (int a, int b)
      {
        return a < b ? Edge (a, b) : Edge (b, a);
      }

    private:
      const M * m_mesh;
      GtsSurface * m_surface;
      VertexMap m_vertices;
      EdgeMap m_edges;

      GtsVertex * addVertex (int id)
      {
        const V & v = m_mesh->vertex (id);
        return gts_vertex_new (gts_vertex_class (), v.coords[0], v.coords[1], v.coords[2]);
      }

      GtsVertex * registerVertex (int id)
      {
        //std::cout << "registerVertex: id = " << id << std::endl;
        VertexMap::iterator found = m_vertices.find (id);
        if (found != m_vertices.end ()) return found->second;

        GtsVertex * vertex = this->addVertex (id);
        m_vertices.insert (VertexMap::value_type (id, vertex));
        return vertex;
      }

      GtsEdge * addEdge (const Edge & e)
      {
        GtsVertex * v0 = registerVertex (e.first);
        GtsVertex * v1 = registerVertex (e.second);
        //std::cout << "addEdge: v0 = " << v0 << ", v1 = " << v1 << std::endl;
        return gts_edge_new (gts_edge_class (), v0, v1);
      }

      GtsEdge * registerEdge (const Edge & e)
      {
        EdgeMap::iterator found = m_edges.find (e);
        if (found == m_edges.end ())
        {
          GtsEdge * id = this->addEdge (e);
          found = m_edges.insert (EdgeMap::value_type (e, id)).first;
        }
        return found->second;
      }

      GtsEdge * registerEdge (int a, int b)
      {
        //std::cout << "registerEdge: a = " << a << ", b = " << b << std::endl;
        return registerEdge (make_edge (a, b));
      }

      GtsFace * addTriangle (int a, int b, int c)
      {
        GtsEdge * e0 = registerEdge (a, b);
        GtsEdge * e1 = registerEdge (b, c);
        GtsEdge * e2 = registerEdge (c, a);
        return gts_face_new (gts_face_class (), e0, e1, e2);
      }

      void registerTriangle (int a, int b, int c)
      {
        GtsFace * face = this->addTriangle (a, b, c);
        gts_surface_add_face (m_surface, face);
      }

      void build (GtsSurface * surface, const M * mesh)
      {
        // Start from scratch.
        m_mesh = mesh;
        m_surface = surface;

        // m_surface->clear ();
        m_vertices.clear ();
        m_edges.clear ();

        const std::vector<T> & triangles = mesh->triangles ();
        for (int i = 0; i < triangles.size (); ++i)
        {
          const T & t = triangles [i];
          this->registerTriangle (t.indices [0], t.indices [1], t.indices [2]);
        }
      }

    public:
      GtsConverter () :
        m_mesh (NULL),
        m_surface (NULL),
        m_vertices (),
        m_edges ()
      {
      }

      void operator() (GtsSurface * s, M * m)
      {
        this->build (s, m);
      }

      GtsSurface * operator() (const M * m)
      {
        GtsSurface * surface =
          gts_surface_new (gts_surface_class (),
                           gts_face_class (),
                           gts_edge_class (),
                           gts_vertex_class ());
        this->build (surface, m);
        return surface;
      }
    };

    template <class M>
    GtsSurface * gts_surface (const M * m)
    {
      return GtsConverter<M> () (m);
    }

    template <class M>
    GNode * gts_bb_tree (const M * m)
    {
      GtsSurface * surface = gts_surface (m);
      GNode * tree = gts_bb_tree_surface (surface);
      gts_object_destroy (GTS_OBJECT (surface));
      return tree;
    }

    bool gts_point_is_inside_mesh (const Math::pR3 & p, GNode * tree)
    {
      GtsPoint * point = gts_point_new (gts_point_class (), p.x, p.y, p.z);
      bool okay = gts_point_is_inside_surface (point, tree, FALSE);
      gts_object_destroy (GTS_OBJECT (point));
      return okay;
    }
    
    bool gts_point_is_inside_mesh (const Math::pR3 & p, GtsSurface * surface)
    {
      GNode * tree = gts_bb_tree_surface (surface);
      bool okay = gts_point_is_inside_mesh (p, tree);
      gts_object_destroy (GTS_OBJECT (tree));
      return okay;
    }
    
    template <class M>
    bool gts_point_is_inside_mesh (const Math::pR3 & p, const M * m)
    {
      GNode * tree = gts_bb_tree (m);
      bool okay = gts_point_is_inside_mesh (p, tree);
      gts_object_destroy (GTS_OBJECT (tree));
      return okay;
    }
    
    template <class M>
    float mesh_is_inside_mesh (const M * m1, const M * m2)
    {
      return 0.0;
    }
  }
}

#endif // __AA_MESH_GTS__

