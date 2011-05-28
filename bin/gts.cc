#include <cstdlib>
#include <fstream>
#include <AaText>
//#include <AaMeshOff>
//#include <AaMeshVrml>
#include <AaMeshGts>

using namespace std;
using namespace Aa;
using namespace Aa::TextParsing;
using namespace Aa::Mesh;

template <class M>
void mesh_read (M * mesh, const string & filename)
{
  mesh->clear ();
  ifstream ifs (filename.c_str ());
  if (! ifs.is_open ()) return;

  ifs >>= "OFF";
  int nPoints, nFaces, nEdges;
  ifs >> nPoints >> nFaces >> nEdges;
  //mesh->reserve (nPoints, nFaces);

  for (int i = 0; i < nPoints; ++i)
  {
    double x, y, z;
    ifs >> x >> y >> z;
    mesh->addVertex (typename M::Vertex (x, y, z));
  }

  for (int i = 0; i < nFaces; ++i)
  {
    int a, b, c;
    ifs >>= "3";
    ifs >> a >> b >> c;
    mesh->addTriangle (typename M::Triangle (a, b, c));
  }
}

template <class M>
M * mesh_read (const string & filename)
{
  M * mesh = new M;
  mesh_read (mesh, filename);
  return mesh;
}

template <class Vertex>
Math::Box mesh_box (const std::vector<Vertex> & vertices)
{
  Math::BoxHelper h;
  for (unsigned int i = 0; i < vertices.size (); ++i)
  {
    const Vertex & v = vertices [i];
    h.feed (Math::pR3 (v.coords[0], v.coords[1], v.coords[2]));
  }
  return h.get ();
}

int main (int argc, char ** argv)
{
  if (argc != 2)
  {
    cout << "usage...\n";
    return 1;
  }

  srand48 (time (NULL));

  BasicMesh * mesh = mesh_read<BasicMesh> (argv [1]);
  GNode * tree = gts_bb_tree (mesh);

  Math::Box box = mesh_box<BasicVertex> (mesh->vertices ());
  cout << box << endl;
  const Math::pR3 & p = box.pos ();
  const Math::vR3 & d = box.dim ();

  int n = 10000;
  float mc = 0;
  for (int i = 0; i < n; ++i)
  {
    double x = p.x + d.x * drand48 ();
    double y = p.y + d.y * drand48 ();
    double z = p.z + d.z * drand48 ();
    mc += gts_point_is_inside_mesh (Math::pR3 (x, y, z), tree) ? 1.0 : 0.0;
  }
  cout << (mc / n) << endl;
  return 0;
}

