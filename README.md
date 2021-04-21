
# CityJSON

This repository is where the specifications and the schemas of CityJSON are kept.

For all the details, see the official CityJSON website: [https://cityjson.org](https://cityjson.org).


## What is CityJSON?

CityJSON is a [JSON-based](http://json.org) encoding for storing 3D city models, also called digital maquettes or digital twins.

The aim of CityJSON is to offer a compact and developer-friendly format, so that files can be easily visualised, manipulated, and edited.
It was designed with programmers in mind, so that tools and APIs supporting it can be quickly built, and several have been created already.

We believe that you should use CityJSON because: 

  1. its simplicity means that it is already supported by [several software](https://cityjson.org/software/) 
  2. you can in one-click convert CityGML files to CityJSON files, and vice versa, with the open-source tool [citygml-tools](https://github.com/citygml4j/citygml-tools)
  3. files are on average [6X more compact](https://github.com/cityjson/specs/wiki/Compression-factor-for-a-few-open-CityGML-datasets) than their CityGML equivalent
  4. there is a [web-viewer](https://ninja.cityjson.org) where you can drag-and-drop a file
  5. you can easily manipulate files with [cjio](https://github.com/tudelft3d/cjio), you can for instance merge files, remove/filter objects, change the CRS, manage the textures, etc.
  6. you can *easily* define Extensions to the core model 
  7. its development is [open on GitHub](https://github.com/cityjson/specs/issues/), it is supported by a vibrant community, and everyone is welcome to contribute


## Contributing to the project 

We invite anyone to contribute to the development and improvement of CityJSON, all discussions, issues, and developments are open to everyone.


## If you use CityJSON in an academic context, please cite this article

Ledoux H, Arroyo Ohori K, Kumar K, Dukai B, Labetski A, Vitalis S (2019). CityJSON: A compact and easy-to-use encoding of the CityGML data model. **Open Geospatial Data, Software and Standards**, 4:4 [(DOI)](http://dx.doi.org/10.1186/s40965-019-0064-0) [(PDF)](https://opengeospatialdata.springeropen.com/track/pdf/10.1186/s40965-019-0064-0)

