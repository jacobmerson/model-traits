//
// This example shows how associated model traits can be loaded into a classified PUMI mesh
// Results in a vtk database named mt-pumi where you can visually verify that the model traits
// are applied correctly.
//
#include <apf.h>
#include <gmi_mesh.h>
#include <apfMDS.h>
#include <apfMesh2.h>
#include <apfShape.h>
#include <model_traits/ModelTraits.h>
#include <model_traits/ModelTraitsIO.h>
#include <model_traits/AssociatedModelTraits.h>
#include <iostream>
#include <fmt/format.h>
#include <mpi.h>
#include <PCU.h>
#include <array>
#include <cassert>


int main(int argc, char** argv)
{
  // initialize MPI and PCU
  MPI_Init(&argc,&argv);
  PCU_Comm_Init();

  // load the model traits from yaml
  auto model_traits = mt::ReadFromFile<mt::YAML>("inputs/pumi.yaml");
  // get the mesh and model name
  auto* boundary_conditions = GetCategoryByType(&(model_traits->GetCases()[0]),"boundary conditions");
  auto mesh_name = (*mt::MTCast<mt::StringMT>(boundary_conditions->FindModelTraitNode("mesh")->GetModelTraits()[0].second.get()))();
  auto model_name = (*mt::MTCast<mt::StringMT>(boundary_conditions->FindModelTraitNode("model")->GetModelTraits()[0].second.get()))();
  // created the associated model traits to enable search by geometry
  auto associated_model_traits = mt::AssociatedModelTraits<mt::DimIdGeometry>(
      model_traits->GetCases()[0].FindCategoryNodeByType("boundary conditions"));

  // load the pumi mesh and geometry
  gmi_register_mesh();
  auto* mesh = apf::loadMdsMesh(model_name.c_str(),mesh_name.c_str());


  // create and zero a vector and scalar fields
  auto* vector_field = apf::createField(mesh,"vector field",apf::VECTOR, apf::getLagrange(1));
  auto* classified_field = apf::createField(mesh,"classified",apf::SCALAR, apf::getLagrange(1));
  apf::zeroField(vector_field);
  apf::zeroField(classified_field);

  // loop through all mesh entities
  for(int dim = 0; dim<3; ++dim)
  {
  auto* it = mesh->begin(dim);
  apf::MeshEntity* e;
  while((e = mesh->iterate(it)))
  {
    auto* model_entity = mesh->toModel(e);
    auto model_dim = mesh->getModelType(model_entity);
    auto model_tag = mesh->getModelTag(model_entity);
    // get the model traits associated on the geometric entitiy that
    // the mesh entity e is classified on
    auto* traits = associated_model_traits.Find({model_dim,model_tag});
    // skip over any entities where there are no traits to be applied
    if(traits != nullptr)
    {
      // find the model traits with the given names
      auto* classified = traits->FindModelTrait("classified");
      auto* outer_edge = traits->FindModelTrait("outer edge");

      // set the classified fields values based on input model traits data
      if(classified != nullptr)
      {
        auto* value = mt::MTCast<mt::ScalarMT>(classified);
        if(dim > 0)
        {
          apf::Adjacent verts;
          mesh->getAdjacent(e,0,verts);
          for(size_t i=0; i<verts.getSize(); ++i)
          {
            apf::setScalar(classified_field,verts[i],0,(*value)());
          }
        }
        else {
          apf::setScalar(classified_field,e,0,(*value)());
        }
      }
      // set the vector field based on the input vector field data
      else if(outer_edge != nullptr)
      {
        auto& value = *mt::MTCast<mt::VectorMT>(outer_edge);
        if(dim>0)
        {
          apf::Adjacent verts;
          mesh->getAdjacent(e,0,verts);
          for(size_t i=0; i<verts.getSize(); ++i)
          {
            apf::setVector(vector_field,verts[i],0,apf::Vector3(value(0),value(1),value(2)));
          }
        }
        else {
          apf::setVector(vector_field,e,0,apf::Vector3(value(0),value(1),value(2)));
        }
      }
    }
  }
  mesh->end(it);
  }
  // write the result to a vtk file for visual inspection
  apf::writeVtkFiles("mt-pumi", mesh);

  // cleanup pumi mesh/model
  mesh->destroyNative();
  apf::destroyMesh(mesh);

  // cleanup PCU and MPI
  PCU_Comm_Free();
  MPI_Finalize();
  return 0;
}
