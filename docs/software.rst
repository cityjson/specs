========
Software
========

CityJSON utilities
------------------
We wrote some pieces of script and software, to test CityJSON.
These are to use with cautious, and should be considered *early alpha*.

They demonstate how to read and write CityJSON with Python, Ruby and C++.

We have currently the following:

- `cityjson_valschema <https://github.com/tudelft3d/cityjson/tree/master/software/cityjson-valschema>`_ : to validate a file against the CityJSON schema and more checks are done. 
- `citygml2cityjson <https://github.com/tudelft3d/cityjson/tree/master/software/citygml2cityjson>`_ : to convert the geometry of the buildings in a CityGML file to CityJSON and keeping the attributes. 
- `cityjson-compress <https://github.com/tudelft3d/cityjson/tree/master/software/cityjson-compress>`_ : implements the compression in the specifications (:doc:`specs`) 
- `cityjson-info <https://github.com/tudelft3d/cityjson/tree/master/software/cityjson-info>`_ : displays some general information a CityJSON file; useful to learn to parse a CityJSON file.
- `cityjson2obj <https://github.com/tudelft3d/cityjson/tree/master/software/cityjson2obj>`_ : converts a CityJSON to an OBJ file (textures/materials are ignored in the C++, considered in the Python script)

Software supporting CityJSON
----------------------------

  #. `val3dity <https://github.com/tudelft3d/val3dity>`_: validation of the geometric 3D primitives (the branch 'v2' reads CityJSON as input; everything is supported)
