=========
Changelog
=========

.. http://keepachangelog.com/en/1.0.0/

[Unreleased]
------------
- Bridge, Tunnels, and Transportation modules of CityGML
- ADE or mechanisms to extend the CityJSON data model in a structured and document way
- handling of very large files, eg for streaming (see https://github.com/tudelft3d/cityjson/issues/6)

----

[0.3] - 2017-09-13
------------------

Added
*****
- utility cityjson-valschema now ensures that no two City Objects have the same ID/key

Changed
*******
- change to "header" and versioning of the file
- >1 textures and materials per geometry

----

[0.2] - 2017-09-05
------------------

Added
*****
- Semantics Objects, so that Semantic Surfaces have specific attributes 
- software to validate the schema + other properties impossible represent with the schema, eg warning users of "soft" errors like attributes not in CityGML

Changed
*******
- metadata now ISO19115 compliant 💥
- materials now use X3D mechanism, same as CityGML
- textures now use COLLADA mechanism, same as CityGML
- improved greatly the schema (more is validated) 

----

[0.1] - 2017-08-01 
------------------
Added
*****
- first beta release of CityJSON


