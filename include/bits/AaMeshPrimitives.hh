#ifndef AA_MESH_PRIMITIVES__H
#define AA_MESH_PRIMITIVES__H

namespace Aa
{
  namespace Mesh
  {

////////////////////////////////////////////////////////////////////////////////
// Aa::Mesh::TDisk<M> //////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    template <class M>
    class TQuad
    {
      public:
        typedef M Mesh;
        typedef typename Mesh::Vertex   Vertex;
        typedef typename Mesh::Triangle Triangle;

      private:
        inline static
        Vertex CreateVertex (double x, double y, const vec3 & normal);

      public:
        inline static
        M * Create ()
        {
          M * m = new M;
          static const vec3 NORMAL = vec<float> (0, 0, 1);
          m->addVertex (CreateVertex (-1, -1, NORMAL));
          m->addVertex (CreateVertex (-1, +1, NORMAL));
          m->addVertex (CreateVertex (+1, -1, NORMAL));
          m->addVertex (CreateVertex (+1, +1, NORMAL));
          m->addTriangle (Triangle (0, 1, 2));
          m->addTriangle (Triangle (1, 3, 2));
          return m;
        }
    };

////////////////////////////////////////////////////////////////////////////////
// Aa::Mesh::TCube<M> //////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    template <class M>
    class TCube
    {
      public:
        typedef M Mesh;
        typedef typename Mesh::Vertex   Vertex;
        typedef typename Mesh::Triangle Triangle;

        enum
        {
          LEFT   = 0,
          RIGHT  = 1,
          BOTTOM = 2,
          TOP    = 3,
          BACK   = 4,
          FRONT  = 5
        };

      private:
        inline static
        Vertex CreateVertex (const vec3 & p, const vec3 & normal);

      public:
        inline static
        M * Create ()
        {
          static const vec3 POSITIONS [8] =
          {
            vec<float> (0, 0, 0),
            vec<float> (1, 0, 0),
            vec<float> (0, 1, 0),
            vec<float> (1, 1, 0),
            vec<float> (0, 0, 1),
            vec<float> (1, 0, 1),
            vec<float> (0, 1, 1),
            vec<float> (1, 1, 1)
          };

          static const vec3 NORMALS [6] =
          {
            vec<float> ( 0,  0, -1),
            vec<float> ( 0,  0, +1),
            vec<float> ( 0, -1,  0),
            vec<float> ( 0, +1,  0),
            vec<float> (-1,  0,  0),
            vec<float> (+1,  0,  0)
          };

          static const AaUInt FACES [6][4] = 
          {
            {0, 4, 6, 2},
            {5, 1, 3, 7},
            {0, 1, 5, 4},
            {6, 7, 3, 2},
            {0, 2, 3, 1},
            {4, 5, 7, 6},
          };

          M * m = new M;
          for (AaUInt i = 0; i < 6; ++i)
          {
            const vec3 & n = NORMALS [i];
            AaUInt v0 = m->addVertex (CreateVertex (POSITIONS[FACES[i][0]], n));
            AaUInt v1 = m->addVertex (CreateVertex (POSITIONS[FACES[i][1]], n));
            AaUInt v2 = m->addVertex (CreateVertex (POSITIONS[FACES[i][2]], n));
            AaUInt v3 = m->addVertex (CreateVertex (POSITIONS[FACES[i][3]], n));
            m->addTriangle (vec (v0, v1, v2));
            m->addTriangle (vec (v0, v2, v3));
          }
          return m;
        }
    };

    /** BasicCube */

    typedef TCube<BasicMesh> BasicCube;

    template <>
    inline
    BasicVertex BasicCube::CreateVertex (const vec3 & p, const vec3 &)
    {
      return BasicVertex (p);
    }

////////////////////////////////////////////////////////////////////////////////
// Aa::Mesh::TDisk<M> //////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    template <class M>
    class TDisk
    {
      public:
        typedef M Mesh;
        typedef typename Mesh::Vertex   Vertex;
        typedef typename Mesh::Triangle Triangle;

      private:
        inline static
        Vertex CreateVertex (double theta, double rho, const vec3 & normal);

      public:
        inline static
        M * Create (AaUInt nSlices, AaUInt nRings = 0, bool up = true)
        {
          M * m = new M;

          const vec3 normal = vec (0.0, 0.0, up ? +1.0 : -1.0);

          typedef std::vector<AaUInt> Ring;
          std::vector<Ring> rings (nRings + 2);

          rings [0].push_back (m->addVertex (CreateVertex (0.0, 0.0, normal)));

          for (AaUInt i = 1; i <= nRings + 1; ++i)
          {
            double rho = (double) i / (nRings + 1);
            rings [i].resize (nSlices + 1);

            for (AaUInt j = 0; j <= nSlices; ++j)
            {
              double theta = (2.0 * M_PI * j) / nSlices;
              rings [i][j] = m->addVertex (CreateVertex (theta, rho, normal));
            }
          }

          const AaUInt J0 = (up ? 0 : 1);
          const AaUInt J1 = (up ? 1 : 0);

          {
            for (AaUInt j0 = J0, j1 = J1; j0 < nSlices; ++j0, ++j1)
              m->addTriangle (Triangle (vec (rings [0][0], rings [1][j0], rings [1][j1])));
          }

          for (AaUInt i0 = 1, i1 = 2; i0 < nRings; ++i0, ++i1)
          {
            for (AaUInt j0 = J0, j1 = J1; j0 < nSlices; ++j0, ++j1)
            {
              m->addTriangle (Triangle (vec (rings [i0][j0], rings [i1][j0], rings [i1][j1])));
              m->addTriangle (Triangle (vec (rings [i0][j0], rings [i1][j1], rings [i0][j1])));
            }
          }

          return m;
        }
    };

    /** BasicDisk */

    typedef TDisk<BasicMesh> BasicDisk;

    template <>
    inline
    BasicVertex BasicDisk::CreateVertex (double theta, double rho, const vec3 &)
    {
      vec3 p = rho * vec (std::sin (theta), std::cos (theta), 0.0);
      return BasicVertex (p);
    }

    /** NormalDisk */

    typedef TDisk<NormalMesh> NormalDisk;

    template <>
    inline
    NormalVertex NormalDisk::CreateVertex (double theta, double rho, const vec3 & normal)
    {
      vec3 p = rho * vec (std::sin (theta), std::cos (theta), 0.0);
      return NormalVertex (p, normal);
    }

////////////////////////////////////////////////////////////////////////////////
// Aa::Mesh::TCylinder<M> //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    template <class M>
    class TCylinder
    {
      public:
        typedef M Mesh;
        typedef typename Mesh::Vertex   Vertex;
        typedef typename Mesh::Triangle Triangle;

      private:
        inline static
        Vertex CreateVertex (double theta, double h);

      public:
        inline static
        M * Create (AaUInt nSlices, AaUInt nStacks = 0)
        {
          M * m = new M;

          typedef std::vector<AaUInt> Stack;
          std::vector<Stack> stacks (nStacks + 2);

          for (AaUInt i = 0; i <= nStacks + 1; ++i)
          {
            double h = i / (nStacks + 1);
            stacks [i].resize (nSlices + 1);

            for (AaUInt j = 0; j <= nSlices; ++j)
            {
              double theta = (2.0 * M_PI * j) / nSlices;
              stacks [i][j] = m->addVertex (CreateVertex (theta, h));
            }
          }

          for (AaUInt i0 = 0, i1 = 1; i0 < nStacks + 1; ++i0, ++i1)
          {
            for (AaUInt j0 = 0, j1 = 1; j0 < nSlices; ++j0, ++j1)
            {
              m->addTriangle (Triangle (vec (stacks [i0][j0], stacks [i1][j0], stacks [i1][j1])));
              m->addTriangle (Triangle (vec (stacks [i0][j0], stacks [i1][j1], stacks [i0][j1])));
            }
          }

          return m;
        }
    };

    /** BasicCylinder */

    typedef TCylinder<BasicMesh> BasicCylinder;

    template <>
    inline
    BasicVertex BasicCylinder::CreateVertex (double theta, double h)
    {
      double sin_theta = std::sin (theta);
      double cos_theta = std::cos (theta);
      vec3 p = vec (sin_theta, cos_theta, h);
      return BasicVertex (p);
    }

    /** NormalCylinder */

    typedef TCylinder<NormalMesh> NormalCylinder;

    template <>
    inline
    NormalVertex NormalCylinder::CreateVertex (double theta, double h)
    {
      double sin_theta = std::sin (theta);
      double cos_theta = std::cos (theta);
      vec3 p = vec (sin_theta, cos_theta, h);
      vec3 n = vec (sin_theta, cos_theta, 0.0);
      return NormalVertex (p, n);
    }

////////////////////////////////////////////////////////////////////////////////
// Aa::Mesh::TSphere<M> ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    template <class M>
    class TSphere
    {
      public:
        typedef M Mesh;
        typedef typename Mesh::Vertex   Vertex;
        typedef typename Mesh::Triangle Triangle;

      private:
        inline static
        Vertex CreateVertex (double theta, double phi);

      public:
        inline static
        M * Create (AaUInt nSlices, AaUInt nStacks)
        {
          M * m = new M;

          typedef std::vector<AaUInt> Stack;
          std::vector<Stack> stacks (nStacks + 2);

          stacks [0].push_back (m->addVertex (CreateVertex (0.0, 0.0)));

          for (AaUInt i = 1; i < nStacks + 1; ++i)
          {
            double theta = (M_PI * i) / (nStacks + 1);
            stacks [i].resize (nSlices + 1);

            for (AaUInt j = 0; j <= nSlices; ++j)
            {
              double phi = (2.0 * M_PI * j) / nSlices;
              stacks [i][j] = m->addVertex (CreateVertex (theta, phi));
            }
          }

          stacks [nStacks+1].push_back (m->addVertex (CreateVertex (M_PI, 0.0)));

          {
            for (AaUInt j0 = 0, j1 = 1; j0 < nSlices; ++j0, ++j1)
              m->addTriangle (Triangle (vec (stacks [0][0], stacks [1][j0], stacks [1][j1])));
          }

          for (AaUInt i0 = 1, i1 = 2; i0 < nStacks; ++i0, ++i1)
          {
            for (AaUInt j0 = 0, j1 = 1; j0 < nSlices; ++j0, ++j1)
            {
              m->addTriangle (Triangle (vec (stacks [i0][j0], stacks [i1][j0], stacks [i1][j1])));
              m->addTriangle (Triangle (vec (stacks [i0][j0], stacks [i1][j1], stacks [i0][j1])));
            }
          }

          {
            for (AaUInt j0 = 0, j1 = 1; j0 < nSlices; ++j0, ++j1)
              m->addTriangle (Triangle (vec (stacks [nStacks][j0], stacks [nStacks+1][0], stacks [nStacks][j1])));
          }

          return m;
        }
    };

    /** BasicSphere */

    typedef TSphere<BasicMesh> BasicSphere;

    template <>
    inline
    BasicVertex BasicSphere::CreateVertex (double theta, double phi)
    {
      double sin_theta = std::sin (theta), sin_phi = std::sin (phi);
      double cos_theta = std::cos (theta), cos_phi = std::cos (phi);
      vec3 n = vec (sin_theta * cos_phi, sin_theta * sin_phi, cos_theta);
      return BasicVertex (n);
    }

    /** NormalSphere */

    typedef TSphere<NormalMesh> NormalSphere;

    template <>
    inline
    NormalVertex NormalSphere::CreateVertex (double theta, double phi)
    {
      double sin_theta = std::sin (theta), sin_phi = std::sin (phi);
      double cos_theta = std::cos (theta), cos_phi = std::cos (phi);
      vec3 n = vec (sin_theta * cos_phi, sin_theta * sin_phi, cos_theta);
      return NormalVertex (n, n);
    }

////////////////////////////////////////////////////////////////////////////////
// Aa::Mesh::TTorus<M> /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  } // namespace Mesh
} // namespace Aa

#endif // AA_MESH_PRIMITIVES__H

