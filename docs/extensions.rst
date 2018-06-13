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

One of the philosophy of JSON is "schema-less", which means that one is allowed to define whatever properties for her JSON objects.
While this is in contract to CityGML (and GML as a whole), the schemas of CityJSON are partly following that philosopy.
That is, for a given City Object, the list of "allowed" properties/attributes is listed in the schema, but it is not an error to add new ones. 
The validator of CityJSON will return a *warning* if an attribute is not in the schema, but it is allowed and valid.

Thus, if one wants to add a new attribute to a given Building, say to document its colour (``"colour": "red"``), the easiest is simply to add it in the instance file:

.. code-block:: js

  {
    "type": "Building", 
    "bbox": [ 84710.1, 446846.0, -5.3, 84757.1, 446944.0, 40.9 ],
    "attributes": { 
      "roofType": "gable",
      "colour": "red"
    },
    "geometry": [...]
  }

Observe that complex attributes (ie 'hierarchical') can be added:

.. code-block:: js

  {
    "type": "Building", 
    "bbox": [ 84710.1, 446846.0, -5.3, 84757.1, 446944.0, 40.9 ],
    "attributes": { 
      "roofType": "gable",
      "colour": {
        "facade": {
          "rgba": [255,255,255,1],
          "hex": "#000"
        },
        "roof": {
          "rgba": [0,255,0,1],
          "hex": "#0F0"
        }
      }
    },
    "geometry": [...]
  }

However, these will not be documented, nor will they be validated.
It is recommended to document complex attributes in a schema, and thus a new City Object needs to be defined; see below.



2. Creating/extending new City Objects
**************************************