#ifndef AA_MESH_EXTRUSION__H
#define AA_MESH_EXTRUSION__H

#include <AaCatmullRom>

namespace Aa
{
  namespace Mesh
  {

////////////////////////////////////////////////////////////////////////////////
// Aa::Mesh::Extrusion<M> //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    template <class M>
    class Extrusion
    {
      public:
        typedef /******/ M          Mesh;
        typedef typename M::Vertex  Vertex;
        typedef typename M::Face    Face;
        typedef std::vector<Vertex> Section;

      private:
        inline static
        Vertex CreateVertex (float r, float theta);

        inline static
        Section CreateSection (float r, AaUInt n = 8);

      public:
        inline static
        std::vector<mat4> RMF (const std::vector<vec3> & points);

        inline static
        M * Extrude (const std::vector<mat4> & frames, const Section &);

        inline static
        M * Extrude (const std::vector<vec3> & points, float radius, AaUInt n = 8);
    };

////////////////////////////////////////////////////////////////////////////////
// Aa::Mesh::Extrusion<M> //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    template <class M>
    inline
    typename Extrusion<M>::Section Extrusion<M>::CreateSection (float r, AaUInt n)
    {
      Section section;
      for (AaUInt i = 0; i <= n; ++i)
      {
        float theta = (2.0f * M_PI * i) / n;
        section.push_back (Extrusion<M>::CreateVertex (r, theta));
      }
      return section;
    }

    template <class M>
    inline
    std::vector<mat4> Extrusion<M>::RMF (const std::vector<vec3> & p)
    {
      const AaUInt n = p.size ();

      std::vector<vec3> t;
      t.push_back ((p[1] - p[0]).normalize ());
      for (AaUInt i = 1; i < n - 1; ++i)
        t.push_back ((p[i+1] - p[i-1]).normalize ());
      t.push_back ((p[n-1] - p[n-2]).normalize ());

      std::vector<mat4> frames;
      frames.push_back (mat4 () * Translation (p[0]));

      for (AaUInt i = 1; i < n; ++i)
      {
        const mat3 & f0 = frames [i-1];

        vec3 v1 = p[i] - p[i-1];
        float c1 = v1.length2 ();
        vec3 rl = f0[0] - (2.0/c1) * DotProd (v1, f0[0]) * v1;
        vec3 tl = f0[2] - (2.0/c1) * DotProd (v1, f0[2]) * v1;
        
        vec3 v2 = t[i] - tl;
        float c2 = v2.length2 ();
        vec3 r = (rl - (2.0/c2) * DotProd (v2, rl) * v2).normalize ();
        vec3 s = CrossProd (t[i], r).normalize ();

        mat3 f = mat (r, s, t[i]);
        frames.push_back (Translation (p[i]) * mat4 (f));
      }

      return frames;
    }

    template <class M>
    inline
    M * Extrusion<M>::Extrude (const std::vector<mat4> & frames, const Section & section)
    {
      M * mesh = new M;

      const AaUInt n = frames.size ();
      const AaUInt m = section.size ();

//      std::cout << "n = " << n << std::endl;
//      std::cout << "m = " << m << std::endl;

      typedef std::vector<AaUInt> Stack;
      std::vector<Stack> stacks (n);

      for (AaUInt i = 0; i < n; ++i)
      {
        stacks [i].resize (m);

        TVertexTransform<Vertex> f (frames [i]);
        for (AaUInt j = 0; j < m; ++j)
          stacks [i][j] = mesh->addVertex (f (section [j]));
      }

      for (AaUInt i0 = 0, i1 = 1; i1 < n; ++i0, ++i1)
      {
        for (AaUInt j0 = 0, j1 = 1; j1 < m; ++j0, ++j1)
        {
          mesh->addFace (Face (vec (stacks [i0][j0], stacks [i1][j0], stacks [i1][j1])));
          mesh->addFace (Face (vec (stacks [i0][j0], stacks [i1][j1], stacks [i0][j1])));
        }
      }

      return mesh;
    }

    template <class M>
    inline
    M * Extrusion<M>::Extrude (const std::vector<vec3> & points, float r, AaUInt n)
    {
      return Extrusion<M>::Extrude (Extrusion<M>::RMF (points), Extrusion<M>::CreateSection (r, n));
    }

////////////////////////////////////////////////////////////////////////////////
// Aa::Mesh::Extrusion<NormalMesh> /////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    template <>
    inline
    NormalVertex Extrusion<NormalMesh>::CreateVertex (float r, float theta)
    {
      float sin_theta = std::sin (theta);
      float cos_theta = std::cos (theta);
      vec3 p = vec (sin_theta * r, cos_theta * r, 0.0f);
      vec3 n = vec (sin_theta,     cos_theta,     0.0f);
      return NormalVertex (p, n);
    }

  } // namespace Mesh
} // namespace Aa

#endif // AA_MESH_EXTRUSION__H

