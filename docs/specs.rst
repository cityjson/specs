
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

  #. one member with the name ``"type"``, whose value must be ``"CityModel"``;
  #. one member with the name ``"version"``, whose value must be a URL pointing to the CityJSON version;
  #. one member with the name ``"CityObjects"``. The value of this member is a collection of key-value pairs, where the key is the ID of the object, and the value is one City Object.
  #. one member with the name ``"vertices"``, whose value is an array of coordinates of each vertex of the city model. Their position in this array (0-based) is used as an index to be referenced by the Geometric Objects. The indexing mechanism of the format `Wavefront OBJ <https://en.wikipedia.org/wiki/Wavefront_.obj_file>`_ is basically reused.


- A CityJSON may have one member with the name ``"metadata"``, whose value may contain JSON objects describing the coordinate reference system used, the extent of the dataset, its creator, etc.
- A CityJSON may have one member with the name ``"transform"``, whose value must contain 2 JSON objects describing how to *decompress* the coordinates. Transform is used to reduce the file size only.
- A CityJSON may have one member with name ``"appearance"``, the value may contain JSON objects representing the textures and/or materials of surfaces.
- A CityJSON must not have other members.

The minimal valid CityJSON object is thus:

.. code-block:: js

  {
    "type": "CityModel",
    "version": "http://www.cityjson.org/version/0.1",
    "CityObjects": {},
    "vertices": []
  }


An empty CityJSON will look like this:

.. code-block:: js

  {
    "type": "CityModel",
    "version": "http://www.cityjson.org/version/0.1",
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


Be aware that the EPSG code should be a 3D CRS, ie the elevation/height values should be with respect to a specific datum.
It is not possible to give a WKT string with the parameters, or any other way.

.. note::
  Unlike in (City)GML where each object can have a different CRS (eg a wall of a building could theoretically have a different from the other walls used to represent the building), in CityJSON all the city objects need to be in the same CRS.


``"bbox"`` (extent of the dataset)
**********************************

While this can be extracted from the dataset itself, it is useful to store it. 
It may be stored as an array with 6 values: [minx, miny, minz, maxx, maxy, maxz]

.. code-block:: js

  "metadata": {
    "bbox": [ 84710, 446846, -5, 84757, 446944, 40 ]
  }


``"keywords"``
**************

An array of keywords of type ``"string"`` may be listed:

.. code-block:: js

  "metadata": {
    "keywords": [ "energy", "solar potential" ]
  }


Other properties
****************

The following are all of type ``"string"``:

- ``"title"``
- ``"abstract"``
- ``"dataUrl"``
- ``"metadataUrl"``
- ``"dateOfCreation"``
- ``"dateOfLastRevision"``
- ``"dateOfPublication"``
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
  #. ``"TINRelief"``
  #. ``"WaterBody"``
  #. ``"PlantCover"``
  #. ``"SolitaryVegetationObject"``
  #. ``"LandUse"``
  #. ``"CityFurniture"``
  #. ``"GenericCityObject"``

A City Object:

- must have one member with the name ``"geometry"``, whose value is an array containing 0 or more Geometry Objects. More than one Geometry Object is used to have several different levels-of-detail (LoDs) for the same object. However, the different Geometry Objects of a given City Object do not have be of different LoDs.
- may have one member with the name ``"attributes"``, whose value is an object with the different attributes allowed by CityGML. The attributes differ per City Object, and can be seen either in the `offical CityGML documentation <https://portal.opengeospatial.org/files/?artifact_id=47842>`_ or in the schema of CityJSON (:doc:`schema`). Any other attributes can be added with a JSON key-value pair ("owner" in the following is one such attribute), although it is not guaranteed that a parser will read them.


.. code-block:: js

  "CityObjects": {
      "id-1": {
        "type": "Building", 
        "attributes": { 
          "measuredHeight": 22.3,
          "roofType": "gable",
          "owner": "Elvis Presley"
        },
        "geometry": [
          {
            ...
          }        
        ]
      },
      "id-2": {
        "type": "PlantCover", 
        ...
      }
  }


Building, BuildingPart, and BuildingInstallation
************************************************


- A City Object of type ``"Building"`` may have a member ``"Parts"``, whose value is an array of the IDs of the City Objects of type ``"BuildingPart"`` it contains.
- A City Object of type ``"Building"`` or ``"BuildingPart"`` may have a member ``"Installations"``, whose value is an array of the IDs of the City Objects of type ``"BuildingInstallation"`` it contains.
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
      "ThoroughfareNumber": "4419",
      "ThoroughfareName": "rue de la Patate",
      "PostalCode": "H0H 0H0"

    },
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
        [ [[0, 3, 2, 1]], [[4, 5, 6, 7]], [[0, 1, 5, 4]] ]
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


GenericCityObject
*****************

- The geometry of a City Object of type ``"GenericCityObject"`` can be any of the following: ``"MultiPoint"``, ``"MultiLineString"``, ``"MultiSurface"``, ``"CompositeSurface"``, ``"Solid"``, or ``"CompositeSolid"``.


----------------
Geometry Objects
----------------

CityJSON defines the following 3D geometric primitives, ie all of them are embedded in 3D space and thus have coordinates *(x, y, z)* for their vertices. 
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

  - the array of vertices may be empty.
  - one vertex must be an array with exactly 3 values, representing the *(x,y,z)* location of the vertex.
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
      [ [[0, 3, 2, 1, 22]], [[4, 5, 6, 7]], [[0, 1, 5, 4]], [[1, 2, 6, 5]] ], //exterior shell
      [ [[240, 243, 124]], [[244, 246, 724]], [[34, 414, 45]], [[111, 246, 5]] ] //interior shell
    ]
  }

.. code-block:: js

  {
    "type": "CompositeSolid",
    "boundaries": [
      [ // 1st Solid
        [ [[0, 3, 2, 1, 22]], [[4, 5, 6, 7]], [[0, 1, 5, 4]], [[1, 2, 6, 5]] ],
        [ [[240, 243, 124]], [[244, 246, 724]], [[34, 414, 45]], [[111, 246, 5]] ]
      ],
      [ // 2st Solid
        [ [[666, 667, 668]], [[74, 75, 76]], [[880, 881, 885]], [[111, 122, 226]] ] 
      ]    
    ]
  }



Semantics
*********

Each surface of a Geometry Object can be assigned a semantics.
Since surfaces are assigned a semantics, and not rings, the depth of an array is one less than the array for storing the boundaries, eg for the case above.

.. code-block:: js

  {
    "type": "MultiSurface",
    "lod": 2,
    "boundaries": [
      [[0, 3, 2, 1]], [[4, 5, 6, 7]], [[0, 1, 5, 4]]
    ],
    "semantics": [
      ["RoofSurface"], ["GroundSurface"], ["WallSurface"]
    ],
  }


``"Building"``, ``"BuildingPart"``, and ``"BuildingInstallation"`` can have the following semantics for (LoD0 to LoD3; LoD4 is omitted):

- ``"RoofSurface"``, 
- ``"GroundSurface"``, 
- ``"WallSurface"``,
- ``"ClosureSurface"``,
- ``"OuterCeilingSurface"``,
- ``"OuterFloorSurface"``,
- ``"Window"``,
- ``"Door"``.

For WaterBody:

- ``"WaterSurface"``,
- ``"WaterGroundSurface"``,
- ``"WaterClosureSurface".``


------------------------------------------------
Transform Object (to compress the CityJSON file)
------------------------------------------------

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

Most of the Appearance module of CityGML is supported, and both textures and materials are supported. 
The material is represented with the `X3D <http://www.web3d.org/documents/specifications/19775-1/V3.2/Part01/components/shape.html#Material>`_ specifications, as is the case for CityGML.
For the texture, the COLLADA is reused, as is the case for CityGML.
However:
  - the CityGML concept of *themes* is not supported, this means that only one texture and one material per surface is allowed, and only one side of a surface can have a texture. Different LoDs can however have different textures/materials.
  - the CityGML class ``GeoreferencedTexture`` is not supported (this is used )
  - the CityGML class ``TexCoordGen`` is not supported, ie one must specify the UV coordinates in the texture files.
  - texture files have to be local and given in a relative position to the CityJSON file (thus requests to web services as is the case with CityGML are not supported).
  - the major difference is that in CityGML each Material/Texture object keeps a list of the primitives using it, while in CityJSON it is the opposite: if a primitive has a Material/Texture than it is stated with the primitive (with a link to it).

An Appearance Object is a JSON object that
  - may have one member with the name ``"materials"``, whose value is an array of Material Objects.
  - may have one member with the name ``"textures"``, whose value is an array of Texture Objects.
  - may have one member with the name ``"vertex-texture"``, whose value is an array of coordinates of each so-called UV vertex of the city model.

.. code-block:: js

  "appearance": {
    "materials": [],
    "textures":[],
    "vertices-texture": []
  }

Geometry Object having a material
*********************************

To store the material, a Geometry Object may have a member with the value ``"material"``, whose value is a hierarchy of arrays with integers (the depth depends on the Geometry object).
Each integer refers to the position (0-based) in the ``"materials"`` member of the ``"appearance"`` member of the CityJSON object.

In the following, the 6 surfaces representing a building get different materials, the roof and ground surfaces get the first material listed in the appearance, and the others get the second.

.. code-block:: js

  {
    "type": "Solid",
    "lod": 2,
    "boundaries": [
      [ [[0, 3, 2, 1]], [[4, 5, 6, 7]], [[0, 1, 5, 4]], [[1, 2, 6, 5]], [[2, 3, 7, 6]], [[3, 0, 4, 7]] ] 
    ],
    "semantics": [
      ["RoofSurface", "GroundSurface", "WallSurface", "WallSurface", "WallSurface", "WallSurface"]
    ],
    "material": [
      [0, 0, 1, 1, 1, 1]
    ]
  }


Geometry Object having a texture
********************************

To store the textures of surfaces, a Geometry Object may have a member with value ``"texture"``, whose value is a hierarchy of arrays with integers (the depth depends on the Geometry object).

For each ring of each surface, the first value refers to the position (0-based) in the ``"textures"`` member of the ``"appearance"`` member of the CityJSON object.
The other indices refer to the UV positions of the corresponding vertices (as listed in the ``"boundaries"`` member of the geometry).
Each array representing a ring therefore has one more value than that to store its vertices.

In the following, there are 6 surfaces representing a building and each get a texture.
The first 5 refer to the first texture, and the last one to the second one.

.. code-block:: js

  {
    "type": "Solid",
    "lod": 2.1,
    "boundaries": [
      [ [[0, 3, 2, 1]], [[4, 5, 6, 7]], [[0, 1, 5, 4]], [[1, 2, 6, 5]], [[2, 3, 7, 6]], [[3, 0, 4, 7]] ]
    ],
    "texture": [
      [ [[0, 10, 23, 23, 11]], [[0, 124, 35, 56, 76]], [[0, 10, 11, 45, 54]], [[0, 13, 52, 66, 57]], [[0, 12, 23, 17, 46]], [[1, 453, 4540, 44, 57]] ]
    ]
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
  - must have one member with the name ``"image"``, whose value is a string with the name of the file. This file must in a folder named ``"appearances"`` located in the same folder as the CityJSON file
  
.. code-block:: js

  "textures": [
    {
      "type": "PNG",
      "image": "myfacade.png"
    },
    {
      "type": "JPG",
      "image": "myroof.jpg"
    }      
  ]


Vertices-texture Object
***********************

- A Vertices-texture may have an empty array
- One vertex must be an array with exactly 2 values, representing the *(u,v)* coordinates of the vertex. The value of *u* and *v* must be between 0.0 and 1.0
- vertices may be repeated


.. code-block:: js

  "vertices-texture": [
    [0.0, 0.5],
    [1.0, 0.0],
    [1.0, 1.0],
    [0.0, 1.0]
  ]


