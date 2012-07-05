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

      protected:
        static inline
        void SetPointers (GLuint id, const Vertex * p);

        static inline
        void SetPointers (const std::vector<Vertex> & v)
        {
          if (! v.empty ())
            SetPointers (0, &(v[0]));
        }

        static inline
        void DrawElements (GLuint id, GLuint * p, GLuint count)
        {
          glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, id);
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

            DrawElements (0, &(indices[0]), indices.size ());
          }
        }

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
            SetPointers  (m_mesh->vertices ());
            DrawElements (m_mesh->triangles ());
            glBindBuffer (GL_ARRAY_BUFFER, 0);
            glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
          }
        }
    };

////////////////////////////////////////////////////////////////////////////////
// Aa::Mesh::BasicMeshRenderer /////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    typedef TMeshRenderer<BasicMesh> BasicMeshRenderer;

    template <>
    inline
    void BasicMeshRenderer::SetPointers (GLuint id, const BasicVertex * p)
    {
      glBindBuffer (GL_ARRAY_BUFFER, id);
      glEnableClientState (GL_VERTEX_ARRAY);
      glVertexPointer (3, GL_FLOAT, sizeof (BasicVertex), &(p->coords));
    }

////////////////////////////////////////////////////////////////////////////////
// Aa::Mesh::NormalMeshRenderer<M> /////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    typedef TMeshRenderer<NormalMesh> NormalMeshRenderer;

    template <>
    inline
    void NormalMeshRenderer::SetPointers (GLuint id, const NormalVertex * p)
    {
      glBindBuffer (GL_ARRAY_BUFFER, id);

      glEnableClientState (GL_VERTEX_ARRAY);
      glVertexPointer (3, GL_FLOAT, sizeof (NormalVertex), &(p->coords));

      glEnableClientState (GL_NORMAL_ARRAY);
      glNormalPointer    (GL_FLOAT, sizeof (NormalVertex), &(p->normal));
    }

  } // namespace Mesh
} // namespace Aa

#endif // AA_MESH_RENDERER__H

