#ifndef BC_NODE_VISITOR_H__
#define BC_NODE_VISITOR_H__

namespace bc {
struct CategoryNode;
struct BCNode;

struct NodeVisitor {
  virtual void visit(CategoryNode &) = 0;
  virtual void visit(BCNode &) = 0;
  virtual ~NodeVisitor() = default;
};

} // namespace bc

#endif
