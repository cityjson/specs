
# CityJSON

CityJSON is a format for encoding a subset of the [CityGML](https://www.citygml.org) data model using JavaScript Object Notation [JSON](http://json.org).
A CityJSON file represents both the geometry and the semantics of the city features of a given area, eg buildings, roads, rivers, the vegetation, and the city furniture.

The aim of CityJSON is to offer an alternative to the GML encoding of CityGML, which can be verbose and complex (and thus rather frustrating to work with). 
CityJSON aims at being easy-to-use, both for reading datasets, and for creating them.
It was designed with programmers in mind, so that tools and APIs supporting it can be quickly built.
It was also designed to be compact (we have noticed a factor at least 4X, often 10X+), and friendly for web and mobile development.

A CityJSON object, representing a city, is as 'flat' as possible, ie the hierarchy of CityGML has been flattened out and only the city objects which are 'leaves' of this hierarchy are implemented.
This considerably simplifies the storage of a city model, and furthermore does not mean that information is lost.

[All details on the website](http://www.cityjson.org)