# Model Traits Library

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

This software should be considered alpha quality. Currently the API should not be considered stable as I may make
changes based on community feedback.

## Features

* multiple simulation cases
* tensor, vector, scalar, string, int, bool, function boundary condition types
* support for runtime evaluated string boundary conditions using [Exprtk](http://www.partow.net/programming/exprtk/).
* multiple IO backends (see below)

## Backends

I designed the software to make it straight forward to add custom IO backends. No modification of the library code is
needed to add a new backend type. The following are the currently supported backend types.

* yaml: input and output
* simmodsuite: input only

## Example Usage

For examples of how this code can be used please see the [examples folder](./examples). Example Yaml input files can be
found in the [inputs folder](./examples/inputs). An example Simmetrix input smd file can be
found [here](https://github.com/SCOREC/pumi-meshes/blob/master/phasta/simModelAndAttributes/translated.smd).

Please open an issue if there are more examples you would like to see.

## Contributing

Pull requests are welcome! Contact @jacobmerson with any questions.