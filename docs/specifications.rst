=======================
CityJSON specifications
=======================

.. highlights::
   (version 0.1)

.. contents:: :local:

---------------
CityJSON Object
---------------

A CityJSON object represents one 3D city model of a given area, this model may contain features of different types, as defined in the CityGML data model.

- A CityJSON object is a JSON object.
- A CityJSON object must have 4 members: 

  #. one member with the name "type", whose value must be "CityModel";
  #. one member with the name "version", whose value must be a URL pointing to the CityJSON version;
  #. one member with the name "CityObjects". The value of this member is a collection of key-value pairs, where the key is the ID of the object, and the value is one City Object.
  #. one member with the name "vertices", whose value is an array of coordinates of each vertex of the city model. Their position in this array (0-based) is used as an index to be referenced by the Geometric Objects.

- A CityJSON may have one member with the name "metadata", whose value may contain JSON objects describing the coordinate reference system used, the extent of the dataset, its creator, etc.
- A CityJSON may have one member with the name "transform", whose value must contain 2 JSON objects describing how to *decompress* the coordinates. Transform is used to reduce the file size only.
- A CityJSON may have one member with name "appearance", the value may contain JSON objects representing the textures and/or materials of surfaces.
- A CityJSON must not have other members.

The minimal valid CityJSON object is thus:

.. code-block:: js

  {
    "type": "CityModel",
    "version": "http://www.cityjson.org/version/0.1",
    "CityObjects": {},
    "vertices": []
  }


An empty CityJSON will look like this:

.. code-block:: js

  {
    "type": "CityModel",
    "version": "http://www.cityjson.org/version/0.1",
    "metadata": {},
    "transform": {
      "scale": [],
      "translate": []
    },
    "CityObjects": {},
    "vertices": [],
    "appearance": {}
  }


--------
Metadata
--------

The metadata related to the 3D city model may be stored.

"crs" (Coordinate reference system)
```````````````````````````````````

The CRS may be given with a JSON object that must contain one member "epsg" with as value the `EPSG code <https://epsg.io>`_, as an integer.
For instance, for the WGS84 latitude-longitude:

.. code-block:: js

  "metadata": {
    "crs": { 
      "epsg": 7415
    }
  }


Be aware that the EPSG code should be a 3D CRS, ie the elevation/height values should be with respect to a specific datum.

It is not possible to give a WKT string with the parameters, or any other way.


"bbox" (extent of the dataset)
``````````````````````````````

While this can be extracted from the dataset itself, it is useful to store it. 
It may be stored as an array with 6 values: [minx, miny, minz, maxx, maxy, maxz]

.. code-block:: js

  "metadata": {
    "bbox": [ 84710, 446846, -5, 84757, 446944, 40 ]
  }


"keywords"
``````````

An array of keywords of type "string" may be listed:

.. code-block:: js

  "metadata": {
    "keywords": [ "energy", "solar potential" ]
  }


Other properties
````````````````

The following are all of type "string":

- "title"
- "abstract"
- "dataUrl"
- "metadataUrl"
- "dateOfCreation"
- "dateOfLastRevision"
- "dateOfPublication"
- "copyright"

.. note::

  It should be noticed that JSON does not have a date type, thus for all dates in a CityJSON document the following should be used: "YYYY-MM-DD" (as a string).






