#include "model_traits/AssociatedModelTraits.h"
#include "model_traits/ModelTrait.h"
#include "model_traits/ModelTraits.h"
#include "model_traits/Typedefs.h"
#include <catch2/catch_test_macros.hpp>

using mt::AssociatedModelTraits;
using mt::BoolMT;
using mt::DimIdGeometry;
using mt::DimIdGeometrySet;
using mt::IdGeometry;
using mt::IdGeometrySet;
using mt::IntMT;
using mt::ModelTraits;
using mt::MTCast;

TEST_CASE("Associate Traits", "[associated]") {
  ModelTraits model_traits{"test"};
  auto *case1 = model_traits.AddCase("case 1");

  SECTION("Associate Id Geometry") {
    case1->AddModelTrait("top level bool", IdGeometrySet({1, 2, 3}),
                         BoolMT{true});
    case1->AddModelTrait("top level int", IdGeometrySet({1, 2, 3}), IntMT{5});

    // adding cases potentially invalidates any pointers
    auto *cat = case1->AddCategory("category 1");
    cat->AddCategory("category 3");
    cat->AddCategory("category 2");
    auto *right = cat->FindCategoryNode("category 3");
    auto *left = cat->FindCategoryNode("category 2");
    left->AddModelTrait("base boundary condition 1",
                        IdGeometrySet({6, 8, 19, 34}), IntMT{10});
    right->AddModelTrait("base boundary condition 2",
                         IdGeometrySet({6, 8, 19, 34}), IntMT{17});

    AssociatedModelTraits<DimIdGeometry> wrong_assoc{case1};
    REQUIRE(wrong_assoc.NumGeometricEntities() == 0);
    REQUIRE(wrong_assoc.GetNullGeometry() == nullptr);

    AssociatedModelTraits<IdGeometry> model_association{case1};
    // seven unique geometric entities
    REQUIRE(model_association.NumGeometricEntities() == 7);
    auto *g1 = model_association.Find(1);
    auto *g2 = model_association.Find(2);
    auto *g19 = model_association.Find(19);
    REQUIRE(g1 != nullptr);
    REQUIRE(g2 != nullptr);
    REQUIRE(g19 != nullptr);
    REQUIRE(g1->GetGeometry() == 1);
    REQUIRE(g2->GetGeometry() == 2);
    REQUIRE(g19->GetGeometry() == 19);

    REQUIRE(model_association.Find(77) == nullptr);

    auto *bc1_bool = g1->FindModelTrait("top level bool");
    auto *bc1_int = g1->FindModelTrait("top level int");
    auto *bc2_bool = g2->FindModelTrait("top level bool");
    auto *bc2_int = g2->FindModelTrait("top level int");
    REQUIRE((*MTCast<BoolMT>(bc1_bool))() == true);
    REQUIRE((*MTCast<BoolMT>(bc2_bool))() == true);

    REQUIRE((*MTCast<IntMT>(bc1_int))() == 5);
    REQUIRE((*MTCast<IntMT>(bc2_int))() == 5);
    REQUIRE(bc1_bool != nullptr);
    REQUIRE(bc1_int != nullptr);
    REQUIRE(bc2_bool != nullptr);
    REQUIRE(bc2_int != nullptr);
    // the top level geometry should not see the mt inside of a category
    REQUIRE(g19->FindModelTrait("base boundary condition 1") == nullptr);
    REQUIRE(g19->FindModelTrait("base boundary condition 2") == nullptr);

    REQUIRE(g1->FindCategory("category 1") == nullptr);
    auto *category1 = g19->FindCategory("category 1");
    REQUIRE(category1 != nullptr);
    auto *category2 = category1->FindCategory("category 2");
    auto *category3 = category1->FindCategory("category 3");
    REQUIRE(category2 != nullptr);
    REQUIRE(category3 != nullptr);
    auto *base_bc_1 = category2->FindModelTrait("base boundary condition 1");
    auto *base_bc_2 = category3->FindModelTrait("base boundary condition 2");
    REQUIRE(base_bc_1 != nullptr);
    REQUIRE(base_bc_2 != nullptr);
    REQUIRE((*MTCast<IntMT>(base_bc_1))() == 10);
    REQUIRE((*MTCast<IntMT>(base_bc_2))() == 17);
    REQUIRE(model_association.GetNullGeometry() == nullptr);
  }
  SECTION("Associate Dim Geometry") {

    case1->AddModelTrait("top level int", DimIdGeometrySet({{1, 1}, {1, 2}}),
                         IntMT{7});
    case1->AddModelTrait("top level int", DimIdGeometrySet({{2, 1}, {2, 2}}),
                         IntMT{5});
    AssociatedModelTraits<DimIdGeometry> model_association{case1};

    // seven unique geometric entities
    REQUIRE(model_association.NumGeometricEntities() == 4);
    auto *g1 = model_association.Find({1, 1});
    auto *g2 = model_association.Find({2, 1});
    REQUIRE(g1 != nullptr);
    REQUIRE(g2 != nullptr);
    REQUIRE(g1->GetGeometry() == DimIdGeometry{1, 1});
    REQUIRE(g1->GetGeometry() != DimIdGeometry{2, 1});
    REQUIRE(g2->GetGeometry() == DimIdGeometry{2, 1});
    REQUIRE(g2->GetGeometry() != DimIdGeometry{1, 1});

    REQUIRE(model_association.Find({1, 3}) == nullptr);

    auto *bc1 = g1->FindModelTrait("top level int");
    REQUIRE(bc1 != nullptr);
    const IntMT *bc1_int = MTCast<IntMT>(bc1);
    REQUIRE(bc1_int != nullptr);
    REQUIRE((*bc1_int)() == 7);

    auto *bc2 = g2->FindModelTrait("top level int");
    REQUIRE(bc2 != nullptr);
    const IntMT *bc2_int = MTCast<IntMT>(bc2);
    REQUIRE(bc2_int != nullptr);
    REQUIRE((*bc2_int)() == 5);
    REQUIRE(model_association.GetNullGeometry() == nullptr);
  }
  SECTION("Repeat BC Fail") {
    // the name is a misnomer on purpose. This test shows that even if the BCs
    // are different types, but they share the same name they cannot be
    // associated on the same model entity
    case1->AddModelTrait("top level int", IdGeometrySet({1, 2, 3}),
                         BoolMT{true});
    case1->AddModelTrait("top level int", IdGeometrySet({1, 5, 7}), IntMT{5});

    // multiple boundary conditions with the same name applied to the same
    // geometry fails
    REQUIRE_THROWS(AssociatedModelTraits<IdGeometry>(case1));
  }
  SECTION("empty geometry set") {
    case1->AddModelTrait("top level int", IdGeometrySet(), IntMT{88});
    AssociatedModelTraits<IdGeometry> associated_model_traits{case1};
    REQUIRE(associated_model_traits.NumGeometricEntities() == 1);
    REQUIRE(associated_model_traits.GetNullGeometry() != nullptr);
    auto *trait = associated_model_traits.GetNullGeometry()->FindModelTrait(
        "top level int");
    REQUIRE(trait != nullptr);
    auto *int_trait = MTCast<IntMT>(trait);
    REQUIRE(int_trait != nullptr);
    REQUIRE((*int_trait)() == 88);
  }
}
