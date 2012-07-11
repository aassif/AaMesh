#ifndef AA_MESH_RENDERER__H
#define AA_MESH_RENDERER__H

#include <GL/glew.h>

namespace Aa 
{
  namespace Mesh
  {

////////////////////////////////////////////////////////////////////////////////
// Aa::Mesh::TMeshRenderer<M> //////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    template <class M>
    class TMeshRenderer
    {
      public:
        typedef M                    Mesh;
        typedef typename M::Vertex   Vertex;
        typedef typename M::Triangle Triangle;

      protected:
        Mesh * m_mesh;

      public:
        static inline
        void SetPointers (const Vertex * p);

        static inline
        void SetPointers (const std::vector<Vertex> & v)
        {
          if (! v.empty ())
            SetPointers (&(v[0]));
        }

        static inline
        void DrawElements (GLuint * p, GLuint count)
        {
          glDrawElements (GL_TRIANGLES, count, GL_UNSIGNED_INT, p);
        }

        static inline
        void DrawElements (const std::vector<Triangle> & t)
        {
          if (! t.empty ())
          {
            std::vector<GLuint> indices (3 * t.size ());
            for (AaUInt i = 0; i < t.size (); ++i)
            {
              indices [3*i+0] = t[i].indices[0];
              indices [3*i+1] = t[i].indices[1];
              indices [3*i+2] = t[i].indices[2];
            }

            DrawElements (&(indices[0]), indices.size ());
          }
        }

        static inline
        void Disable ();

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
            SetPointers  (m_mesh->vertices  ());
            DrawElements (m_mesh->triangles ());
            Disable ();
          }
        }
    };

////////////////////////////////////////////////////////////////////////////////
// Aa::Mesh::BasicMeshRenderer /////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    typedef TMeshRenderer<BasicMesh> BasicMeshRenderer;

    template <>
    inline
    void BasicMeshRenderer::SetPointers (const BasicVertex * p)
    {
      glEnableClientState (GL_VERTEX_ARRAY);
      glVertexPointer (3, GL_FLOAT, sizeof (BasicVertex), &(p->coords));
    }

    template <>
    inline
    void BasicMeshRenderer::Disable ()
    {
      glDisableClientState (GL_VERTEX_ARRAY);
    }

////////////////////////////////////////////////////////////////////////////////
// Aa::Mesh::NormalMeshRenderer<M> /////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    typedef TMeshRenderer<NormalMesh> NormalMeshRenderer;

    template <>
    inline
    void NormalMeshRenderer::SetPointers (const NormalVertex * p)
    {
      glEnableClientState (GL_VERTEX_ARRAY);
      glVertexPointer (3, GL_FLOAT, sizeof (NormalVertex), &(p->coords));

      glEnableClientState (GL_NORMAL_ARRAY);
      glNormalPointer    (GL_FLOAT, sizeof (NormalVertex), &(p->normal));
    }

    template <>
    inline
    void NormalMeshRenderer::Disable ()
    {
      glDisableClientState (GL_VERTEX_ARRAY);
      glDisableClientState (GL_NORMAL_ARRAY);
    }

  } // namespace Mesh
} // namespace Aa

#endif // AA_MESH_RENDERER__H

