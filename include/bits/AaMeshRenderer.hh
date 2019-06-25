#ifndef AA_MESH_RENDERER__H
#define AA_MESH_RENDERER__H

#include <AaGL>

namespace Aa 
{
  namespace Mesh
  {

////////////////////////////////////////////////////////////////////////////////
// Aa::Mesh::TVertexRenderer<V> ////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    template <class V>
    class TVertexRenderer
    {
      public:
        typedef V Vertex;

      public:
        static inline
        void SetPointers (const Vertex *);

        static inline
        void SetPointers (const std::vector<Vertex> & v)
        {
          if (! v.empty ())
            SetPointers (&(v[0]));
        }

        static inline
        void Disable ();
    };

////////////////////////////////////////////////////////////////////////////////
// Aa::Mesh::BasicVertexRenderer ///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    typedef TVertexRenderer<BasicVertex> BasicVertexRenderer;

    template <>
    inline
    void BasicVertexRenderer::SetPointers (const BasicVertex * p)
    {
#if 0
      glEnableClientState (GL_VERTEX_ARRAY);
      glVertexPointer (3, GL_FLOAT, sizeof (BasicVertex), &(p->coords));
#else
      glEnableVertexAttribArray (0);
      glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, sizeof (BasicVertex), &(p->coords));
#endif
    }

    template <>
    inline
    void BasicVertexRenderer::Disable ()
    {
#if 0
      glDisableClientState (GL_VERTEX_ARRAY);
#else
      glDisableVertexAttribArray (0);
#endif
    }

////////////////////////////////////////////////////////////////////////////////
// Aa::Mesh::NormalVertexRenderer //////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    typedef TVertexRenderer<NormalVertex> NormalVertexRenderer;

    template <>
    inline
    void NormalVertexRenderer::SetPointers (const NormalVertex * p)
    {
#if 0
      glEnableClientState (GL_VERTEX_ARRAY);
      glVertexPointer (3, GL_FLOAT, sizeof (NormalVertex), &(p->coords));

      glEnableClientState (GL_NORMAL_ARRAY);
      glNormalPointer    (GL_FLOAT, sizeof (NormalVertex), &(p->normal));
#else
      glEnableVertexAttribArray (0);
      glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, sizeof (NormalVertex), &(p->coords));

      glEnableVertexAttribArray (1);
      glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, sizeof (NormalVertex), &(p->normal));
#endif
    }

    template <>
    inline
    void NormalVertexRenderer::Disable ()
    {
#if 0
      glDisableClientState (GL_VERTEX_ARRAY);
      glDisableClientState (GL_NORMAL_ARRAY);
#else
      glDisableVertexAttribArray (0);
      glDisableVertexAttribArray (1);
#endif
    }

////////////////////////////////////////////////////////////////////////////////
// Aa::Mesh::TFaceRenderer<F> //////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    template <class F>
    class TFaceRenderer
    {
      public:
        typedef F Face;

      public:
        static inline
        void DrawElements (GLuint *, GLuint count);

        static inline
        void DrawElements (const std::vector<Face> &);
    };

////////////////////////////////////////////////////////////////////////////////
// Aa::Mesh::BasicTriangleRenderer /////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    typedef TFaceRenderer<BasicTriangle> BasicTriangleRenderer;

    template <>
    inline
    void BasicTriangleRenderer::DrawElements (GLuint * p, GLuint count)
    {
      glDrawElements (GL_TRIANGLES, count, GL_UNSIGNED_INT, p);
    }

    template <>
    inline
    void BasicTriangleRenderer::DrawElements (const std::vector<BasicTriangle> & v)
    {
      if (! v.empty ())
      {
        std::vector<GLuint> indices (3 * v.size ());
        for (AaUInt i = 0; i < v.size (); ++i)
        {
          indices [3*i + 0] = v[i].indices[0];
          indices [3*i + 1] = v[i].indices[1];
          indices [3*i + 2] = v[i].indices[2];
        }

        BasicTriangleRenderer::DrawElements (&(indices[0]), indices.size ());
      }
    }

////////////////////////////////////////////////////////////////////////////////
// Aa::Mesh::TMeshRenderer<M> //////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    template <class M>
    class TMeshRenderer
    {
      public:
        typedef /******/ M         Mesh;
        typedef typename M::Vertex Vertex;
        typedef typename M::Face   Face;

      protected:
        Mesh * m_mesh;

      public:
        inline
        TMeshRenderer (Mesh * m = NULL) :
          m_mesh (m)
        {
        }

        inline
        virtual void draw ()
        {
          if (m_mesh != NULL)
          {
            TVertexRenderer<Vertex>::SetPointers (m_mesh->vertices ());
            TFaceRenderer<Face>::DrawElements (m_mesh->faces ());
            TVertexRenderer<Vertex>::Disable ();
          }
        }
    };

////////////////////////////////////////////////////////////////////////////////
// Aa::Mesh::BasicMeshRenderer /////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    typedef TMeshRenderer<BasicMesh> BasicMeshRenderer;

////////////////////////////////////////////////////////////////////////////////
// Aa::Mesh::NormalMeshRenderer<M> /////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    typedef TMeshRenderer<NormalMesh> NormalMeshRenderer;

  } // namespace Mesh
} // namespace Aa

#endif // AA_MESH_RENDERER__H

