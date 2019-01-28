
=========
Changelog
=========

.. http://keepachangelog.com/en/1.0.0/

[Unreleased - coming in the future]
-----------------------------------

- handling of very large files, eg for streaming (see https://github.com/tudelft3d/cityjson/issues/6)
- binary encoding of JSON, with either `CBOR <http://cbor.io>`_ or `BSON <http://bsonspec.org>`_

----

[0.9] - 2019-01-28
------------------

Changed
*******

- the ``"parent"`` property of City Objects is now named ``"parents": []``, and is an array. This is to allow new City Objects in Extensions to have more than one parent; for the core objects this doesn't change anything (except that the property must be renamed and put an array; cjio ``upgrade_version()`` takes care of this)
- the schemas have been revamped. The content is the same, but there are now abstract objects and city objects reuse these. It's thus way easier to modify and update and understand the schemas
- the Extensions are not schemas files anymore, but JSON files with a specific syntax. The idea is the same, but now creating Extensions is simpler.
- Extensions now allow new attributes for already existing City Objects, ie it's possible to add an attribute "+myattribute" to a "Building" and document it in a schema. Before it was only possible if one created a new specific City Object.
- Extensions now allow root properties to be added and documented.
- Transportation module now has LoD0 with MultiLineString geometries.

----

[0.8] - 2018-09-24
------------------
Added
*****

- Extensions, aka ADEs in the CityGML world, are now supported. The page 'Extensions' explains how to create one (its schema), how to validate your data, etc.

Changed
*******

- introduction of a concept that was implicit but never mentioned: City Objects can be either of type '1st-level' or '2nd-level' (a Building would be 1st-level, and a BuildingPart 2nd-level). This concept is now for all City Object, including those that are defined in an Extension. This means that the properties "Parts" and "Installations" dissapear from the schema of Building/Bridge/Tunnel, and are replaced by a single one: "children". Also, a child (2nd-level City Object) needs to have a link to its parent City Object (with "parent").
- metadata/crs and metadata/bbox have been replaced by metadata/geographicReference and metadata/geographicExtent. Longer to type (file sizes will increase!), but ISO19115-compliant. Also, bbox->geographicExtent for each of the City Object too. cjio's upgrade_version() takes care of all these automatically.
- metadata has more possibilities, it's been extended to 3D city models needs. Some fields from the previous metadata properties were modified, be aware. 
- Observe that the software cjio (https://github.com/tudelft3d/cjio) offers an option '--upgrade_version' (from v0.6 --> v0.8) that takes care of the changes above (except the metadata, you'll need to adjust manually, sorry I am not coding all this). Simply run a v0.6 file and it's upgraded.

----

[0.6] - 2018-04-11
------------------

Added
*****

- support for Geometry Templates (aka Implicit Geometries)
- support for City Object Groups
- any members at the root of CityJSON are now allowed (but might be ignored by parser)
- each City Object can have a "bbox" member

Changed
*******

- the schema is now split into different schemas that are linked together
- the schema validator (software cjvalschema) is now in Python and improved, thus everyone can run it easily
- cjcompress does a better job and bugs have been removed
- textures are not forced to be in the folder 'appearances' anymore, any link to an image will do (useful for WFS for instance).


[0.5] - 2017-11-14 
------------------

Added
*****
- handling of ``null`` values at any level in a nested arrays is now supported in the schema
- CityGML module 'Transportation' added
- CityGML module 'Bridge' added, thus the 4 classes: Bridge, BridgePart, BridgeInstallation, and BridgeConstructionElement 
- CityGML module 'Tunnel' added, thus the 4 classes: Tunnel, TunnelPart, and TunnelInstallation

Changed
*******
- the way semantics is stored for the surface is completely changed and breaks previous version
- material now can be per surface ``"values": []`` or ``"value": 2`` 

----

[0.3] - 2017-10-12
------------------

Added
*****
- utility cityjson-valschema now ensures that no two City Objects have the same ID/key
- the schema now has depths of Geometry Objects for "texture", "material", and "semantics" arrays.
- the schema and the validator are generally better
- clear use of null and empty {} where appropriate

Changed
*******
- change to "header" and versioning of the file
- >1 textures and materials per geometry

----

[0.2] - 2017-09-05
------------------

Added
*****
- Semantics Objects, so that Semantic Surfaces have specific attributes 
- software to validate the schema + other properties impossible represent with the schema, eg warning users of "soft" errors like attributes not in CityGML

Changed
*******
- metadata now ISO19115 compliant 💥
- materials now use X3D mechanism, same as CityGML
- textures now use COLLADA mechanism, same as CityGML
- improved greatly the schema (more is validated) 

----

[0.1] - 2017-08-01 
------------------
Added
*****
- first beta release of CityJSON


