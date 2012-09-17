#ifndef AA_MESH_NOFF__H
#define AA_MESH_NOFF__H

#include <fstream>
#include <AaMesh>

namespace Aa
{
  namespace Mesh
  {

////////////////////////////////////////////////////////////////////////////////
// Aa::Mesh::noff_read /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    inline
    NormalVertex noff_read_vertex (std::istream & is)
    {
      double px, py, pz, nx, ny, nz;
      is >> px >> py >> pz >> nx >> ny >> nz;
      return NormalVertex (vec (px, py, pz), vec (nx, ny, nz));
    }

    inline
    BasicTriangle noff_read_triangle (std::istream & is)
    {
      AaUInt n, a, b, c;
      is >> n >> a >> b >> c;
      return BasicTriangle (vec (a, b, c));
    }

    inline
    NormalMesh * noff_read (std::istream & is)
    {
      std::string noff;
      is >> noff;

      AaUInt vertices, triangles, edges;
      is >> vertices >> triangles >> edges;

      NormalMesh * m = new NormalMesh (vertices, triangles);

      for (AaUInt i = 0; i < vertices; ++i)
        m->addVertex (noff_read_vertex (is));

      for (AaUInt i = 0; i < triangles; ++i)
        m->addTriangle (noff_read_triangle (is));

      return m;
    }

    inline
    NormalMesh * noff_read (const std::string & path)
    {
      std::ifstream ifs (path.c_str ());
      return noff_read (ifs);
    }

////////////////////////////////////////////////////////////////////////////////
// Aa::Mesh::noff_write ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    inline
    void noff_write_vertex (std::ostream & os, const NormalVertex & v)
    {
      os << v.coords [0] << ' '
         << v.coords [1] << ' '
         << v.coords [2] << ' '
         << v.normal [0] << ' '
         << v.normal [1] << ' '
         << v.normal [2] << std::endl;
    }

    inline
    void noff_write_triangle (std::ostream & os, const BasicTriangle & t)
    {
      os << "3 "
         << t.indices [0] << ' '
         << t.indices [1] << ' '
         << t.indices [2] << std::endl;
    }

    inline
    void noff_write (std::ostream & os, NormalMesh * m)
    {
      const std::vector<NormalVertex>  & v = m->vertices  ();
      const std::vector<BasicTriangle> & t = m->triangles ();

      os << "NOFF" << std::endl;
      os << v.size () << ' ' << t.size () << " 0" << std::endl;

      for (AaUInt i = 0; i < v.size (); ++i)
        noff_write_vertex (os, v[i]);

      for (AaUInt i = 0; i < t.size (); ++i)
        noff_write_triangle (os, t[i]);
    }

    inline
    void noff_write (const std::string & path, NormalMesh * m)
    {
      std::ofstream ofs (path.c_str ());
      noff_write (ofs, m);
    }

  } // namespace Mesh
} // namespace Aa

#endif // AA_MESH_NOFF__H

