========
Software
========

CityJSON utilities
------------------
We wrote some pieces of script and software, to test CityJSON.
These are to use with cautious, and should be considered *early alpha*.

They demonstate how to read and write CityJSON with Python, Ruby and C++.

We have currently the following:

  - `cjvalschema <https://github.com/tudelft3d/cityjson/tree/master/software/cjvalschema>`_ : to validate a file against the CityJSON schema, and more checks are done. 
  - `cjcompress <https://github.com/tudelft3d/cityjson/tree/master/software/chcompress>`_ : implements the compression in the specifications and allows to remove unused vertices(:doc:`specs`) 
  - `cjinfo <https://github.com/tudelft3d/cityjson/tree/master/software/cjinfo>`_ : displays some general information a CityJSON file; useful to learn how to parse a CityJSON file.
  - `cityjson2obj <https://github.com/tudelft3d/cityjson/tree/master/software/cityjson2obj>`_ : converts a CityJSON to an OBJ file (textures/materials are ignored in the C++, considered in the Python script)

Software supporting CityJSON
----------------------------

  - `citygml4j <https://github.com/citygml4j/citygml4j>`_: The open-source Java API for CityGML, allows us to convert back and forth between CityJSON and CityGML. (the branch 'json' is in beta and supports v0.5)
  - `3dfier <https://github.com/tudelft3d/3dfier>`_: '3dfication' of 2D GIS datasets (the branch 'cityjson-output' outputs CityJSON)
  - `azul <https://github.com/tudelft3d/azul>`_: beta support for CityJSON
  - `val3dity <https://github.com/tudelft3d/val3dity>`_: validation of the geometric 3D primitives (everything is supported in master branch); the `web-application <http://geovalidation.bk.tudelft.nl/val3dity/>`_ also fully CityJSON input)
