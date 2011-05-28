#ifndef __AA_MESH_EXPORT__
#define __AA_MESH_EXPORT__

#include <fstream>
#include "bits/AaMesh.hh"

namespace Aa
{
  namespace Mesh
  {
    void export_off_vertex (std::ostream & o, const BasicVertex & v)
    {
      o << v.coords [0] << ' ' << v.coords [1] << ' ' << v.coords [2] << std::endl;
    }

    void export_off_triangle (std::ostream & o, const BasicTriangle & t)
    {
      o << "3 " << t.indices [0] << ' ' << t.indices [1] << ' ' << t.indices [2] << std::endl;
    }

    void export_off (const BasicMesh * mesh, const std::string & filename)
    {
      const std::vector<Mesh::BasicVertex>   & v = mesh->vertices ();
      const std::vector<Mesh::BasicTriangle> & t = mesh->triangles ();

      std::ofstream ofs (filename.c_str ());

      // Header.
      ofs << "OFF" << std::endl
          << v.size () << ' '
          << t.size () << " 0" << std::endl;

      // Vertices.
      for (unsigned int i = 0; i < v.size ();) export_off_vertex (ofs, v[i++]);

      // Triangles.
      for (unsigned int i = 0; i < t.size ();) export_off_triangle (ofs, t[i++]);

      ofs.close ();
    }
  }
}

#endif // __AA_MESH_EXPORT__

