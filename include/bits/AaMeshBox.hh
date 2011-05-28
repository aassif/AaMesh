#include <AaMesh>

namespace Aa
{
  namespace Mesh
  {
    template <class Vertex>
    static Box box (const std::vector<Vertex> & vertices)
    {
      BoxHelper h;
      for (unsigned int i = 0; i < vertices.size (); ++i)
      {
        const Vertex & v = vertices [i];
        h.feed (Math::pR3 (v.coords[0], v.coords[1], v.coords[2]));
      }
      return h.get ();
    }
  } // namespace Mesh
} // namespace Aa

