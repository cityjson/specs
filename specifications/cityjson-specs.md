

# Introduction

CityJSON is a format for encoding a subset of the [CityGML](https://www.citygml.org) data model using JavaScript Object Notation ([JSON](http://json.org)).
A CityJSON file represents both the geometry and the semantics of the city features of a given area, eg buildings, roads, rivers, the vegetation, and the city furniture.

The aim of CityJSON is to offer an alternative to the GML encoding of CityGML, which can be verbose and complex. 
CityJSON aims at being easy-to-use, both for creating datasets, and for reading/parsing them.
It was designed with programmers in mind, so that tools and APIs supporting it can be quickly built.
It was also designed to be compact.

A CityJSON object, representing a city, is as 'flat' as possible, ie the hierarchy of CityGML has been flattened out and only the city objects which are 'leaves' of this hierarchy are implemented.
This considerably simplifies the storage of a city model, and furthermore does not mean that information is lost.


## A JSON encoding of GML, huh?!?

While its name otherwise implies, CityGML is not only a GML encoding, but is actually an open standardised data model to store digital 3D models of cities and landscapes. 
It defines ways to describe most of the common 3D features found in cities, and the relationships between them. 
It also defines different standard levels of detail (LODs) for the 3D objects, which allows us to represent objects for different applications and purposes.

CityGML currently has 2 implementations:

  1. the GML encoding is the "standard" one, and is defined in the [offical documentation](https://portal.opengeospatial.org/files/?artifact_id=47842)
  1. a [database schema called 3DCityDB](http://www.3dcitydb.org), which can be implemented both for [PostgreSQL](https://www.postgresql.org) and [Oracle Spatial](https://www.oracle.com/database/spatial/index.html).

CityJSON can be considered as the third implementation of the CityGML data model.


## Which CityGML modules are supported?

At this moment, CityJSON implements most of the data model.
However, for the sake of simplicity and efficiency, some modules and features have been omitted.

The types of objects stored in CityGML are grouped into different modules, and CityJSON supports these(more details below):

  1. __Building__: everything is supported except LOD4 features (interior of buildings) and the concept of Terrain-Intersection-Curve (TIC)
  1. __CityFurniture__: benches, traffic lights, signs, etc. are all supported
  1. __LandUse__: everything supported
  1. __Relief__: only the `TINRelief/TriangulatedSurface` is currently supported. `Tin` (where only elevation points and break lines are stored) is not supported since it would require viewer/applications to have a Constrained Delaunay Triangulator, which is problematic (especially for web-based tools). Also, it is not possible to store areas over a terrain that would support different resolutions (as in Figure 25 of the CityGML standard document). `RasterRelief` is also not supported.
  1. __Vegetation__: everything is supported
  1. __WaterBody__: everything is supported
  1. __Generics__: everything is supported
  1. __Address__: everything is supported
  1. __Appearance__: textures and materials are supported, albeit only one per surface is allowed 


## CityGML modules and characteristics not supported

The following modules are __not__ supported (but we're working on it):

  1. __Bridge__: bridge-related structures, possibly split into parts
  1. __Transportation__: roads, railways and squares
  1. __Tunnel__: tunnels, possibly split into parts

Furthermore, these characteristics are not supported:

  1. no support for ImplicitGeometries
  1. no support for ADEs (or other extension mechanism)
  1. no support for the topological relationships that can be defined, eg  *relativeToTerrain* and *relativeToWater*
  1. no support for arbitrary coordinate reference systems (CRSs). Only an [EPSG code](https://epsg.io) can be used. Furthermore, all geometries in a given CityJSON must be using the same CRS.



# CityJSON Object

A CityJSON object represents one 3D city model of a given area, this model may contain features of different types, as defined in the CityGML data model.

  - A CityJSON object is a JSON object.
  - A CityJSON object must have 4 members: 
    1. one member with the name "type", whose value must be "CityModel";
    1. one member with the name "version", whose value must be a URL pointing to the CityJSON version;
    1. one member with the name "CityObjects". The value of this member is a collection of city objects, with their ID as the name.
    1. one member with the name "vertices", whose value is an array of coordinates of each vertex of the city model. Their position in this array (0-based) is used to represent the Geometric Objects.
  - A CityJSON may have one member with the name "metadata", whose value may contain JSON objects describing the coordinate reference system used, the extent of the dataset, its creator, etc.
  - A CityJSON may have one member with the name "transform", whose value must contain 2 JSON objects describing how to *decompress* the coordinates. Transform is used to reduce the file size only.
  - A CityJSON may have one member with name "appearance", the value may contain JSON objects representing the textures and/or materials of surfaces.
  - A CityJSON must not have other members.

The minimal valid CityJSON object is thus:

```json
{
  "type": "CityModel",
  "version": "http://www.cityjson.org/version/0.1",
  "CityObjects": {},
  "vertices": []
}
```

An empty CityJSON will look like this:

```json
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
```

# Metadata

The metadata related to the 3D city model may be stored.

## "crs" (Coordinate reference system)

The CRS may be given with a JSON object that must contain one member "epsg" with as value the [EPSG code](https://epsg.io), as an integer.
For instance, for the WGS84 latitude-longitude:

```json
  "metadata": {
    "crs": { 
      "epsg": 7415
    }
  }
```

Be aware that the EPSG code should be a 3D CRS, ie the elevation/height values should be with respect to a specific datum.

## "bbox" (extent of the dataset)

While this can be extracted from the dataset itself, it is useful to store it. 
It may be stored as an array with 6 values: [minx, miny, minz, maxx, maxy, maxz]

```json
  "metadata": {
    "bbox": [ 84710, 446846, -5, 84757, 446944, 40 ]
  }
```


## "keywords"

An array of keywords of type "string" may be listed:

```json
  "metadata": {
    "keywords": [ "energy", "solar potential" ]
  }
```

## Other properties

The following are all of type "string":

  - "title"
  - "abstract"
  - "dataUrl"
  - "metadataUrl"
  - "dateOfCreation"
  - "dateOfLastRevision"
  - "dateOfPublication"
  - "copyright"

It should be noticed that JSON does not have a date type, thus for all dates in a CityJSON document the following should be used: YYYY-MM-DD (as a string).


# City Object types

A City Object is a JSON object for which the type member’s value is one of the following (of type string):

  1. Building
  1. BuildingPart
  1. BuildingInstallation
  1. TINRelief
  1. WaterBody
  1. PlantCover
  1. SolitaryVegetationObject
  1. LandUse
  1. CityFurniture
  1. GenericCityObject

A City Object:

  - must have one member with the name "geometry", whose value is an array containing 0 or more Geometry Objects.
  - may have one member with the name "attributes", whose value is an object with the different attributes allowed by CityGML (these differ per City Object). 
  - may have one member with the name "generic-attributes", whose value is any JSON object, or a JSON null value. This is used to add attributes not defined in the CityGML data model, and is the same as the 'generics' module of CityGML.

```json
"CityObjects": {
    "id-1": {
      "type": "Building", 
      "attributes": { 
        "measuredHeight": 22.3,
        "roofType": "gable"
      },
      "generic-attributes": {
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
```


## Building, BuildingPart, and BuildingInstallation

- A City Object of type "Building" may have a member "Parts", whose value is an array of the IDs of the City Objects of type "BuildingPart" it contains.
- A City Object of type "Building" or "BuildingPart" may have a member "Installations", whose value is an array of the IDs of the City Objects of type "BuildingInstallation" it contains.

```json
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
```

- The geometry of both "Building" and "BuildingPart" can only be represented with these Geometry Objects: (1) Solid, (2) CompositeSolid, (3) MultiSurface.
- The geometry of a "BuildingInstallation" object can be represented with any of the Geometry Objects.
- A City Object of type "Building" or "BuildingPart" may have a member "address", whose value is a JSON object describing the address. One location (a MultiPoint) can be given, to for instance locate the front door inside the building.

```json
{
  "type": "Building", 
  "address": {
    "CountryName": "string",
    "LocalityName": "string",
    "ThoroughfareNumber": "string",
    "ThoroughfareName": "string",
    "PostalCode": "string"
  },
}
},


## TINRelief

- The geometry of a City Object of type "TINRelief" can only be of type "CompositeSurface".
- There is no specific Geometry Object for a TIN, it is simply a CompositeSurface for which every surface is a triangle (without interior rings).

```json
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
```

<!-- TODO: should TINs have semantics?!? -->

## WaterBody

- The geometry of a City Object of type "WaterBody" can be of types: "MultiLineString", "MultiSurface"
"CompositeSurface", "Solid", or "CompositeSolid".

```json
"mylake": {
      "type": "WaterBody", 
      "attributes": {
        "usage": "leisure",
      },
      "geometry": [{
        "type": "Solid",
        "lod": 2,
        "boundaries": [
          [ [[0, 3, 2, 1]], [[4, 5, 6, 7]], [[0, 1, 5, 4]] ]
        ],
        "semantics": [ ["WaterSurface"], ["WaterGroundSurface"], ["WaterGroundSurface"] ]
      }]    
    }               
  }
```


## LandUse

- The geometry of a City Object of type "LandUse" can be of type "MultiSurface" or "CompositeSurface".

```json
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
```


## PlantCover

- The geometry of a City Object of type "PlantCover" can be of type "MultiSurface" or "MultiSolid".

```json
"myterrain01": {
  "type": "LandUse", 
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
```

## SolitaryVegetationObject

- The geometry of a City Object of type "SolitaryVegetationObject" can be any of the following: "MultiPoint", MultiLineString", "MultiSurface", "CompositeSurface", "Solid", "CompositeSolid", or "MultiGeometry".
- The concept of Implicit Geometries, as defined in CityGML, is not supported. An implicit geometry is a template, eg of certain species of a tree, that can be reused with different parameters to define its appearance.

```json
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
```


## CityFurniture

- The geometry of a City Object of type "CityFurniture" can be any of the following: "MultiPoint", MultiLineString", "MultiSurface", "CompositeSurface", "Solid", "CompositeSolid", or "MultiGeometry".


## GenericCityObject

- The geometry of a City Object of type "CityFurniture" can be any of the following: "MultiPoint", MultiLineString", "MultiSurface", "CompositeSurface", "Solid", "CompositeSolid", or "MultiGeometry".


# Geometry Objects

CityJSON defines the following 3D geometric primitives, ie all of them are embedded in 3D space and thus have coordinates *(x, y, z)* for their vertices. 
As is the case in CityGML, only linear and planar primitives are allowed (no curves or parametric surfaces for instance).

A Geometry object is a JSON object for which the type member’s value is one of the following:

  1. MultiPoint
  1. MultiLineString
  1. MultiSurface
  1. CompositeSurface
  1. Solid
  1. MultiSolid
  1. CompositeSolid

A Geometry object:

  - must have one member with the name "lod", whose value is a number identifying the level-of-detail (LoD) of the geometry. This can be either an integer (following the CityGML standards), or a number following the [improved LoDs by TU Delft](https://www.citygml.org/ongoingdev/tudelft-lods/)
  - must have one member with the name "boundaries", whose value is a hierarchy of arrays (the depth depends on the Geometry object) with integers. An integer refers to the index in the "vertices" array of the CityJSON object, and it is 0-based (ie the first element in the array has the index "0", the second one "1").
  - may have one member "semantics", whose value is a hierachy of nested arrays (the depth depends on the Geometry object). The value of each entry is a string, and the values allowed are depended on the CityObject (see below).
  - may have one member "material", whose value is a hierachy of nested arrays (the depth depends on the Geometry object). The value of each entry is an integer referring to the material used (see below).
  - may have one member "texture", whose value is a hierachy of nested arrays (the depth depends on the Geometry object). The value of each entry is explained below.
  - must not have other members.

Observe that there is __no__ Geometry Object for MultiGeometry. 
Instead, for the "geometry" member of a CityObject, the different geometries may be enumerated in the array (all with the same value for the member "lod").


## The coordinates of the vertices

A CityJSON must have one member named "vertices", whose value is an array of coordinates of each vertex of the city model. 
Their position in this array (0-based) is used to represent the Geometric Objects.

  - the array of vertices may be empty.
  - one vertex must be an array with exactly 3 values, representing the *(x,y,z)* location of the vertex.
  - vertices may be repeated


```json
"vertices": [
  [0.0, 0.0, 0.0],
  [1.0, 0.0, 0.0],
  [1.0, 0.0, 0.0],
  ...
  [0.0, 1.0, 0.0],
  [8523.134, 487625.134, 2.03]
]
```


## Arrays to represent boundaries

- A MultiPoint has an array with the indices of the vertices; this array can be empty.
- A MultiLineString has an array of arrays, each containing the indices of a LineString
- A MultiSurface, or a CompositeSurface, has an array containing surfaces, each surface is modelled by an array of array, the first array being the exterior boundary of the surface, and the others the interior boundaries.
- A Solid has an array of shells, the first array being the exterior shell of the Solid, and the others the interior shells. Each shell has an array of surfaces, modelled in the exact same way as a MultiSurface/CompositeSurface.
- A MultiSolid, or a CompositeSolid, has an array containing Solids, each Solid is modelled as above.

```json
{
  "type": "MultiPoint",
  "boundaries": [2, 44, 0, 7]
}
```

```json
{
  "type": "MultiLineString",
  "boundaries": [
    [2, 3, 5], [77, 55, 212]
  ]  
}
```

```json
{
  "type": "MultiSurface",
  "boundaries": [
    [[0, 3, 2, 1]], [[4, 5, 6, 7]], [[0, 1, 5, 4]]
  ]
}
```

```json
{
  "type": "Solid",
  "boundaries": [
    [ [[0, 3, 2, 1, 22]], [[4, 5, 6, 7]], [[0, 1, 5, 4]], [[1, 2, 6, 5]] ], //exterior shell
    [ [[240, 243, 124]], [[244, 246, 724]], [[34, 414, 45]], [[111, 246, 5]] ] //interior shell
  ]
}
```

```json
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
```


## Semantics

<!-- TODO: describe the part about semantics "one-level higher"? Do that anyways? -->

Each surface of a Geometry Object can be assigned a semantics.
Since surfaces are assigned a semantics, and not rings, the depth of an array is one less than the array for storing the boundaries, eg for the case above.

```json
{
  "type": "MultiSurface",
  "boundaries": [
    [[0, 3, 2, 1]], [[4, 5, 6, 7]], [[0, 1, 5, 4]]
  ],
  "semantics": [
    ["RoofSurface"], ["GroundSurface"], ["WallSurface"]
  ],
}
```

Building, BuildingPart, and BuildingInstallation can have the following semantics for (LoD0 to LoD3, LoD4 is omitted):

  - "RoofSurface", 
  - "GroundSurface", 
  - "WallSurface",
  - "ClosureSurface",
  - "OuterCeilingSurface",
  - "OuterFloorSurface",
  - "Window",
  - "Door".

For WaterBody:

  - "WaterSurface",
  - "WaterGroundSurface",
  - "WaterClosureSurface".


# Transform Object (to compress the CityJSON)

To reduce the size of a file, it is possible to represent the coordinates of the vertices with integer values, and store the scale factor and the translation needed to obtain the original coordinates (stored with floats/doubles).
To use compression, a CityJSON object may have one member "transform", whose values are 2 mandatory JSON objects ("scale" and "translate"), both arrays with 3 values.

We reuse the [scheme of TopoJSON (called quantization)](https://github.com/topojson/topojson-specification/blob/master/README.md#212-transforms) and simply add a third coordinate because our vertices are embedded in 3D space.

To obtain the real position of a given vertex *v*, we must take the 3 values *vi* listed in the "vertices" member and:

    v[0] = (vi[0] * ["transform"]["scale"][0]) + ["transform"]["translate"][0]
    v[1] = (vi[1] * ["transform"]["scale"][1]) + ["transform"]["translate"][1]
    v[2] = (vi[2] * ["transform"]["scale"][2]) + ["transform"]["translate"][2]

If the CityJSON file does not have a "transform" member, then the values of the vertices must be read as-is.

There is a program [cityjson-compress](../software/cityjson-compress/) that will compress a given file by: (1) merging duplicate vertices; (2) convert coordinates to integer. 
Both operation use a tolerance, which is given as number-of-digits-after-the-dot.


```json
"transform": {
    "scale": [0.01, 0.01, 0.01],
    "translate": [4424648.79, 5482614.69, 310.19]
}
```


# Appearance Object

A subset of the Appearance module of CityGML.
Both textures and materials are supported, albeit only one texture and one material per surface is allowed, and only one side of a surface can have a texture.
The CityGML concept of *themes* is thus not supported.
Different LoDs can however have different textures/materials.
The standard from the [Material Template Library format (MTL)](https://en.wikipedia.org/wiki/Wavefront_.obj_file#Material_template_library) is reused, which is used by the well-known [format Wavefront OBJ](https://en.wikipedia.org/wiki/Wavefront_.obj_file).

  - An Appearance Object may have one member with the name "materials", whose value is an array of Material Objects.
  - An Appearance Object may have one member with the name "textures", whose value is an array of Texture Objects.
  - An Appearance Object may have one member with the name "vertex-texture", whose value is an array of coordinates of each so-called UV vertex of the city model.

```json
"appearance": {
  "materials": [],
  "textures":[],
  "vertices-texture": []
}
```

## Geometry Object having a material

To store the material, a Geometry Object may have a member with value "material", whose value is an is a hierarchy of arrays (the depth depends on the Geometry object) with integers.
Each integer refers to the position (0-based) in the "materials" member of the "appearance" member of the CityJSON object.

In the following, the 6 surfaces representing a building get different materials, the roof and ground surfaces get the first material listed in the appearance, and the others get the second.

```json
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
```


## Geometry Object having a texture

To store the textures of surfaces, a Geometry Object may have a member with value "texture", whose value is a hierarchy of arrays (the depth depends on the Geometry object) with integers.

For each ring of each surface, the first value refers to the the position (0-based) in the "textures" member of the "appearance" member of the CityJSON object; this is to allow geometries having more than one textures.
The other indices, refer to the UV position of the corresponding vertices (as listed in the "boundaries" member of the geometry).

```json
{
  "type": "Solid",
  "lod": 2.1,
  "boundaries": [
    [ [[0, 3, 2, 1]], [[4, 5, 6, 7]], [[0, 1, 5, 4]], [[1, 2, 6, 5]], [[2, 3, 7, 6]], [[3, 0, 4, 7]] ]
  ],
  "texture": [
    [ [[0, 10, 23, 23, 11]], [[0, 124, 35, 56, 76]], [[0, 10, 11, 45, 54]], [[0, 13, 52, 66, 57]], [[0, 12, 23, 17, 46]], [[0, 453, 4540, 44, 57]] ]
  ]
}        
```

## Material Object

A Material Object:

  - must have one member with the name "name", whose value is a string identifying the material.
  - may have the following members:
    1. "ambient" (ambient colour), whose value is an array with 3 numbers between 0.0 and 1.0
    1. "diffuse" (diffuse colour), whose value is an array with 3 numbers between 0.0 and 1.0
    1. "specular" (specular colour), whose value is an array with 3 numbers between 0.0 and 1.0
    1. "specular-diffuse" (the weight of the specular colour), whose value is a number between 0 and 1000
    1. "illumination", whose value is an integer between 0 and 10
    1. "transparency", whose value is a number between 0.0 and 1.0 (1.0 being completely opaque)

```json
"materials": [
  {
    "name": "roofandground",
    "ambient":  [0.9000, 0.1000, 0.7500],
    "diffuse":  [0.9000, 0.1000, 0.7500],
    "specular": [0.9000, 0.1000, 0.7500],
    "specular-exponent": 10.0,
    "illumination": 2,
    "transparency": 1.0
  },
  {
    "name": "wall",
    "ambient":  [0.9000, 0.9000, 0.7500],
    "diffuse":  [0.9000, 0.9000, 0.7500],
    "specular": [0.9000, 0.9000, 0.7500],
    "specular-exponent": 60.0,
    "illumination": 1,
    "transparency": 0.5
  }            
]
```


## Texture Object

A Texture Object:

  - must have one member with the name "type", whose value is a string with either "PNG" or "JPG" as value.
  - must have one member with the name "image", whose value is a string with the name of the file. This file must in a folder named "appearances" located in the same folder as the CityJSON file.
  
```json
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
```


## Vertices-Texture Object

  - the array may be empty.
  - one vertex must be an array with exactly 2 values, representing the *(u,v)* coordinates of the vertex. The value of *u* and *v* must be between 0.0 and 1.0
  - vertices may be repeated


```json
"vertices-texture": [
  [0.0, 0.5],
  [1.0, 0.0],
  [1.0, 1.0],
  [0.0, 1.0]
]
```

