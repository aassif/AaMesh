#ifndef AA_MESH_VAO__H
#define AA_MESH_VAO__H

namespace Aa 
{
  namespace Mesh
  {

////////////////////////////////////////////////////////////////////////////////
// Aa::Mesh::TMeshVAO<M> ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    template <class M>
    class TMeshVAO
    {
      public:
        typedef /******/ M         Mesh;
        typedef typename M::Vertex Vertex;
        typedef typename M::Face   Face;

      protected:
        GLuint m_id;
        VBO  * m_vertices;
        VBO  * m_faces;

      protected:
        inline
        void create (const Mesh * m)
        {
          if (m != NULL)
          {
            m_vertices = VBO::Array        (m->vertices ());
            m_faces    = VBO::ElementArray (m->faces    ());

            glGenVertexArrays (1, &m_id);
            glBindVertexArray (m_id);

            glBindBuffer (GL_ARRAY_BUFFER, m_vertices->id);
            TVertexRenderer<Vertex>::SetPointers (NULL);

            glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, m_faces->id);

            glBindVertexArray (0);
          }
        }

        inline
        void destroy ()
        {
          if (m_id != 0)
          {
            glDeleteVertexArrays (1, &m_id);
            delete m_vertices;
            delete m_faces;
            m_id = 0;
          }
        }

      public:
        inline
        TMeshVAO (const Mesh * m = NULL) :
          m_id (0),
          m_vertices (NULL),
          m_faces (NULL)
        {
          create (m);
        }

        inline
        ~TMeshVAO ()
        {
          destroy ();
        }

        inline
        void bind () const
        {
          glBindVertexArray (m_id);
        }

        inline
        void draw () const
        {
          if (m_id != 0)
          {
            this->bind ();
            TFaceRenderer<Face>::DrawElements (NULL, m_faces->count);
          }
        }
    };

////////////////////////////////////////////////////////////////////////////////
// Aa::Mesh::BasicMeshVAO //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    typedef TMeshVAO<BasicMesh> BasicMeshVAO;

////////////////////////////////////////////////////////////////////////////////
// Aa::Mesh::NormalMeshVAO /////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    typedef TMeshVAO<NormalMesh> NormalMeshVAO;

  } // namespace Mesh
} // namespace Aa

#endif // AA_MESH_VAO__H

