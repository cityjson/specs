.. CityJSON documentation master file, created by
   sphinx-quickstart on Thu Jun 22 17:47:16 2017.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

========
CityJSON
========

(version |release|)


.. code-block:: js

  {
    "type": "CityJSON",
    "version": "0.9",
    "extensions": { "Noise": "https://someurl.org/noise.json" },
    "metadata": { "referenceSystem": "urn:ogc:def:crs:EPSG::7415" },
    "CityObjects": {
      "id-1": {
        "type": "Building",
        "attributes": { "roofType": "gable" },
        "geometry": [{
          "type": "Solid",
          "lod": 2,
          "boundaries": []
        }]
      },
      "id-56": {...}
    },
    "vertices": [
      [23.1, 2321.2, 11.0],
      [14.0, 2299.5, 14.0],
      ...
    ],
    "appearance": {
      "textures": []
    },
    "geometry-templates": {}
  }

CityJSON is a format for encoding a subset of the `CityGML <https://www.citygml.org>`_ data model (version 2.0.0) using JavaScript Object Notation (`JSON <http://json.org>`_).
A CityJSON file represents both the geometry and the semantics of the city features of a given area, eg buildings, roads, rivers, the vegetation, and the city furniture.

The aim of CityJSON is to offer an alternative to the GML encoding of CityGML, which can be verbose and complex (and thus rather frustrating to work with). 
CityJSON aims at being easy-to-use, both for reading datasets, and for creating them.
It was designed with programmers in mind, so that tools and APIs supporting it can be quickly built.
It was also designed to be compact (it compresses with a factor of around 7X the CityGML files publicly available, see :ref:`compression_factors`), and to be friendly for web and mobile development.

A CityJSON object, representing a city, is as 'flat' as possible, ie the hierarchy of CityGML has been flattened out and only the city objects which are 'leaves' of this hierarchy are implemented.
This considerably simplifies the storage of a city model, and furthermore does not mean that information is lost.

As of v0.8, CityJSON supports the extension of the core modules in a structured way (new city objects and complex attributes), see the page :doc:`extensions` for all the details. 


.. rubric:: A JSON encoding of GML, huh?!?

While its name otherwise implies, CityGML is not only a GML encoding, but is actually an open standardised data model to store digital 3D models of cities and landscapes. 
It defines ways to describe most of the common 3D features found in cities, and the relationships between them. 
It also defines different standard levels of detail (LoDs) for the 3D objects, which allows us to represent objects for different applications and purposes.

CityGML currently has 2 implementations:

  #. the GML encoding is the only official one, and is defined in the `official documentation <https://portal.opengeospatial.org/files/?artifact_id=47842>`_
  #. a database schema called `3DCityDB <http://www.3dcitydb.org>`_, which can be implemented both for `PostgreSQL <https://www.postgresql.org>`_ and `Oracle Spatial <https://www.oracle.com/database/spatial/index.html>`_.

CityJSON can be considered as the third implementation of the CityGML data model.


.. note::       
   CityJSON is not an official standard, and it is not part of the OGC standards (unlike its parent standard CityGML).
   It was started, and is maintained, by the `3D geoinformation group at TU Delft <https://3d.bk.tudelft.nl>`_  

   We invite others to contribute, all `discussions/issues <https://github.com/tudelft3d/cityjson/issues>`_ and developments are open to everyone on the `GitHub repository of CityJSON <https://github.com/tudelft3d/cityjson>`_.


.. rubric:: Content

.. toctree::
   :maxdepth: 2

   specs
   schema
   software
   extensions
   citygmlsupport
   datasets
   changelog
   contact

