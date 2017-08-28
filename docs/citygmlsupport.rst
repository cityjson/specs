===============
CityGML support
===============


CityGML modules and characteristics supported
---------------------------------------------

At this moment, CityJSON implements most of the data model.
However, for the sake of simplicity and efficiency, some modules and features have been omitted.
If a module is supported, it does not mean that there is a 1-to-1 mapping between the classes and features in CityGML and CityJSON, but rather that it is possible to represent the same information, but in a different manner.

The types of objects stored in CityGML are grouped into different modules, and CityJSON supports these (more details below):

#. **Building**: everything is supported except LOD4 features (interior of buildings) and the concept of Terrain-Intersection-Curve (TIC)
#. **CityFurniture**: benches, traffic lights, signs, etc. are all supported
#. **LandUse**: everything supported
#. **Relief**: only the ``TINRelief/TriangulatedSurface`` is currently supported. ``Tin`` (where only elevation points and break lines are stored) is not supported since it would require viewer/applications to have a Constrained Delaunay Triangulator, which is problematic (especially for web-based tools). Also, it is not possible to store areas over a terrain that would support different resolutions (as in Figure 25 of the `CityGML standard document <https://portal.opengeospatial.org/files/?artifact_id=47842>`_). ``RasterRelief`` is also not supported.
#. **Vegetation**: everything is supported
#. **WaterBody**: everything is supported
#. **Generics**: everything is supported
#. **Address**: everything is supported
#. **Appearance**: the CityGML concept of *themes* is not supported, this means that only one texture and one material per surface is allowed, and only one side of a surface can have a texture. Different LoDs can however have different textures/materials. The CityGML class ``GeoreferencedTexture`` is not supported. The CityGML class ``TexCoordGen`` is not supported, ie one must specify the UV coordinates in the texture files. Also, the texture files have to be local and put in folder named ``"appearances"`` located in the same folder as the CityJSON file (thus requests to web services as is the case with CityGML are not supported).

.. note::
  CityJSON has built-in support for the metadata of a dataset, while, oddly, CityGML does not.


CityGML modules and characteristics not supported
-------------------------------------------------

The following modules are **not** supported (but we're working on it):

#. **Bridge**: bridge-related structures, possibly split into parts
#. **Transportation**: roads, railways and squares
#. **Tunnel**: tunnels, possibly split into parts

Furthermore, these are not supported:

#. no support for ImplicitGeometries
#. no support for ADEs (or other extension mechanism)
#. no support for the topological relationships that can be defined, eg  *relativeToTerrain* and *relativeToWater*
#. no support for arbitrary coordinate reference systems (CRSs). Only an `EPSG code <https://epsg.io>`_ can be used. Furthermore, all geometries in a given CityJSON must be using the same CRS.
#. in CityGML most objects can have an ID (usually ``gml:id``), that is one Building can have an ID, but also each 3D primitive forming its geometry can have an ID. In CityJSON, only :ref:`specs_cityobjects` can have IDs, and each surface. 