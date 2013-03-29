#include <AaBox>
#include <AaMesh>

namespace Aa
{
  namespace Mesh
  {

    template <typename InputIterator>
    static box3 box (InputIterator first, InputIterator last)
    {
      BoxHelper3f h;
      for (InputIterator i = first; i != last; ++i)
        h.feed (i->coords);
      return h.get ();
    }

    template <class M>
    static box3 box (const M * mesh)
    {
      const std::vector<typename M::Vertex> & v = mesh->vertices ();
      return Mesh::box (v.begin (), v.end ());
    }

  } // namespace Mesh
} // namespace Aa

