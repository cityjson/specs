
=======================
CityJSON specifications
=======================

.. highlights::
  (version |release|)


.. contents:: :local:


---------------
CityJSON Object
---------------

A CityJSON object represents one 3D city model of a given area, this model may contain features of different types, as defined in the CityGML data model.

- A CityJSON object is a JSON object.
- A CityJSON object must have 4 members: 

  #. one member with the name ``"type"``, whose value must be ``"CityJSON"``;
  #. one member with the name ``"version"``, whose value must be a string with the version (X.Y) of CityJSON used;
  #. one member with the name ``"CityObjects"``. The value of this member is a collection of key-value pairs, where the key is the ID of the object, and the value is one City Object. The ID of a City Object should be unique (within one dataset/file).
  #. one member with the name ``"vertices"``, whose value is an array of coordinates of each vertex of the city model. Their position in this array (0-based) is used as an index to be referenced by the Geometric Objects. The indexing mechanism of the format `Wavefront OBJ <https://en.wikipedia.org/wiki/Wavefront_.obj_file>`_ is basically reused.


- A CityJSON may have one member with the name ``"metadata"``, whose value may contain JSON objects describing the coordinate reference system used, the extent of the dataset, its creator, etc.
- A CityJSON may have one member with the name ``"transform"``, whose value must contain 2 JSON objects describing how to *decompress* the coordinates. Transform is used to reduce the file size only.
- A CityJSON may have one member with name ``"appearance"``, the value may contain JSON objects representing the textures and/or materials of surfaces.
- A CityJSON must not have other members.

The minimal valid CityJSON object is thus:

.. code-block:: js

  {
    "type": "CityJSON",
    "version": "0.5",
    "CityObjects": {},
    "vertices": []
  }


An empty CityJSON will look like this:

.. code-block:: js

  {
    "type": "CityJSON",
    "version": "0.3",
    "metadata": {},
    "transform": {
      "scale": [],
      "translate": []
    },
    "CityObjects": {},
    "vertices": [],
    "appearance": {}
  }

.. note::
  While the order of the member values of a CityJSON should preferably be as above, not all JSON generators allow one to do this, thus the order is not prescribed.

--------
Metadata
--------

The metadata related to the 3D city model may be stored in a JSON object that may have different members, as follows.
The mandatory members in `ISO19115 <https://www.iso.org/standard/53798.html>`_ are used, and a few are added (eg ``copyright``, and ``presentLoDs`` because they are useful in 3D in a city modelling context).


``"crs"`` (Coordinate reference system)
***************************************
The CRS may be given with a JSON object that must contain one member ``"epsg"`` with as value the `EPSG code <https://epsg.io>`_, as an integer.
For instance, for the WGS84 latitude-longitude:

.. code-block:: js

  "metadata": {
    "crs": { 
      "epsg": 7415
    }
  }


Be aware that the EPSG code should be a three-dimensional CRS, ie the elevation/height values should be with respect to a specific datum.
It is not possible to give a WKT string with the parameters, or any other way.

.. note::
  Unlike in (City)GML where each object can have a different CRS (eg a wall of a building could theoretically have a different from the other walls used to represent the building), in CityJSON all the city objects need to be in the same CRS.


``"bbox"`` (extent of the dataset)
**********************************
While this can be extracted from the dataset itself, it is useful to store it. 
It may be stored as an array with 6 values: [minx, miny, minz, maxx, maxy, maxz]

.. code-block:: js

  "metadata": {
    "bbox": [ 84710.1, 446846.0, -5.3, 84757.1, 446944.0, 40.9 ]
  }


``"keywords"``
**************
An array of keywords of type ``"string"`` may be listed:

.. code-block:: js

  "metadata": {
    "keywords": ["energy", "solar potential"]
  }


Geographic Location
*******************
The name of an area or a city.

.. code-block:: js

  "metadata": {
    "geographicLocation": "TU Delft campus"
  }


Dataset Topic Category
**********************
A one-word category, the possible values are enumerated in the Table B.3.30 of the `ISO19115-1:2014 document <https://www.iso.org/standard/53798.html>`_

.. code-block:: js

  "metadata": {
    "datasetTopicCategory": "planningCadastre"
  }


Present LoDs (levels-of-detail)
*******************************
An array of all the LoDs present in the file.

.. code-block:: js

  "metadata": {
    "presentLoDs": ["1.2", "2.2", "2.3"]
  }


Other properties
****************
The following are all of type ``"string"``:

- ``"datasetTitle"``
- ``"datasetReferenceDate"``
- ``"datasetLanguage"``
- ``"datasetAbstract"``
- ``"metadataDateStamp"``
- ``"pointOfContact"``
- ``"copyright"``

.. note::
  
  It should be noticed that JSON does not have a date type, thus for all dates in a CityJSON document the following should be used: ``"YYYY-MM-DD"`` (as a string).


.. _specs_cityobjects:

-----------------
City Object types
-----------------

A City Object is a JSON object for which the type member’s value is one of the following (of type string):

  #. ``"Building"``
  #. ``"BuildingPart"``
  #. ``"BuildingInstallation"``
  #. ``"Road"``
  #. ``"Railway"``
  #. ``"TransportSquare"``
  #. ``"TINRelief"``
  #. ``"WaterBody"``
  #. ``"PlantCover"``
  #. ``"SolitaryVegetationObject"``
  #. ``"LandUse"``
  #. ``"CityFurniture"``
  #. ``"GenericCityObject"``

A City Object:

- must have one member with the name ``"geometry"``, whose value is an array containing 0 or more Geometry Objects. More than one Geometry Object is used to represent several different levels-of-detail (LoDs) for the same object. However, the different Geometry Objects of a given City Object do not have be of different LoDs.
- may have one member with the name ``"attributes"``, whose value is an object with the different attributes allowed by CityGML. 

.. code-block:: js

  "CityObjects": {
    "id-1": {
      "type": "Building", 
      "attributes": { 
        "measuredHeight": 22.3,
        "roofType": "gable",
        "owner": "Elvis Presley"
      },
      "geometry": [{...}]
    },
    "id-2": {
      "type": "PlantCover", 
      ...
    }
  }

Attributes
**********

The attributes prescribed by CityGML differ per City Object, and can be seen either in the `official CityGML documentation <https://portal.opengeospatial.org/files/?artifact_id=47842>`_ or in the schema of CityJSON (:doc:`schema`). 
The program `cityjson_valschema <https://github.com/tudelft3d/cityjson/tree/master/software/cityjson-valschema>`_ returns WARNINGS when a City Object has attributes not in the CityGML list.
In CityJSON any other attributes can be added with a JSON key-value pair ("owner" in the example above is one such attribute)---it is however not guaranteed that a parser will read them.

All the City Objects have the following 3 possible attributes:
  - ``"class"``
  - ``"function"``
  - ``"usage"``

While CityGML does not prescribe the values for these, the `SIG 3D maintains a codelist <http://www.sig3d.de/codelists/standard/>`_ that can be used.
In CityJSON, as can be seen in the schema, the values should be a string, thus either the name of the values should be used, or the code as a string:

.. code-block:: js

  "CityObjects": {
    "id-1": {
      "type": "LandUse", 
      "attributes": { 
        "function": "Industry and Business"
      },
      "geometry": [{...}]
    },
    "id-2": {
      "type": "WaterBody", 
      "attributes": { 
        "class": "1010"
      },
      "geometry": [{...}]
    }
  }



Building, BuildingPart, and BuildingInstallation
************************************************

- A City Object of type ``"Building"`` may have a member ``"Parts"``, whose value is an array of the IDs of the City Objects of type ``"BuildingPart"`` it contains.
- A City Object of type ``"BuildingPart"`` must have a parent ``"Building"`` referencing it, however, unlike in CityGML, a ``"BuildingPart"`` cannot be decomposed into a ``"BuildingPart"``.
- A City Object of type ``"Building"`` or ``"BuildingPart"`` may have a member ``"Installations"``, whose value is an array of the IDs of the City Objects of type ``"BuildingInstallation"`` it contains.
- A City Object of type ``"BuildingInstallation"`` must have a parent ``"Building"`` referencing it.
- The geometry of both ``"Building"`` and ``"BuildingPart"`` can only be represented with these Geometry Objects: (1) ``"Solid"``, (2) ``"CompositeSolid"``, (3) ``"MultiSurface"``.
- The geometry of a ``"BuildingInstallation"`` object can be represented with any of the Geometry Objects.
- A City Object of type ``"Building"`` or ``"BuildingPart"`` may have a member ``"address"``, whose value is a JSON object describing the address. One location (a ``"MultiPoint"``) can be given, to for instance locate the front door inside the building.

.. code-block:: js

  "CityObjects": {
    "id-1": {
      "type": "Building", 
      "attributes": { 
        "roofType": "gable"
      },
      "Parts": ["id-56", "id-832"],
      "Installations": ["mybalcony"]
    },
    "id-56": {
      "type": "BuildingPart", 
      ...
    },
    "mybalcony": {
      "type": "BuildingInstallation", 
      ...
    }
  }

.. code-block:: js

  {
    "type": "Building", 
    "address": {
      "CountryName": "Canada",
      "LocalityName": "Chibougamau",
      "ThoroughfareNumber": "1",
      "ThoroughfareName": "rue de la Patate",
      "PostalCode": "H0H 0H0"
    },
  }


Transportation: Road, Railway, and TransportSquare 
**************************************************

CityJSON uses 3 classes related to transportation (``"Road"``, ``"Railway"``, ``"TransportSquare"``) and omits the "Track" from CityGML because it simply can be a road with specific attributes.
``"TransportSquare"`` is used to model for instance parking lots and squares.

In CityGML, each of the 3 classes can have a number of "TrafficArea" and "AuxiliaryTrafficArea", which are defined as new surfaces. 
In CityJSON, these surfaces do not need to be defined again since the road surfaces become Semantic Surface Objects (with type ``"TrafficArea"`` or ``"AuxiliaryTrafficArea"``).
That is, the surface representing a road should be split into sub-surfaces (therefore forming a ``"MultiSurface"``), and each of the sub-surfaces get a semantics attached to it.

- The geometry of a City Object of type ``"Road"``, ``"Railway"``, ``"TransportSquare"`` can be of types ``"MultiSurface"`` or ``"CompositeSurface"``.
- The geometry of a City Object of type ``"Road"``, ``"Railway"``, ``"TransportSquare"`` cannot be of ``"lod"`` 0, only 1 and above are allowed.

.. code-block:: js

  "ma_rue": {
    "type": "Road", 
    "geometry": [{
      "type": "MultiSurface",
      "lod": 2,
      "boundaries": [
         [[0, 3, 2, 1, 4]], [[4, 5, 6, 666, 12]], [[0, 1, 5]], [[20, 21, 75]]
      ]
    }],
    "semantics": {
      "surfaces": [
        {
          "type": "TrafficArea",
          "surfaceMaterial": ["asphalt"],
          "function": "road"
        },
        {
          "type": "AuxiliaryTrafficArea",
          "function": "green areas"
        },
        {
          "type": "TrafficArea",
          "surfaceMaterial": ["dirt"],
          "function": "road"
        }
      ],
      "values": [0, 1, null, 2]
    }
  }



TINRelief
*********

- The geometry of a City Object of type ``"TINRelief"`` can only be of type ``"CompositeSurface"``.
- CityJSON does not define a specific Geometry Object for a TIN (triangulated irregular network), it is simply a CompositeSurface for which every surface is a triangle (thus a polygon having 3 vertices, and no interior ring).

.. code-block:: js

  "myterrain01": {
    "type": "TINRelief", 
    "geometry": [{
      "type": "CompositeSurface",
      "lod": 2,
      "boundaries": [
         [[0, 3, 2]], [[4, 5, 6]], [[0, 1, 5]], [[1, 2, 6]], [[2, 3, 7]], [[3, 0, 4]]
      ]
    }]    
  }


WaterBody
*********

- The geometry of a City Object of type ``"WaterBody"`` can be of types: ``"MultiLineString"``, ``"MultiSurface"``, ``"CompositeSurface"``, ``"Solid"``, or ``"CompositeSolid"``.

.. code-block:: js

  "mygreatlake": {
    "type": "WaterBody", 
    "attributes": {
      "usage": "leisure",
    },
    "geometry": [{
      "type": "Solid",
      "lod": 2,
      "boundaries": [
        [ [[0, 3, 2, 1]], [[4, 5, 6, 7]], [[0, 1, 5, 4]] ]
      ]
    }]    
  }               


LandUse
*******

- The geometry of a City Object of type ``"LandUse"`` can be of type ``"MultiSurface"`` or ``"CompositeSurface"``.

.. code-block:: js

  "oneparcel": {
    "type": "LandUse", 
    "geometry": [{
      "type": "MultiSurface",
      "lod": 1,
      "boundaries": [
        [[0, 3, 2, 1]], [[4, 5, 6, 7]], [[0, 1, 5, 4]]
      ]
    }]    
  }


PlantCover
**********

- The geometry of a City Object of type ``"PlantCover"`` can be of type ``"MultiSurface"`` or ``"MultiSolid"``.

.. code-block:: js

  "plants": {
    "type": "PlantCover", 
    "attributes": { 
      "averageHeight": 11.05
    },
    "geometry": [{
      "type": "MultiSolid",
      "lod": 2,
      "boundaries": [
        [
          [ [[0, 3, 2, 1]], [[4, 5, 6, 7]], [[0, 1, 5, 4]], [[10, 13, 22, 31]] ]
        ],
        [
          [ [[5, 34, 31, 12]], [[44, 54, 62, 74]], [[10, 111, 445, 222]], [[111, 123, 922, 66]] ]
        ]  
      ]
    }]    
  }


SolitaryVegetationObject
************************

- The geometry of a City Object of type ``"SolitaryVegetationObject"`` can be any of the following: ``"MultiPoint"``, ``"MultiLineString"``, ``"MultiSurface"``, ``"CompositeSurface"``, ``"Solid"``, or ``"CompositeSolid"``.

.. code-block:: js

  "onebigtree": {
    "type": "SolitaryVegetationObject", 
    "attributes": { 
      "trunkDiameter": 5.3,
      "crownDiameter": 11.0
    },
    "geometry": [{
      "type": "MultiPoint",
      "lod": 0,
      "boundaries": [1]
    }]
  }

.. note::
  The concept of Implicit Geometries, as defined in CityGML, is not supported. An implicit geometry is a template, eg of certain species of a tree, that can be reused with different parameters to define its appearance.


CityFurniture
*************

- The geometry of a City Object of type ``"CityFurniture"`` can be any of the following: ``"MultiPoint"``, ``"MultiLineString"``, ``"MultiSurface"``, ``"CompositeSurface"``, ``"Solid"``, or ``"CompositeSolid"``.

.. code-block:: js

  "stop": {
    "type": "CityFurniture", 
    "attributes": { 
      "function": "bus stop"
    },
    "geometry": [{
      "type": "MultiSurface",
      "lod": 2,
      "boundaries": [
        [[0, 3, 2, 1]], [[4, 5, 6, 7]], [[0, 1, 5, 4]]
      ]
    }]
  }


GenericCityObject
*****************

- The geometry of a City Object of type ``"GenericCityObject"`` can be any of the following: ``"MultiPoint"``, ``"MultiLineString"``, ``"MultiSurface"``, ``"CompositeSurface"``, ``"Solid"``, or ``"CompositeSolid"``.

.. code-block:: js

  "whatisthat": {
    "type": "GenericCityObject", 
    "attributes": { 
      "usage": "it's not clear"
    },
    "geometry": [{
      "type": "CompositeSurface",
      "lod": 1,
      "boundaries": [
        [[0, 3, 2, 1]], [[4, 5, 6, 7]], [[0, 1, 5, 4]]
      ]
    }]
  }



----------------
Geometry Objects
----------------

CityJSON defines the following 3D geometric primitives, ie all of them are embedded in 3D space (and therefore their vertices have *(x, y, z)* coordinates). 
The indexing mechanism of the format `Wavefront OBJ <https://en.wikipedia.org/wiki/Wavefront_.obj_file>`_ is reused, ie a geometry does not store the locations of its vertices, but points to a vertex in a list (in the CityJSON member object ``"vertices"``).

As is the case in CityGML, only linear and planar primitives are allowed (no curves or parametric surfaces for instance).

A Geometry object is a JSON object for which the type member’s value is one of the following:

#. ``"MultiPoint"``
#. ``"MultiLineString"``
#. ``"MultiSurface"``
#. ``"CompositeSurface"``
#. ``"Solid"``
#. ``"MultiSolid"``
#. ``"CompositeSolid"``


A Geometry object:

  - must have one member with the name ``"lod"``, whose value is a number identifying the level-of-detail (LoD) of the geometry. This can be either an integer (following the CityGML standards), or a number following the `improved LoDs by TU Delft <https://www.citygml.org/ongoingdev/tudelft-lods/>`_
  - must have one member with the name ``"boundaries"``, whose value is a hierarchy of arrays (the depth depends on the Geometry object) with integers. An integer refers to the index in the ``"vertices"`` array of the CityJSON object, and it is 0-based (ie the first element in the array has the index "0", the second one "1").
  - may have one member ``"semantics"``, whose value is a hierarchy of nested arrays (the depth depends on the Geometry object). The value of each entry is a string, and the values allowed are depended on the CityObject (see below).
  - may have one member ``"material"``, whose value is a hierarchy of nested arrays (the depth depends on the Geometry object). The value of each entry is an integer referring to the material used (see below).
  - may have one member ``"texture"``, whose value is a hierarchy of nested arrays (the depth depends on the Geometry object). The value of each entry is explained below.


.. note::

  There is **no** Geometry Object for MultiGeometry. 
  Instead, for the ``"geometry"`` member of a CityObject, the different geometries may be enumerated in the array (all with the same value for the member ``"lod"``).


The coordinates of the vertices
*******************************

A CityJSON must have one member named ``"vertices"``, whose value is an array of coordinates of each vertex of the city model. 
Their position in this array (0-based) is used to represent the Geometric Objects.

  - one vertex must be an array with exactly 3 values, representing the *(x,y,z)* location of the vertex.
  - the array of vertices may be empty.
  - vertices may be repeated


.. code-block:: js

  "vertices": [
    [0.0, 0.0, 0.0],
    [1.0, 0.0, 0.0],
    [0.0, 0.0, 0.0],
    ...
    [1.0, 0.0, 0.0],
    [8523.134, 487625.134, 2.03]
  ]


Arrays to represent boundaries
******************************

- A ``"MultiPoint"`` has an array with the indices of the vertices; this array can be empty.
- A ``"MultiLineString"`` has an array of arrays, each containing the indices of a LineString
- A ``"MultiSurface"``, or a ``"CompositeSurface"``, has an array containing surfaces, each surface is modelled by an array of array, the first array being the exterior boundary of the surface, and the others the interior boundaries.
- A ``"Solid"`` has an array of shells, the first array being the exterior shell of the solid, and the others the interior shells. Each shell has an array of surfaces, modelled in the exact same way as a MultiSurface/CompositeSurface.
- A ``"MultiSolid"``, or a ``"CompositeSolid"``, has an array containing solids, each solid is modelled as above.

.. note::

  JSON does not allow comments, the comments in the example below (C++ style: ``//-- my comments``) are only to explain the cases, and should be removed

.. code-block:: js

  {
    "type": "MultiPoint",
    "boundaries": [2, 44, 0, 7]
  }

.. code-block:: js

  {
    "type": "MultiLineString",
    "boundaries": [
      [2, 3, 5], [77, 55, 212]
    ]  
  }


.. code-block:: js

  {
    "type": "MultiSurface",
    "boundaries": [
      [[0, 3, 2, 1]], [[4, 5, 6, 7]], [[0, 1, 5, 4]]
    ]
  }

.. code-block:: js

  {
    "type": "Solid",
    "boundaries": [
      [ [[0, 3, 2, 1, 22]], [[4, 5, 6, 7]], [[0, 1, 5, 4]], [[1, 2, 6, 5]] ], //-- exterior shell
      [ [[240, 243, 124]], [[244, 246, 724]], [[34, 414, 45]], [[111, 246, 5]] ] //-- interior shell
    ]
  }

.. code-block:: js

  {
    "type": "CompositeSolid",
    "boundaries": [
      [ //-- 1st Solid
        [ [[0, 3, 2, 1, 22]], [[4, 5, 6, 7]], [[0, 1, 5, 4]], [[1, 2, 6, 5]] ],
        [ [[240, 243, 124]], [[244, 246, 724]], [[34, 414, 45]], [[111, 246, 5]] ]
      ],
      [ //-- 2st Solid
        [ [[666, 667, 668]], [[74, 75, 76]], [[880, 881, 885]], [[111, 122, 226]] ] 
      ]    
    ]
  }


.. _specs_semantics:


Semantic Surface Object
***********************

A Semantics Surface is a JSON object representing the semantics of a surface, and may also represent other attributes of the surface (eg the slope of the roof or the solar potential).
A Semantic Object:
  
  - must have one member with the name ``"type"``, whose value is one of the allowed value. These depend on the City Object, see below.
  - may have other attributes in the form of a JSON key-value pair, where the value must not be a JSON object (but a string/number/integer/boolean). 

.. code-block:: js

  {
    "type": "RoofSurface",
    "slope": 16.4,
    "solar-potential": 5
  }

----

.. rubric:: Values for Semantics

``"Building"``, ``"BuildingPart"``, and ``"BuildingInstallation"`` can have the following semantics for (LoD0 to LoD3; LoD4 is omitted):


  * ``"RoofSurface"`` 
  * ``"GroundSurface"`` 
  * ``"WallSurface"``
  * ``"ClosureSurface"``
  * ``"OuterCeilingSurface"``
  * ``"OuterFloorSurface"``
  * ``"Window"``
  * ``"Door"``

For ``"WaterBody"``:

  * ``"WaterSurface"``
  * ``"WaterGroundSurface"``
  * ``"WaterClosureSurface"``

For Transportation (``"Road"``, ``"Railway"``, ``"TransportSquare"``):

  * ``"TrafficArea"``
  * ``"AuxiliaryTrafficArea"``

----

Because in one given City Object (say a ``"Building"``) several surfaces can have the same semantics (think of a complex that has been triangulated, there can be dozens of triangles used to model the same surface), the Semantic Surfaces have to be declared once and each of the surfaces used to represent it point to it.
This is achieved by first declaring all the Semantic Surfaces in array, and then having an array where each surface in the is linked to the position of the Semantic Surfaces array.

A Geometry object:

  - may have one member with the name ``"semantics"``, whose values are two keys ``"surfaces"`` and ``"values"``. Both have to be present.
  -  the value of ``"surfaces"`` is an array of Semantic Surface Objects.
  -  the value of ``"values"`` is a hierarchy of arrays (the depth depends on the Geometry object; it is two less than the array ``"boundaries"``) with integers. An integer refers to the index in the ``"surfaces"`` array of the same geometry, and it is 0-based. If one surface has no semantics, a value of ``null`` must be used.

.. code-block:: js

  {
    "type": "MultiSurface",
    "lod": 2,
    "boundaries": [
      [[0, 3, 2, 1]], [[4, 5, 6, 7]], [[0, 1, 5, 4], [[0, 2, 3, 8], [[10, 12, 23, 48]]
    ],
    "semantics": {
      "surfaces" : [
        {
          "type": "RoofSurface",
          "slope": 33.4
        }, 
        {
          "type": "RoofSurface",
          "slope": 66.6
        },
        {
          "type": "GroundSurface"
        }
      ],
      "values": [0, 0, null, 1, 2]
    },
  }

.. note::
   A ``null`` value is used to specify that a given surface has no semantics, but to avoid having arrays filled with ``null``, it is also possible to specify ``null`` for a shell or a whole Solid in a MultiSolid, the ``null`` propagates to the nested arrays.

   .. code-block:: js
     
     {
        "type": "CompositeSolid",
        "lod": 2,
        "boundaries": [
          [ //-- 1st Solid
            [ [[0, 3, 2, 1, 22]], [[4, 5, 6, 7]], [[0, 1, 5, 4]], [[1, 2, 6, 5]] ]
          ],
          [ //-- 2nd Solid
            [ [[666, 667, 668]], [[74, 75, 76]], [[880, 881, 885]], [[111, 122, 226]] ] 
          ]    
        ],
        "semantics": {
          "surfaces" : [
            {      
              "type": "RoofSurface",
            }, 
            {
              "type": "WallSurface",
            }
          ],
          "values": [
            [ //-- 1st Solid
              [0, 1, 1, null]
            ],
            [ //-- 2nd Solid get all null values
              null
            ]
          ]
        }
      }  


----------------------------------------------
Transform Object (to compress a CityJSON file)
----------------------------------------------

To reduce the size of a file, it is possible to represent the coordinates of the vertices with integer values, and store the scale factor and the translation needed to obtain the original coordinates (stored with floats/doubles).
To use compression, a CityJSON object may have one member ``"transform"``, whose values are 2 mandatory JSON objects (``"scale"`` and ``"translate"``), both arrays with 3 values.

The `scheme of TopoJSON (called quantization) <https://github.com/topojson/topojson-specification/blob/master/README.md#212-transforms>`_ is reused, and here we simply add a third coordinate because our vertices are embedded in 3D space.

If a CityJSON object has a member ``"transform"``, to obtain the real position of a given vertex *v*, we must take the 3 values *vi* listed in the "vertices" member and::

    v[0] = (vi[0] * ["transform"]["scale"][0]) + ["transform"]["translate"][0]
    v[1] = (vi[1] * ["transform"]["scale"][1]) + ["transform"]["translate"][1]
    v[2] = (vi[2] * ["transform"]["scale"][2]) + ["transform"]["translate"][2]

If the CityJSON file does not have a ``"transform"`` member, then the values of the vertices must be read as-is.

The program `cityjson-compress <https://github.com/tudelft3d/cityjson/tree/master/software/cityjson-compress/>`_ compresses a given file by: (1) merging duplicate vertices; (2) convert coordinates to integer. 
Both operation use a tolerance, which is given as number-of-digits-after-the-dot-to-preserve.


.. code-block:: js

  "transform": {
      "scale": [0.01, 0.01, 0.01],
      "translate": [4424648.79, 5482614.69, 310.19]
  }


-----------------
Appearance Object
-----------------

Both textures and materials are supported, and the same mechanisms as CityGML are used for these, so the conversion back-and-forth should be easy.
The material is represented with the `X3D <http://www.web3d.org/documents/specifications/19775-1/V3.2/Part01/components/shape.html#Material>`_ specifications, as is the case for CityGML.
For the texture, the COLLADA is reused, as is the case for CityGML.
However:
  - the CityGML class ``GeoreferencedTexture`` is not supported. 
  - the CityGML class ``TexCoordGen`` is not supported, ie one must specify the UV coordinates in the texture files.
  - texture files have to be local and put in folder named ``"appearances"`` located in the same folder as the CityJSON file (thus requests to web services as is the case with CityGML are not supported).
  - the major difference is that in CityGML each Material/Texture object keeps a list of the primitives using it, while in CityJSON it is the opposite: if a primitive has a Material/Texture than it is stated with the primitive (with a link to it).

An Appearance Object is a JSON object that
  - may have one member with the name ``"materials"``, whose value is an array of Material Objects.
  - may have one member with the name ``"textures"``, whose value is an array of Texture Objects.
  - may have both ``"materials"`` and ``"textures"``.
  - may have one member with the name ``"vertex-texture"``, whose value is an array of coordinates of each so-called UV vertex of the city model.
  - may have one member with the name ``"default-theme-texture"``, whose value is the name of the default theme for the appearance (a string). This can be used if geometries have more than one textures, so that a viewer displays the default one.
  - may have one member with the name ``"default-theme-material"``, whose value is the name of the default theme for the material (a string). This can be used if geometries have more than one textures, so that a viewer displays the default one.
  
        
.. code-block:: js

  "appearance": {
    "materials": [],
    "textures":[],
    "vertices-texture": [],
    "default-theme-texture": "myDefaultTheme1",
    "default-theme-material": "myDefaultTheme2"
  }


Geometry Object having material(s)
**********************************

Each surface in a Geometry Object can have one or more materials assigned to it.
To store these, a Geometry Object may have a member ``"material"``, the value of this member is a collection of key-value pairs, where the key is the *theme* of the material, and the value is one JSON object that must contain either:

  * one member ``"values"``, whose value is a hierarchy of arrays with integers. Each integer refers to the position (0-based) in the ``"materials"`` member of the ``"appearance"`` member of the CityJSON object. If a surface has no material, then ``null`` should be used in the array. The depth of the array depends on the Geometry object, and is equal to the depth of the ``"boundary"`` array minus 2, because each surface (``[[]]``) gets one material.
  * one member ``"value"``, whose value is one integer referring to the position (0-based) in the ``"materials"`` member of the ``"appearance"`` member of the CityJSON object. This is used because often the materials are used to colour full objects, and repetition of materials is not necessary.

In the following, the Solid has 4 surfaces, and there are 2 themes: "irradiation" and "irradiation-2" could for instance represent different colours based on different scenarios of an solar irradiation analysis.
Notice that the last surface get no material (for both themes), thus ``null`` is used.

.. code-block:: js

  {
    "type": "Solid",
    "lod": 2,
    "boundaries": [
      [ [[0, 3, 2, 1]], [[4, 5, 6, 7]], [[0, 1, 5, 4]], [[1, 2, 6, 5]] ] 
    ],
    "material": {
      "irradiation": { 
        "values": [[0, 0, 1, null]] 
      },
      "irradiation-2": { 
        "values": [[2, 2, 1, null]] 
      }
    }
  }


Geometry Object having texture(s)
*********************************

To store the texture(s) of a surface, a Geometry Object may have a member with the value ``"texture"``, its value is a collection of key-value pairs, where the key is the *theme* of the textures, and the value is one JSON object that must contain one member ``"values"``, whose value is a hierarchy of arrays with integers.
For each ring of each surface, the first value refers to the position (0-based) in the ``"textures"`` member of the ``"appearance"`` member of the CityJSON object.
The other indices refer to the UV positions of the corresponding vertices (as listed in the ``"boundaries"`` member of the geometry).
Each array representing a ring therefore has one more value than that to store its vertices.

The depth of the array depends on the Geometry object, and is equal to the depth of the ``"boundary"`` array.

In the following, the Solid has 4 surfaces, and there are 2 themes: "winter-textures" and "summer-textures" could for instance represent the textures during winter and summer..
Notice that the last 2 surfaces of the first theme gets no material, thus ``null`` is used.

.. code-block:: js

  {
    "type": "Solid",
    "lod": 2,
    "boundaries": [
      [ [[0, 3, 2, 1]], [[4, 5, 6, 7]], [[0, 1, 5, 4]], [[1, 2, 6, 5]] ] 
    ],
    "texture": {
      "winter-textures": {
        "values": [
          [ [[0, 10, 23, 22, 21]], [[0, 1, 2, 6, 5]], [[null]], [[null]] ]                  
        ]
      },
      "summer-textures": {
        "values": [
          [ [[1, 10, 23, 22, 21]], [[1, 1, 2, 6, 5]], [[1, 66, 12, 64, 5]], [[2, 99, 21, 16, 25]] ]                  
        ]      
      }
    }     
  }        

Material Object
***************

A Material Object:

  - must have one member with the name ``"name"``, whose value is a string identifying the material.
  - may have the following members (their meaning is explained `there <http://www.web3d.org/documents/specifications/19775-1/V3.2/Part01/components/shape.html#Material>`_):
    
    #. ``"ambientIntensity"``, whose value is a number between 0.0 and 1.0  
    #. ``"diffuseColor"``, whose value is an array with 3 numbers between 0.0 and 1.0 (RGB colour)
    #. ``"emissiveColor"``, whose value is an array with 3 numbers between 0.0 and 1.0 (RGB colour)
    #. ``"specularColor"``, whose value is an array with 3 numbers between 0.0 and 1.0 (RGB colour)
    #. ``"shininess"``, whose value is a number between 0.0 and 1.0
    #. ``"transparency"``, whose value is a number between 0.0 and 1.0 (1.0 being completely transparent)
    #. ``"isSmooth"``, whose value is a Boolean value, is defined in CityGML as "a hint for normal interpolation. If this boolean flag is set to true, vertex normals should be used for shading (Gouraud shading). Otherwise, normals should be constant for a surface patch (flat shading)."

.. code-block:: js

  "materials": [
    {
      "name": "roofandground",
      "ambientIntensity":  0.2000,
      "diffuseColor":  [0.9000, 0.1000, 0.7500],
      "emissiveColor": [0.9000, 0.1000, 0.7500],
      "specularColor": [0.9000, 0.1000, 0.7500],
      "shininess": 0.2,
      "transparency": 0.5,
      "isSmooth": false
    },
    {
      "name": "wall",
      "ambientIntensity":  0.4000,
      "diffuseColor":  [0.1000, 0.1000, 0.9000],
      "emissiveColor": [0.1000, 0.1000, 0.9000],
      "specularColor": [0.9000, 0.1000, 0.7500],
      "shininess": 0.0,
      "transparency": 0.5,
      "isSmooth": true
    }            
  ]


Texture Object
**************

A Texture Object:

  - must have one member with the name ``"type"``, whose value is a string with either "PNG" or "JPG" as value
  - must have one member with the name ``"image"``, whose value is a string with the name of the file. This file must be in a folder named ``"appearances"`` located in the same folder as the CityJSON file.
  - may have one member with the name ``"wrapMode"``, whose value can be any of the following: ``"none"``, ``"wrap"``, ``"mirror"``, ``"clamp"``, or ``"border"``.
  - may have one member with the name ``"textureType"``, whose value can be any of the following: ``"unknown"``, ``"specific"``, or ``"typical"``.
  - may have one member with the name ``"borderColor"``, whose value is an array with 4 numbers between 0.0 and 1.0 (RGBA colour).
  
.. code-block:: js

  "textures": [
    {
      "type": "PNG",
      "image": "myfacade.png"
    },
    {
      "type": "JPG",
      "image": "myroof.jpg",
      "wrapMode": "wrap",
      "textureType": "unknown",
      "borderColor": [0.0, 0.1, 0.2, 1.0]
    }      
  ]


Vertices-texture Object
***********************

A Appearance Object may have one member named ``"vertices-texture"``, whose value is an array of the *(u,v)* coordinates of the vertices used for texturing surfaces.
Their position in this array (0-based) is used by the ``"texture"`` member of the Geometry Objects.

  - the array of vertices may be empty.
  - one vertex must be an array with exactly 2 values, representing the *(u,v)* coordinates. 
  - The value of *u* and *v* must be between 0.0 and 1.0.
  - vertices may be repeated


.. code-block:: js

  "vertices-texture": [
    [0.0, 0.5],
    [1.0, 0.0],
    [1.0, 1.0],
    [0.0, 1.0]
  ]


