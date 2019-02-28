
A simple example of how to extrude footprints, to which an elevation is attached, to generate CityJSON LoD1 buildings.
The height of the buildings are stored in the example as one attribute of the footprints, but these can be obtained with a LiDAR datasets: simply take the median/average value of all the LiDAR points falling into a given footprint.

The code uses [Fiona](https://github.com/Toblerity/Fiona) to read the GIS datasets, and [Shapely](https://github.com/Toblerity/Shapely) to process the geometries.
The CityJSON file is created in pure Python.

It should be noticed that:

  - there is no need to "clean" the dataset while constructing it (eg to ensure that vertices are not duplicated) since [cjio](https://github.com/tudelft3d/cjio) has functions to clean automatically CityJSON files. Thus the idea is to first construct a CityJSON model, and then: `$ cjio mycitymodel.json remove_duplicate_vertices remove_orphan_vertices save mynewmodel.json`
  - to know if the result is syntactically valid: `$ cjio mynewmodel.json validate`
  - to know whether the geometries are valid solids according to ISO 19107, use [val3dity](https://github.com/tudelft3d/val3dity)
  

