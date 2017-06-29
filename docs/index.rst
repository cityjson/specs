.. CityJSON documentation master file, created by
   sphinx-quickstart on Thu Jun 22 17:47:16 2017.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

CityJSON
========

(version |release|)

.. code-block:: js

  {
    "type": "CityModel",
    "version": "http://www.cityjson.org/version/0.1",
    "metadata": {
      "crs": { "epsg": 7415 }
    },
    "CityObjects": {
      "id-1": {
        "type": "Building",
        "geometry": [...]
        ...
      }
    },
    "vertices": [
      [23.1, 2321.2, 11.0],
      ...
    ],
    "appearance": {...}
  }

CityJSON is a format for encoding a subset of the `CityGML <https://www.citygml.org>`_ data model using JavaScript Object Notation (`JSON <http://json.org>`_).
A CityJSON file represents both the geometry and the semantics of the city features of a given area, eg buildings, roads, rivers, the vegetation, and the city furniture.

The aim of CityJSON is to offer an alternative to the GML encoding of CityGML, which can be verbose and complex (and thus rather frustrating to work with). 
CityJSON aims at being easy-to-use, both for reading datasets, and for creating them.
It was designed with programmers in mind, so that tools and APIs supporting it can be quickly built.
It was also designed to be compact (we have noticed a factor at least 4X, often 10X+), and friendly for web and mobile development.

A CityJSON object, representing a city, is as 'flat' as possible, ie the hierarchy of CityGML has been flattened out and only the city objects which are 'leaves' of this hierarchy are implemented.
This considerably simplifies the storage of a city model, and furthermore does not mean that information is lost.

A JSON encoding of GML, huh?!?
------------------------------

While its name otherwise implies, CityGML is not only a GML encoding, but is actually an open standardised data model to store digital 3D models of cities and landscapes. 
It defines ways to describe most of the common 3D features found in cities, and the relationships between them. 
It also defines different standard levels of detail (LODs) for the 3D objects, which allows us to represent objects for different applications and purposes.

CityGML currently has 2 implementations:

  1. the GML encoding is the "standard" one, and is defined in the `offical documentation <https://portal.opengeospatial.org/files/?artifact_id=47842>`_
  1. a database schema called `3DCityDB <http://www.3dcitydb.org>`_, which can be implemented both for `PostgreSQL <https://www.postgresql.org>`_ and `Oracle Spatial <https://www.oracle.com/database/spatial/index.html>`_.

CityJSON can be considered as the third implementation of the CityGML data model.


Content
-------

.. toctree::
   :maxdepth: 2

   citygmlsupport
   specifications
   validation
   software
   datasets
   about

