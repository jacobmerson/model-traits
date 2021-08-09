# Model Traits Library

[![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)
[![Linux-build](https://github.com/jacobmerson/model-traits/actions/workflows/linux.yml/badge.svg)](https://github.com/jacobmerson/model-traits/actions/workflows/linux.yml)
[![status](https://joss.theoj.org/papers/be610090174f37749613a7c34ae2af44/status.svg)](https://joss.theoj.org/papers/be610090174f37749613a7c34ae2af44)

This is a simple library for dealing with applying simulation information onto a model very flexible way. Simulation
information may consist of boundary conditions, but can also refer to other data that may need to be queried during a
model setup. For example, the frequency that data is written to file, or material properties.

The model traits are stored in a directed acyclic graph with two types of nodes. The first is categories. Categories
allow you to define different components of your simulation data. For example, you may want to store boundary conditions
and material properties in different categories.

The second type of node in the graph is a model trait. A model trait is a piece of data that is stored on a model. This
could be a scalar, string, function, vector, etc. The model traits are stored on the leafs of the DAG.

Setting up and querying a model occurs in two phases. In the first phase, the DAG is setup in a form that is convenient
for reading and writing of the model information. In this form, the DAG is stored by category, and the leaf nodes hold
the set of geometric entities as well as the model trait data.

Often, when setting up a simulation it is most important to know what traits are applied to given geometric entities.
Therefore, in the second phase the DAG is "associated" with the geometric entities. In this form the new DAG can be
queried by geometry which holds categories, and model traits in the leaf nodes.

In summary:

1. Form the DAG either by hand or using the IO routines (see the ModelTraits class)
2. Associate the ModelTraits using the AssociatedModelTraits class
3. Query the associated model traits by geometry

## Features

- multiple simulation cases
- tensor, vector, scalar, string, int, bool, function boundary condition types
- support for runtime evaluated string boundary conditions using [Exprtk](http://www.partow.net/programming/exprtk/).
- multiple IO backends (see below)

## IO Backends

`model-traits` has been designed to easily add new IO backends. No modification of the library code is
needed to add a new backend type. The following are the currently supported backend types.

- YAML: input and output
- Simmetrix Simmodsuite: input only

## Example Usage

For examples of how this code can be used please see the [examples folder](./examples). Example YAML input files can be
found in the [inputs folder](./examples/inputs). An example Simmetrix input smd file can be
found [here](https://github.com/SCOREC/pumi-meshes/blob/master/phasta/simModelAndAttributes/translated.smd).

### Overview of library features:
1. [basic_usage.cc](examples/basic_usage.cc) provides an overview of the basic library usage. Explores just the most typical operations that you will use when working with this library.
2. [core_usage.cc](examples/core_usage.cc) more detailed view into the library functionality.
3. [pumi.cc](examples/pumi.cc) example of how the model-traits library may be used in conjunction with a pumi mesh.
4.[yaml_with_backend.cc](examples/yaml_with_backend.cc) shows how the YAML IO backend can be modified to change the input and output behavior.

### Utilities
1. [smd2yaml](examples/smd2yaml.cc) convert a smd file to a model-traits YAML file.
2. [smd2yaml_phasta](examples/smd2yaml_phasta.cc) convert a smd file to a model-traits YAML file with a custom YAML type mapping.


Please open an issue if there are more examples you would like to see.

## Installation

There are multiple supported ways to install this library with varying levels of automation.

1. Automatic: install `model-traits` with [spack](https://spack.readthedocs.io/en/latest/).
   Once spack is installed just do:

   ```bash
   spack install model-traits
   ```
   There are variants for building with Simmetrix support (`+simmetrix`) and pumi support (`+pumi`) which are off by default. There is also a variant to use the YAML backend (`+yaml`) which is on by default.

2. Semi-automatic: the CMake FetchContent utility is used to bring in most dependencies automatically. If you would like to build the pumi examples, or with the Simmetrix backend, those projects must be installed independently. For a baseline install do:

   ```bash
   git clone https://github.com/jacobmerson/model-traits.git
   cd model-traits
   # configure cmake (defaults to MODEL_TRAITS_BUILD_EXTERNAL=ON)
   cmake -S . -B build -DCMAKE_INSTALL_PREFIX=/path/to/install/location/
   # build with 4 processors
   cmake --build build -j 4
   # run the unit tests
   cmake --build build -t test
   # install model-traits into the 
   cmake --install build
   ```


3. Manual: install all of the [dependencies](#dependencies) required for your desired use case manually and add the install locations to the `CMAKE_PREFIX_PATH`. For example after installing fmtlib building without unit tests or any IO backends can be achieved by doing:

   ```bash
   git clone https://github.com/jacobmerson/model-traits.git
   cd model-traits
   # configure project to not build external libraries and point to the fmt-config.cmake files
   # alternatively, you can add the path of your intallation directory to the CMAKE_PREFIX_PATH
   # environment variable
   cmake -S . -B build -DMODEL_TRAITS_BUILD_EXTERNAL=OFF -Dfmt_DIR=/myinstalldir/lib/cmake/fmt/ -DMODEL_TRAITS_ENABLE_YAML=OFF
   # build with 4 processors
   cmake --build build -j 4
   ```

### Dependencies

1. [fmtlib](https://github.com/fmtlib/fmt) is required.
2. [Catch2 v3](https://github.com/catchorg/Catch2) is required if building with unit tests.
3. [yaml-cpp](https://github.com/jbeder/yaml-cpp) is required if building with the YAML backend.
4. [pumi](https://github.com/SCOREC/core) is required if building with the pumi examples.
5. [Simmetrix Simulation Modeling Suite](http://simmetrix.com/) is required if building with the Simmetrix backend.

## Using in a third party library
`model-traits` sets up CMake target config files for downstream CMake consumers. The easiest way to use `model-traits` in a downstream CMake library is to ensure that the install location is in your `CMAKE_PREFIX_PATH` environment variable. In your `CMakeLists.txt` you can link against the `ModelTraits::ModelTraits` target for the core library, and the `ModelTraits::IO` library to use the build in IO Backends. An example of that usage pattern can be found in the examples [`CMakeLists.txt`](examples/CMakeLists.txt) and is shown in the following snippet.

```cmake
find_package(ModelTraits REQUIRED)
add_executable(my_application main.cpp)
target_link_libraries(my_application ModelTraits::ModelTraits ModelTraits::IO)
```

## Contributing

Pull requests are welcome! Contact @jacobmerson with any questions.

Naming style comes from the [Google C++ style guide](https://google.github.io/styleguide/cppguide.html).
