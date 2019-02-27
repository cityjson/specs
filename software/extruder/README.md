

A simple code to extrude footprints, to which an elevation is attached, to CityJSON LoD1 buildings.

It is meant as an example of how to create 3D city models with CityJSON simply.

The height of the buildings can be obtained with a LiDAR datasets: simply take the median values of all the LiDAR points falling into a given footprint.

- no need to "clean" the dataset while constructing it (eg to ensure that vertices are not duplicated) since [cjio](https://github.com/tudelft3d/cjio) has functions to clean automatically CityJSON files.

Thus the idea is to first construct a CityJSON model, and then:
```bash
cjio mycitymodel.json remove_duplicate_vertices remove_orpha_vertices save mynewmodel.json
```

