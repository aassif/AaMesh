#ifndef AA_MESH_NORMALS__H
#define AA_MESH_NORMALS__H

#ifdef __APPLE__
  #include <OpenGL/gl3.h>
#else
  #include <GL/glew.h>
#endif

namespace Aa
{
  namespace Mesh
  {

    template <class M>
    class TMeshNormals
    {
      public:
        typedef /******/ M         Mesh;
        typedef typename M::Vertex Vertex;
        typedef typename M::Face   Face;

        typedef Aa::V<vec3, 2>     Line;

      private:
        inline static
        void Compute (Mesh *, const Face &);

        inline static
        Line Draw (const Vertex &, float f);

      public:
        inline static
        void Compute (Mesh * m)
        {
          std::vector<Vertex> & v = m->vertices ();
          std::vector<Face>   & f = m->faces    ();

          for (AaUInt i = 0; i < v.size (); ++i)
            v[i].normal = vec3 (0);

          for (AaUInt i = 0; i < f.size (); ++i)
            TMeshNormals<M>::Compute (m, f[i]);

          for (AaUInt i = 0; i < v.size (); ++i)
          {
            double d2 = v[i].normal.length ();
            if (d2 != 0) v[i].normal /= d2;
          }
        }

        inline static
        void Draw (const Mesh * m, float f = 1.0f)
        {
          const std::vector<Vertex> & v = m->vertices ();
          if (! v.empty ())
          {
            std::vector<Line> lines;
            for (AaUInt i = 0; i < v.size (); ++i)
              lines.push_back (TMeshNormals<M>::Draw (v[i], f));

            GLuint vbo = 0;
            glGenBuffers (1, &vbo);
            glBindBuffer (GL_ARRAY_BUFFER, vbo);
            glBufferData (GL_ARRAY_BUFFER, lines.size () * sizeof (Line), &(lines[0][0][0]), GL_STATIC_DRAW);
            glEnableVertexAttribArray (0);
            glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
            glDrawArrays (GL_LINES, 0, 2 * lines.size ());
            glDisableVertexAttribArray (0);
          }
        }
    };

    template <>
    inline
    void TMeshNormals<NormalMesh>::Compute (Mesh * m, const BasicTriangle & t)
    {
      NormalVertex & v0 = m->vertex (t.indices [0]);
      NormalVertex & v1 = m->vertex (t.indices [1]);
      NormalVertex & v2 = m->vertex (t.indices [2]);

      vec3 n1 = CrossProd (v2.coords - v0.coords, v1.coords - v0.coords);
      float d = n1.length ();
      if (d != 0)
      {
        vec3 n2 = n1 / d;
        v0.normal += n2;
        v1.normal += n2;
        v2.normal += n2;
      }
    }

    template <>
    inline
    TMeshNormals<NormalMesh>::Line
      TMeshNormals<NormalMesh>::Draw (const NormalVertex & v, float f)
    {
      return Line (v.coords, v.coords + f * v.normal);
    }

  } // namespace Mesh
} // namespace Aa

#endif // AA_MESH_NORMALS__H

