==========
Extensions
==========

.. contents:: :local:

The CityGML data model allows us to represent the most common city features, but sometimes practitioners may want to model additional features and/or add certain attributes to the data model.
For this, CityGML has the concept of `ADEs (Application Domain Extensions) <https://www.citygml.org/ade/>`_.
An ADE is defined in an extra `XML Schema <https://en.wikipedia.org/wiki/XML_schema/>`_ (XSD file) with its own namespace, and often inheritance is used to refine the classes of the CityGML data model, to define entirely new classes, and to modify any class by adding for instance new geometries and complex attribute anywhere in a City Model.
The ADE allows us to document in a structured way, and also to validate, an instance of a CityGML document that would contain both classes from the core model and from the ADEs.


-------------------
CityJSON Extensions
-------------------

CityJSON uses `JSON Schemas <http://json-schema.org/>`_ to document and validate the data model, schemas should be seen as basically validating the syntax of a JSON document.
JSON schemas are less flexible than XML Schemas: inheritance and namespaces are for instance not supported.

CityJSON nevertheless allows its data model to be extended with what we call *Extensions*.
A CityJSON Extension is a JSON file that allows to document how the core data model of CityJSON may be extended, and to validate CityJSON files.

The following 3 cases are possible:

  1. Adding new complex attributes to existing City Objects
  2. Creating a new City Object, or "extending" one, and define complex geometries
  3. Adding new properties at the root of a document

.. important::

  While Extensions are less flexible than CityGML ADEs (ie they have a narrower scope and less customisation is possible), it should be noted that the flexibility of ADEs come at a price: standard software (eg viewer or spatial analysis software) will often not be able to process correctly the files containing ADEs, because specific code needs to be written. Viewers might be working (the GML geometries are simply parsed), but software parsing the XML might not work directly). CityJSON Extensions are designed such that they can be read and processed by standard CityJSON software, often no changes in the code is required. This is achieved by enforcing a set of simple rules, as defined below, when adding new complex attributes and City Objects; if these are followed then a CityJSON files containing Extensions will be seen as 'standard' CityJSON files.



The Extension file
******************

A CityJSON Extension is defined in a separate JSON file such as this one:

.. code-block:: js

  {
    "type": "CityJSON Extension",
    "name": "Noise",
    "uri": "https://someurl.org/noise.json",
    "version": "0.1",
    "description": "Extension to model the noise"
    "extraRootProperties": {},     
    "extraAttributes": {},
    "extraCityObjects" {}
  }


It must define the name of the Extension, its URI, and its version.
The 3 cases to extend the core model, as described above, are 3 properties of the file.
Each of these properties contain snippets of `JSON Schemas <http://json-schema.org/>`_, as explained below.



1. Adding new complex attributes to existing City Objects
*********************************************************

One of the philosophy of JSON is "schema-less", which means that one is allowed to define "new" properties for the JSON objects without documenting them in a JSON schema (watch out: this does *not* mean that JSON cannot have schemas!).
While this is in contrast to CityGML (and GML as a whole where schemas are holy), the schemas of CityJSON (:doc:`schema`) are partly following that philosophy.
That is, for a given City Object, the list of "allowed" properties/attributes is listed in the schema, but it is not an error to add new ones. 
The validator of CityJSON (`cjio <https://github.com/tudelft3d/cjio>`_ with the option ``--validate``) does more than simply validate a dataset against the schemas, and will return a *warning* if an attribute is not in the schema, but it is not considered invalid in CityJSON.

In brief, if one wants to simply add a new attribute to a given ``"Building"``, say to document its colour (``"colour": "red"``), the easiest way is just to add a property to the City Object (notice that ``"storeysAboveGround"`` is in the CityJSON schema already):

.. code-block:: js

  {
    "type": "Building", 
    "attributes": { 
      "storeysAboveGround": 2,
      "colour": "red"
    },
    "geometry": [...]
  }

It is also possible to add, and document in a schema, complex attributes, for example is we wanted to have the colour as a RGBA value (red-green-blue-alpha):

.. code-block:: js

  {
    "type": "Building", 
    "attributes": { 
      "storeysAboveGround": 2,
      "+colour": {
        "rgba": [255, 255, 255, 1],
      },
    },
    "geometry": [...]
  }


Another example would be to store the area of a the parcel of a building, and to document also the unit of measurement:

.. code-block:: js

  {
    "type": "Building", 
    "attributes": { 
      "storeysAboveGround": 2,
      "+area-parcel": {
        "value": 437,
        "uom": "m2"
      } 
    },
    "geometry": [...]
  }

For this 2 cases, the CityJSON Extension file would like the snippet below.
Notice that ``"extraAttributes"`` may have several properties (the City Objects are the possibilities) and then each of these have as properties the new attributes.
This attributes must start with ``+``.
The value of the property is a JSON schema; this schema can reference and reuse JSON objects already defined in the CityJSON schemas.

.. code-block:: js

  "extraAttributes": {
    "Building": {
      "+colour": {
        "type": "object",
        "properties": {
          "rgba": {
            "type": "array",
            "items": {"type": "number"},
            "minItems": 4,    
            "maxItems": 4
          }
        },
        "required": ["rgba"],
        "additionalProperties": false
      },
      "+area-parcel": {
        "type": "object",
        "properties": {
          "value": { "type": "number" },
          "uom": { "type": "string", "enum": ["m2", "feet2"] }
        },
        "required": ["value", "uom"],
        "additionalProperties": false
      }      
    } 
  }



2. Creating/extending new City Objects
**************************************

The creation of a new City Object is done by defining it in the Extension file in the ``"extraCityObjects"`` property.
The

.. code-block:: js

  "extraCityObjects": {
    "+NoiseBuilding": {
      "allOf": [
        { "$ref": "../cityobjects.json#/_AbstractCityObject"},
        { "$ref": "../cityobjects.json#/_AbstractBuilding" },
        {
          "properties": {
            "type": { "enum": ["+NoiseBuilding"] },
            "attributes": {
              "properties": {
                "buildingLDenMin": {"type": "number"}
              }
            }
          },
          "required": ["type"]
        }
      ]
    }
  }



Since all City Objects are documented in the schemas of CityJSON (in `cityobjects.json <https://github.com/tudelft3d/cityjson/tree/master/schema>`_), it is basically a matter of copying the parts needed in a new file and modifying its content.
A new name for the City Object (for the class) must be given.
  
It should be observed that since JSON schema does not allow inheritance, the only way to extend a City Object is to define an entirely new one (with a new name, eg ``"+NoiseBuilding"``).
This is done by copying the schema of the parent City Object and extending it. 

.. admonition:: Rules to follow to define new City Objects

  The challenge is creating Extensions that will not break the software packages (viewers, spatial analysis, etc) that already read and process CityJSON.
  While one could define a new City Object and document it, if this new object doesn't follow the rules below then it will mean that new specific software needs to be built for it; this would go against the fundamental ideas behind CityJSON.

    1. The name of a new City Object must begin with a ``+``, eg ``"+NoiseBuilding"``
    2. A new City Object must conform to the rules of CityJSON, ie it must contain a property ``"type"`` and one ``"geometry"``. If the object contains appearances, the same schemes should be used so that the new City Objects can be processed by the tools without modification. 
    3. All the geometries must be in the property ``"geometry"``, and cannot be located somewhere else deep in a hierarchy of a new property. This ensures that all the code written to process, manipulate, and view CityJSON files (eg `cjio <https://github.com/tudelft3d/cjio>`_ and `azul <https://github.com/tudelft3d/azul>`_) will be working without modifications. 
    4. If a new City Object needs to store more geometries (see below for an example), then a new City Object needs to be defined using the same structure of parent-children, as used by ``"Building"`` and ``"BuildingPart"``.
    5. The reuse of types defined in CityJSON, eg ``"Solid"`` or semantic surfaces, is allowed.
    6. To define new semantic surfaces, simply add a ``+`` to its name, eg ``"+ThermalSurface"``.


3. Adding new properties at the root of a document
**************************************************

It is allowed to add new properties at the root of a CityJSON file, but if you want to document them in a schema this property needs to start with a ``+``


.. code-block:: js

  "extraRootProperties": {
    "+extra": {
      "type": "object",
      "properties": {
        "un": { "type": "integer"},
        "deux": { "type": "integer"},
        "trois": { "type": "integer"}
      },
      "required": ["un", "deux", "trois"],
      "additionalProperties": false
    }
  }





------------------------------------------------
Mapping of the Noise ADE to a CityJSON Extension
------------------------------------------------

To illustrate the process of creating a new CityJSON extension, we use the Noise ADE, which is the example case in the `CityGML 2.0 documentation <https://portal.opengeospatial.org/files/?artifact_id=47842>`_ (Section 10.13.2 on p. 151 describes it; and Annex H on p. 305 gives more implementation details).
The XSDs and some test datasets are available `here <http://schemas.opengis.net/citygml/examples/2.0/ade/noise-ade/>`_.

The resulting files for the Noise Extension are available:
  - :download:`download noise.json <../schema/v08/extensions/noise.json>`
  - :download:`download noise_data.json <../example-datasets/extensions/noise_data.json>`


Adding new attributes to a Building
***********************************

.. image:: _static/noise_building.png
   :width: 60%

To add these attributes (they are not complex, but for the sake of the exercise let us assume that they are) one needs to:

  1. Define in a new schema file two new City Objects: ``"+NoiseBuilding"`` and ``"+NoiseBuildingPart"`` 
  2. Copy the schemas of ``"Building"`` and ``"BuildingPart"``, `defined in this file <https://github.com/tudelft3d/cityjson/blob/master/schema/v07/cityobjects.json>`_
  3. Extend these schemas and add a new property ``"noise-attributes"``. The new attributes could have been simply added to the list of ``"attributes"`` too.


.. code-block:: js

  "+NoiseBuilding": {
      "type": "object",
      "properties": {
        "type": { "enum": ["+NoiseBuilding"] },
        "attributes": ...
        "noise-attributes": {
          "buildingReflection": {"type": "string"},
          "buildingReflectionCorrection": {"type": "number"},
          "buildingLDenMax": {"type": "number"},
          "buildingLDenMin": {"type": "number"},
          "buildingLNightMax": {"type": "number"},
          "buildingLNightMin": {"type": "number"},
          "buildingLDenEq": {"type": "number"},
          "buildingLNightEq": {"type": "number"},
          "buildingHabitants": {"type": "integer"},
          "buildingImmissionPoints": {"type": "integer"},
          "remark": {"type": "string"}
        }
        ...


A CityJSON file containing this new City Object would look like this:

.. code-block:: js

  {
    "type": "CityJSON",
    "version": "0.8",
    "extensions": {
      "+NoiseBuilding": "https://someurl.org/noise.json" 
    },
    "CityObjects": {
      "1234": {
        "type": "+NoiseBuilding",
        "geometry": [
          {
            "type": "Solid",
            "lod": 2,
            "boundaries": [
              [ [[0, 3, 2, 1]], [[4, 5, 6, 7]], [[0, 1, 5, 4]], [[1, 2, 6, 5]], [[2, 3, 7, 6]], [[3, 0, 4, 7]] ] 
            ]
          }
        ],
        "attributes": {
          "roofType": "pointy"
        },
        "noise-attributes": {
          "buildingReflectionCorrection": 234,
          "buildingLNightMax": 17.33
        }
      },


Adding complex types for CityFurniture
**************************************

.. image:: _static/noise_cf.png
   :width: 80%

As it can be seen in the UML diagram, extending ``"CityFurniture"`` is more challenging because not only new simple attributes need to be defined, but a ``"CityFurniture"`` object can contain several ``"NoiseCityFurnitureSegment"``, which have their own geometry (a 'gml:Curve'). 


The steps to follow are thus:

  1. Create 2 new City Objects: ``"+NoiseCityFurniture"`` and ``"+NoiseCityFurnitureSegment"``
  2. ``"+NoiseCityFurniture"`` can be copied from ``"CityFurniture"``, and we need to add a new property ``"children"`` which contains a list of the IDs of the segments. This is similar to what is done for ``"BuildingParts"`` and ``"BuildingIntallations"``: each City Object has its own geometries, and they are linked together with this simple method.
  3. ``"+NoiseCityFurnitureSegment"`` is a new City Object and it gets the attributes common to all City Objects, and its geometry is restricted to a ``"MultiLineString"``. It also gets one property ``"parent"`` which links to its parent ``"+NoiseCityFurniture"``.

.. code-block:: js

  "+NoiseCityFurniture": {
    "type": "object",
    "properties": {
      "type": { "enum": ["+NoiseCityFurniture"] },
      ...
      "children": {
        "type": "array",
        "description": "the IDs of the +NoiseCityFurnitureSegment",
        "items": {"type": "string"}
      }
      ...
  }

.. code-block:: js

  "+NoiseCityFurnitureSegment": {
    "type": "object",
    "properties": {
      "type": { "enum": ["+NoiseCityFurnitureSegment"] },
      "attributes": {
        ...
      },
      "parent": { "type": "string" },
      "geometry": {
        "type": "array",
        "items": {
          "oneOf": [
            {"$ref": "../geomprimitives.json#/MultiLineString"}
          ]
        }
      }
    },
    "required": ["type", "geometry", "parent"],
    "additionalProperties": false
  }


.. code-block:: js

  "a_noisy_bench": {
    "type": "+NoiseCityFurniture",
    "geometry": [
      {
        "type": "Solid",
        "lod": 2,
        "boundaries": [
          [ [[0, 3, 2, 1]], [[4, 5, 6, 7]], [[0, 1, 5, 4]], [[1, 2, 6, 5]], [[2, 3, 7, 6]], [[3, 0, 4, 7]] ] 
        ]
      }
    ],
    "children": ["thesegment_1", "thesegment_2"]
  },
  "thesegment_1": {
    "type": "+NoiseCityFurnitureSegment",
    "geometry": [
      {
        "type": "MultiLineString",
        "lod": 0,
        "boundaries": [
          [2, 3, 5], [77, 55, 212]
        ]
      }      
    ],
    "parent": "a_noisy_bench",
    "attributes": {
      "reflectionCorrection": 2.33
    }
  }    


-----------------------------------------
Validation of files containing extensions
-----------------------------------------

The validation of a CityJSON file containing extensions needs to be performed as a 2-step operation:
  1. The standard validation of all City Objects (except the new ones; those starting with ``"+"`` are ignored at this step); 
  2. Each City Object defined in the Extensions is (individually) validated against its schema defined in the new schema file.

While this could be done with any JSON schema validator, resolving all the JSON references could be slightly tricky. 
Thus, `cjio <https://github.com/tudelft3d/cjio>`_ (with the option ``--validate``) has automated this process. 
You just need to add the new schemas in the folder ``/extensions`` in the ``schema/v08/`` folder; ``noise.json`` is already present in the `GitHub repository of CityJSON <https://github.com/tudelft3d/cityjson/tree/master/schema/v08>`_.
Then specify the folder where the schemas are with the option ``--folder_schemas``.
  
.. code-block:: bash

  $ cjio noise_data.json validate --folder_schemas /home/elvis/cityjson/schema/v08/

This assumes that the folder is structured as follows:

.. code-block:: console

  appearance.json
  cityjson.json
  cityobjects.json
  extensions
  geomprimitives.json
  geomtemplates.json
  metadata.json
  extensions/
      noise.json
