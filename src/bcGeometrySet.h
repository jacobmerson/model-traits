#ifndef BC_GEOMETRY_H__
#define BC_GEOMETRY_H__
#include "bcConvert.h"
#include "bcTypedefs.h"
#include "bcUtility.h"
#include <unordered_set>

namespace bc {
struct IGeometrySet {};

/*
 * Note this class is templated, so geometry can be stored as Ordinal types
 * or something more complex, like a struct that stores a dimension id, and
 * geometry id, or even a pointer to some geometry.
 *
 */
template <typename Geometry = OrdinalType,
          template <typename> class Cont = BC_VEC_WORKAROUND>
class GeometrySet : public IGeometrySet,
                    public Convertible<GeometrySet<Geometry, Cont>> {
public:
  using Container = Cont<Geometry>;
  template <typename... Args>
  GeometrySet(Args &&... args) : geometry_(std::forward<Args>(args)...) {}
  using iterator = typename Container::iterator;
  using const_iterator = typename Container::const_iterator;
  iterator begin() noexcept { return geometry_.begin(); }
  const_iterator begin() const noexcept { return geometry_.begin(); }
  iterator end() noexcept { return geometry_.end(); }
  const_iterator end() const noexcept { return geometry_.end(); }

private:
  Container geometry_;
};

} // namespace bc
#endif
