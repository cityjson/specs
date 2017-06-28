===============
CityGML support
===============


Which CityGML modules are supported?
------------------------------------

At this moment, CityJSON implements most of the data model.
However, for the sake of simplicity and efficiency, some modules and features have been omitted.

The types of objects stored in CityGML are grouped into different modules, and CityJSON supports these (more details below):

#. **Building**: everything is supported except LOD4 features (interior of buildings) and the concept of Terrain-Intersection-Curve (TIC)
#. **CityFurniture**: benches, traffic lights, signs, etc. are all supported
#. **LandUse**: everything supported
#. **Relief**: only the ``TINRelief/TriangulatedSurface`` is currently supported. ``Tin`` (where only elevation points and break lines are stored) is not supported since it would require viewer/applications to have a Constrained Delaunay Triangulator, which is problematic (especially for web-based tools). Also, it is not possible to store areas over a terrain that would support different resolutions (as in Figure 25 of the `CityGML standard document <https://portal.opengeospatial.org/files/?artifact_id=47842>`_). ``RasterRelief`` is also not supported.
#. **Vegetation**: everything is supported
#. **WaterBody**: everything is supported
#. **Generics**: everything is supported
#. **Address**: everything is supported
#. **Appearance**: textures and materials are supported, albeit only one per surface is allowed 


CityGML modules and characteristics not supported
-------------------------------------------------

The following modules are **not** supported (but we're working on it):

#. **Bridge**: bridge-related structures, possibly split into parts
#. **Transportation**: roads, railways and squares
#. **Tunnel**: tunnels, possibly split into parts

Furthermore, these characteristics are not supported:

#. no support for ImplicitGeometries
#. no support for ADEs (or other extension mechanism)
#. no support for the topological relationships that can be defined, eg  *relativeToTerrain* andrelativeToWater*
#. no support for arbitrary coordinate reference systems (CRSs). Only an `EPSG code <https://epsg.io>`_ can be used. Furthermore, all geometries in a given CityJSON must be using the same CRS.