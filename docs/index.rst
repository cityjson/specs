.. CityJSON documentation master file, created by
   sphinx-quickstart on Thu Jun 22 17:47:16 2017.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

CityJSON
========
 
CityJSON is a format for encoding a subset of the `CityGML <https://www.citygml.org>`_ data model using JavaScript Object Notation (`JSON <http://json.org>`_).
A CityJSON file represents both the geometry and the semantics of the city features of a given area, eg buildings, roads, rivers, the vegetation, and the city furniture.

The aim of CityJSON is to offer an alternative to the GML encoding of CityGML, which can be verbose and complex. 
CityJSON aims at being easy-to-use, both for creating datasets, and for reading/parsing them.
It was designed with programmers in mind, so that tools and APIs supporting it can be quickly built.
It was also designed to be compact.

A CityJSON object, representing a city, is as 'flat' as possible, ie the hierarchy of CityGML has been flattened out and only the city objects which are 'leaves' of this hierarchy are implemented.
This considerably simplifies the storage of a city model, and furthermore does not mean that information is lost.


.. toctree::
   :maxdepth: 1
   :caption: Contents:

   software

a JSON-based implementation of CityGML, easy-to-use and compact
---------------------------------------------------------------

Bacon ipsum dolour sit amet porchetta beef turkey, bacon turducken boudin hamburger venison ball tip. Brisket pork loin bresaola short loin ground round leberkas pastrami tongue jerky cow turducken beef ribs. Pork ribeye landjaeger prosciutto pig venison tenderloin. Swine beef ribs kielbasa, porchetta tenderloin salami venison pork belly tail.


Indices and tables
------------------

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
