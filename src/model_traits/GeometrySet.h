#ifndef BC_GEOMETRY_H__
#define BC_GEOMETRY_H__

#include "Convert.h"
#include "Typedefs.h"
#include "Utility.h"

namespace mt {

// FIXME Better name?
struct DimGeometry {
  DimGeometry(OrdinalType d, OrdinalType g) : dim_(d), GID_(g) {}
  OrdinalType GetDimension() const { return dim_; }
  OrdinalType GetID() const { return GID_; }
  bool operator==(const DimGeometry &rhs) const {
    return dim_ == rhs.dim_ && GID_ == rhs.GID_;
  }
  bool operator!=(const DimGeometry &rhs) const { return !(rhs == *this); }
  bool operator<(const DimGeometry &rhs) const {
    if (dim_ < rhs.dim_)
      return true;
    if (rhs.dim_ < dim_)
      return false;
    return GID_ < rhs.GID_;
  }
  bool operator>(const DimGeometry &rhs) const { return rhs < *this; }
  bool operator<=(const DimGeometry &rhs) const { return !(rhs < *this); }
  bool operator>=(const DimGeometry &rhs) const { return !(*this < rhs); }

private:
  OrdinalType dim_;
  OrdinalType GID_;
};

struct IGeometrySet;

template <typename, template <typename> class> class GeometrySet;

struct GeometrySetVisitor {
  virtual void visit(GeometrySet<OrdinalType, BC_VEC_WORKAROUND> &) = 0;
  virtual void visit(GeometrySet<DimGeometry, BC_VEC_WORKAROUND> &) = 0;
  virtual ~GeometrySetVisitor() = default;
};

struct IGeometrySet {
  virtual void accept(GeometrySetVisitor &v) = 0;
  IGeometrySet() = default;
  virtual ~IGeometrySet() = default;
  IGeometrySet(const IGeometrySet &) = default;
  IGeometrySet(IGeometrySet &&) = default;
  IGeometrySet &operator=(const IGeometrySet &) = default;
  IGeometrySet &operator=(IGeometrySet &&) = default;
};

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
  GeometrySet() = default;
  explicit GeometrySet(const Geometry &g) { geometry_.push_back(g); }
  explicit GeometrySet(Geometry &&g) { geometry_.push_back(std::move(g)); }
  explicit GeometrySet(std::initializer_list<Geometry> list)
      : GeometrySet(list.begin(), list.end()) {}
  // FIXME need to constrain this template
  template <typename InputIt>
  GeometrySet(InputIt first, InputIt last) : geometry_(first, last) {}
  using iterator = typename Container::iterator;
  using const_iterator = typename Container::const_iterator;
  iterator begin() noexcept { return geometry_.begin(); }
  const_iterator begin() const noexcept { return geometry_.begin(); }
  iterator end() noexcept { return geometry_.end(); }
  const_iterator end() const noexcept { return geometry_.end(); }
  virtual void accept(GeometrySetVisitor &v) final { v.visit(*this); }

private:
  Container geometry_;
};
// FIXME Better name?
template <template <typename> class Cont = BC_VEC_WORKAROUND>
using DimGeometrySet = GeometrySet<DimGeometry, Cont>;

} // namespace mt
#endif
