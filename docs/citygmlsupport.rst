===============
CityGML support
===============


CityGML modules and characteristics supported
---------------------------------------------

At this moment, CityJSON implements most of the data model, and *all* the CityGML modules have been mapped to CityJSON objects (as of v0.5).
However, for the sake of simplicity and efficiency, some modules and features have been omitted and/or simplified.
If a module is supported, it does not mean that there is a 1-to-1 mapping between the classes and features in CityGML and CityJSON, but rather that it is possible to represent the same information, but in a different manner.

The types of objects stored in CityGML are grouped into different modules, and CityJSON supports these (more details below):

#. **CityGML Core**: all geometries can be represented, Implicit Geometries are supported. However, the ``ExternalReferences`` are not supported.
#. **Building**: everything is supported except LOD4 features (interior of buildings) and the concept of Terrain-Intersection-Curve (TIC)
#. **Bridge**: everything is supported except LOD4 features (interior rooms of bridges)
#. **Tunnel**: everything is supported except LOD4 features (interior parts of Tunnel Installations)
#. **CityFurniture**: benches, traffic lights, signs, etc. are all supported
#. **LandUse**: everything supported
#. **Relief**: only the ``TINRelief/TriangulatedSurface`` is currently supported. ``Tin`` (where only elevation points and break lines are stored) is not supported since it would require viewer/applications to have a Constrained Delaunay Triangulator, which is problematic (especially for web-based tools). Also, it is not possible to store areas over a terrain that would support different resolutions (as in Figure 25 of the `CityGML standard document <https://portal.opengeospatial.org/files/?artifact_id=47842>`_). ``RasterRelief`` is also not supported.
#. **Transportation**: most modules supported, except the class "Path" is not used (an attribute to the class "Road" is used instead). Also, the LoD0 for all the Transportation is not supported (we plan to develop a network type in future versions to support properly, a simple set of line strings is not sufficient)
#. **CityObjectGroup**: groups of City Objects are supported, but not groups of parts of objects (eg it is not possible to group some walls of a building together)
#. **Vegetation**: everything is supported
#. **WaterBody**: everything is supported
#. **Generics**: everything is supported
#. **Address**: everything is supported
#. **Appearance**: The CityGML class ``GeoreferencedTexture`` is not supported. The CityGML class ``TexCoordGen`` is not supported, ie one must specify the UV coordinates in the texture files. Also, the texture files have to be local and put in folder named ``"appearances"`` located in the same folder as the CityJSON file (thus requests to web services as is the case with CityGML are not supported).

.. note::
  CityJSON has built-in support for the metadata of a dataset, while, surprisingly, CityGML does not.


Other CityGML modules and characteristics not supported
-------------------------------------------------------

#. no support for ADEs (or other extension mechanism)
#. no support for the topological relationships that can be defined, eg  *relativeToTerrain* and *relativeToWater*
#. no *ClosureSurface*
#. no support for arbitrary coordinate reference systems (CRSs). Only an `EPSG code <https://epsg.io>`_ can be used. Furthermore, all geometries in a given CityJSON must be using the same CRS.
#. in CityGML most objects can have an ID (usually ``gml:id``), that is one Building can have an ID, but also each 3D primitive forming its geometry can have an ID. In CityJSON, only :ref:`specs_cityobjects` can have IDs, and each :ref:`specs_semantics`. 
