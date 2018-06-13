==========
Extensions
==========

The CityGML data model allows us to represent the most common city features, but sometimes practitioners may want to model extra features and/or add complex attributes to the data model.
For this, CityGML has the concept of `ADEs (Application Domain Extensions) <https://www.citygml.org/ade/>`_.
An ADE is defined in an extra XSD (XML Schema Definition) file with its own namespace, and often inheritance is used to refine the classes of the CityGML data model, but entirely new classes can be defined too.
The ADE allows us to document in a structured way, and also to validate, an instance CityGML document that would contain a both classes from the core model and from the ADEs.

-------------------
CityJSON Extensions
-------------------

CityJSON uses `JSON schemas <http://json-schema.org/>`_ to document and validate the data model, and these are less powerful and less flexible than XSDs.
Inheritance and namespaces are for instance not supported; schemas should be seen as basically validating the syntax of a JSON document.

CityJSON allows its data model to be extended with what we call *Extensions*.
These extensions are defined as JSON schemas, and thus validation is possible.
The following cases are possible with CityJSON extensions:

  1. **Adding new (complex) attributes to City Objects**
  2. **Creating a new City Object, or extending one, and define complex geometries**


1. Adding new (complex) attributes to City Objects
**************************************************










2. Creating/extending new City Objects
**************************************