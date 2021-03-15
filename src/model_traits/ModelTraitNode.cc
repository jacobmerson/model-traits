#include "model_traits/ModelTraitNode.h"

namespace mt {

ModelTraitNode::ModelTraitNode(const std::string &name,
                               ModelTraitNode::GeomPtrT g,
                               ModelTraitNode::MTPtrT bc)
    : name_(name) {
  model_traits_.emplace_back(std::move(g), std::move(bc));
}
void ModelTraitNode::AddModelTrait(ModelTraitNode::GeomPtrT g,
                                   ModelTraitNode::MTPtrT bc) {
  model_traits_.emplace_back(std::move(g), std::move(bc));
}
const std::vector<std::pair<ModelTraitNode::GeomPtrT, ModelTraitNode::MTPtrT>> &
ModelTraitNode::GetModelTraits() const noexcept {
  return model_traits_;
}
const std::string &ModelTraitNode::GetName() const noexcept { return name_; }
} // namespace mt