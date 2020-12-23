# Boundary Conditions

This is a simple library for dealing with the application of simulation boundary conditions
in a very flexible way. The goal of this library is to easily allow the addition of
multiple read-write backends.


**Currently this library is not ready to be used, and is under development.**

## Intended Features
* multiple boundary condition cases
* tensor, scalar, string boundary condition types
* boundary condition categories i.e. can have a solution method category which has Newton's method,
  which has multiple parameters
* String boundary conditions can be expressions which are evaluated at runtime using exprtk

## Backends
* yaml: under development
* simmodsuite: under developement, however due to licensing restrictions
  this backend will be kept in a separate location. If you are interested
  in using the Simmetrix backend, please contact me (@jacobmerson).
