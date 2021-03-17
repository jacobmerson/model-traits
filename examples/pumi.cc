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
  MPI_Init(&argc,&argv);
  PCU_Comm_Init();
  if(argc != 3)
  {
    fmt::print("Usage: {} pumi.yaml /path/to/pumi-meshes/\n",argv[0]);
    exit(1);
  }
  // load the model traits from yaml
  auto model_traits = mt::ReadFromFile<mt::YAML>(fmt::format("{}",argv[1]));
  // created the associated model traits to enable search by geometry
  auto associated_model_traits = mt::AssociatedModelTraits<mt::DimGeometry>(&(model_traits->GetCases()[0]));

  // load the pumi mesh and geometry
  gmi_register_mesh();
  auto* mesh = apf::loadMdsMesh(fmt::format("{}/pumi/model.dmg",argv[2]).c_str(),fmt::format("{}/pumi/2d.smb",argv[2]).c_str());


  auto* vector_field = apf::createField(mesh,"vector field",apf::VECTOR, apf::getLagrange(1));
  auto* classified_field = apf::createField(mesh,"classified",apf::SCALAR, apf::getLagrange(1));
  apf::zeroField(vector_field);
  apf::zeroField(classified_field);

  for(int dim = 0; dim<3; ++dim)
  {
  auto* it = mesh->begin(dim);
  apf::MeshEntity* e;
  while((e = mesh->iterate(it)))
  {
    auto* model_entity = mesh->toModel(e);
    auto model_dim = mesh->getModelType(model_entity);
    auto model_tag = mesh->getModelTag(model_entity);
    auto* traits = associated_model_traits.Find({model_dim,model_tag});
    if(traits != nullptr)
    {
      auto* classified = traits->FindModelTrait("classified");
      auto* outer_edge = traits->FindModelTrait("outer edge");
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
  apf::writeVtkFiles("mt-pumi", mesh);

  mesh->destroyNative();
  apf::destroyMesh(mesh);

  PCU_Comm_Free();
  MPI_Finalize();
  return 0;
}
