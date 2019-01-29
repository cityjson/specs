========
Software
========

CityJSON utilities
------------------

To test CityJSON, we wrote some pieces of script and software.
They demonstate how to read and write CityJSON with Python, Ruby, and C++.
These are to use with cautious, and should be considered beta.

`demo software on GitHub <https://github.com/tudelft3d/cityjson/tree/master/software/>`_


Software supporting CityJSON
----------------------------

  - `cjio <https://github.com/tudelft3d/cjio>`_: Python CLI to process and manipulate CityJSON files. The different operators can be chained to perform several processing operations in one step, the CityJSON model goes through them and different versions of the CityJSON model can be saved as files along the pipeline. Also used as the 'official validator' for CityJSON files.
  - `citygml4j <https://github.com/citygml4j/citygml4j>`_: The open-source Java API for CityGML, allows us to convert back and forth between CityJSON and CityGML
  - `3dfier <https://github.com/tudelft3d/3dfier>`_: '3dfication' of 2D GIS datasets
  - `azul <https://github.com/tudelft3d/azul>`_: 3D city model viewer for Mac; full support for v0.5+ (including Extensions), except of the textures/materials
  - `val3dity <https://github.com/tudelft3d/val3dity>`_: validation of the geometric 3D primitives (everything is supported in master branch); the `web-application <http://geovalidation.bk.tudelft.nl/val3dity/>`_ also fully CityJSON input
  - `QGIS Plugin <https://github.com/tudelft3d/cityjson-qgis-plugin>`_: a plugin that supports the loading of CityJSON datasets in QGIS 3.
