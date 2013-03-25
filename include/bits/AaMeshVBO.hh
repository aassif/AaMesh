#ifndef AA_MESH_VBO__H
#define AA_MESH_VBO__H

namespace Aa 
{
  namespace Mesh
  {

////////////////////////////////////////////////////////////////////////////////
// Aa::Mesh::VBO ///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    class VBO
    {
      public:
        GLuint id;
        GLuint count;

      public:
        template <typename T>
        static inline
        VBO * Array (const T * data, GLuint count, GLenum usage = GL_STATIC_DRAW)
        {
          return new VBO (GL_ARRAY_BUFFER, data, count, usage);
        }

        template <typename T>
        static inline
        VBO * Array (const std::vector<T> & v, GLenum usage = GL_STATIC_DRAW)
        {
          return VBO::Array (v.empty () ? NULL : &(v[0]), v.size (), usage);
        }

        template <typename T>
        static inline
        VBO * ElementArray (const T * data, GLuint count, GLenum usage = GL_STATIC_DRAW)
        {
          return new VBO (GL_ELEMENT_ARRAY_BUFFER, data, count, usage);
        }

        template <typename T>
        static inline
        VBO * ElementArray (const std::vector<T> & v, GLenum usage = GL_STATIC_DRAW)
        {
          return VBO::ElementArray (v.empty () ? NULL : &(v[0]), v.size (), usage);
        }

      private:
        template <typename T>
        inline
        VBO (GLenum target, const T * data, GLuint count, GLenum usage) :
          id (0),
          count (count)
        {
          glGenBuffers (1, &id);
          glBindBuffer (target, id);
          glBufferData (target, count * sizeof (T), data, usage);
        }

      public:
        inline
        VBO () :
          id (0),
          count (0)
        {
        }

        inline
        ~VBO ()
        {
          glDeleteBuffers (1, &id);
        }
    };

////////////////////////////////////////////////////////////////////////////////
// Aa::Mesh::TMeshVBO<M> ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    template <class M>
    class TMeshVBO
    {
      public:
        typedef M                    Mesh;
        typedef typename M::Vertex   Vertex;
        typedef typename M::Triangle Triangle;

      protected:
        VBO * m_vertices;
        VBO * m_triangles;

      public:
        static inline
        VBO * CreateVBO (const std::vector<Vertex> & v, GLenum usage = GL_STATIC_DRAW)
        {
          return VBO::Array (v, usage);
        }

        static inline
        VBO * CreateVBO (const std::vector<Triangle> & t, GLenum usage = GL_STATIC_DRAW)
        {
          std::vector<GLuint> indices (3 * t.size ());
          for (AaUInt i = 0; i < t.size (); ++i)
          {
            indices [3*i + 0] = t[i].indices[0];
            indices [3*i + 1] = t[i].indices[1];
            indices [3*i + 2] = t[i].indices[2];
          }

          return VBO::ElementArray (indices, usage);
        }

        static inline
        void SetPointers (const VBO * vbo)
        {
          if (vbo != NULL)
          {
            glBindBuffer (GL_ARRAY_BUFFER, vbo->id);
            TMeshRenderer<M>::SetPointers (NULL);
          }
        }

        static inline
        void DrawElements (const VBO * vbo)
        {
          if (vbo != NULL)
          {
            glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, vbo->id);
            TMeshRenderer<M>::DrawElements (NULL, vbo->count);
          }
        }

      protected:
        inline
        void create (Mesh * m)
        {
          if (m != NULL)
          {
            m_vertices  = CreateVBO (m->vertices  ());
            m_triangles = CreateVBO (m->triangles ());
          }
        }

        inline
        void destroy ()
        {
          if (m_vertices  != NULL) delete m_vertices;
          if (m_triangles != NULL) delete m_triangles;
        }

      public:
        inline
        TMeshVBO (Mesh * m = NULL) :
          m_vertices (NULL),
          m_triangles (NULL)
        {
          create (m);
        }

        inline
        ~TMeshVBO ()
        {
          destroy ();
        }

        inline
        virtual void draw () const
        {
          if (m_vertices != NULL)
          {
            SetPointers  (m_vertices);
            DrawElements (m_triangles);
            glBindBuffer (GL_ARRAY_BUFFER, 0);
            glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
            TMeshRenderer<M>::Disable ();
          }
        }

        inline
        void draw_points () const
        {
          if (m_vertices != NULL)
          {
            SetPointers (m_vertices);
            glDrawArrays (GL_POINTS, 0, m_vertices->count);
            glBindBuffer (GL_ARRAY_BUFFER, 0);
            TMeshRenderer<M>::Disable ();
          }
        }
    };

////////////////////////////////////////////////////////////////////////////////
// Aa::Mesh::BasicMeshVBO //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    typedef TMeshVBO<BasicMesh> BasicMeshVBO;

////////////////////////////////////////////////////////////////////////////////
// Aa::Mesh::NormalMeshVBO /////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    typedef TMeshVBO<NormalMesh> NormalMeshVBO;

  } // namespace Mesh
} // namespace Aa

#endif // AA_MESH_VBO__H

