#ifndef __AA_MESH_EXPORT__
#define __AA_MESH_EXPORT__

#include <fstream>
#include "bits/AaMesh.hh"

namespace Aa
{
  namespace Mesh
  {

////////////////////////////////////////////////////////////////////////////////
// export_off //////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////
// export_off //////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    void export_vrml_vertex (ostream & o, const BasicVertex & v)
    {
      o << "        " << v.coords [0] << ' ' << v.coords [1] << ' ' << v.coords [2] << endl;
    }

    void export_vrml_triangle (ostream & o, const BasicTriangle & t)
    {
      o << "      " << t.indices [0] << ", " << t.indices [1] << ", " << t.indices [2] << ", -1\n";
    }

    void export_vrml (const BasicMesh * mesh, const string & filename)
    {
      const vector<BasicVertex>   & vertices  = mesh->getVertices ();
      const vector<BasicTriangle> & triangles = mesh->getTriangles ();

      ofstream of (filename.c_str ());
      of << "#VRML V2.0 utf8\n"
         << "Shape {\n"
         << "  appearance Appearance {\n"
         << "    material Material {}\n"
         << "    }\n"
         << "  geometry IndexedFaceSet {\n"
         << "    coord Coordinate {\n"
         << "      point [\n";
      if (! vertices.empty ())
      {
        export_vrml_vertex (of, vertices [0]);
        for (int i = 1; i < vertices.size ();) export_vrml_vertex (of, vertices [i++]);
      }
      of << "      ]\n"
         << "    }\n"
         << "    coordIndex [\n";
      if (! triangles.empty ())
      {
        export_vrml_triangle (of, triangles [0]);
        for (int i = 1; i < triangles.size ();) export_vrml_triangle (of, triangles [i++]);
      }
      of << "    ]\n"
         << "  }\n"
         << "}\n";
      of.close ();
    }

  }
}

#endif // __AA_MESH_EXPORT__

