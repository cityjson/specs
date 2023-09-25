

# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).


## [2.0.0] - 2023-09-25
- this version is basically v1.1 with small fixes (see below) and an improved/clearer text
- the version 2.0 was required by the OGC to be officially approved
### Changed
- `GenericCityObject` is back as a City Object in the data model, because it makes life easy for everyone
- it is now possible to define extra Semantic Surfaces in Extensions (with `"extraSemanticSurfaces"`)
- `"metadata/pointOfContact"` is now a JSON object, like other addresses for Buildings/Bridges
- the text of the specs has been improved and harmonised (there were some inconsistencies, for instance with addresses for City Objects and metadata)
- the values of *u* and *v* for textures are allowed to be outside [0.0, 1.0] to allow repeated patterns.


## [1.1.3] - 2022-11-25
### Changed
- CityJSONFeatures now do not have to dereference the GeometryTemplates, those can be in the "1st line"/metadata. Dereferencing is still allowed. cjio passes the GeometryTemplates in the 1st-line now. 
- `CityJSONFeature`: now clear that all children (recursively) of a feature should be bundled with its parent.
- improved the text of the specifications at several places, and fixed sentences that were not precise enough.
- change the `"uri"` to `"url"` for the Extensions, and now the schemas can have either of these. Going forward `"url"` is the preferred way.
- restructured the text for the Extensions, now a 4th use-case is added: "Defining a new Semantic Object". It was always there but not listed as a case. Now fixed.
### Added
- `"BuildingConstructiveElement"` was missing from the schemas as a CO, now added.
- added 2 suggested conventions in the specs: file extensions `.city.json` and `.city.jsonl`


## [1.1.2] - 2022-08-16
Minor changes to the schemas for errors, improved the text of the specs so that it's easier for everyone to read and understand.
### Changed
- fixed the typo in the schema: `"TunnelFurniture"`
- added the missing CityGML v3 types in the schemas: `"BridgeFurniture"` and `"Waterway"` and `"BuildingConstructiveElement"`
- added a link in the specs text to the official CityGML v3 specs for definitions of each of the City Objects
- improved and added a few sentences in the specs to make them clearer


## [1.1.1] - 2022-02-02
Minor changes to the schemas for omissions/errors, harmonised the specs text slightly.
### Removed
- `trafficDirection` attribute has been removed (https://github.com/cityjson/specs/issues/107)
### Changed
- added Semantic types for interior surfaces: "FloorSurface", "InteriorWallSurface", and "CeilingSurface".
- the restrictions for the geometry types for different City Objects are harmonised
- MultiPoint and MultiLineString can have semantics (it was in the text but not in the schema)
- added "address" property for BuildingUnit in the schema (was already described in the text)
- the property "children_roles" of "CityObjectGroup" can have NULL values if not used
- the specs text now is correct for the appearance: `"vertices-texture"` and not `"vertex-texture"`

## [1.1.0] - 2021-12-01
### Added
- add support for CityGML v3.0.0: 
  - add interior of Buildings/Tunnels/Bridges classes, eg `BuildingStorey`, `BuildingUnit`, `BuildingRoom`, `BridgeRoom` 
  - add `OtherConstruction`
  - updated Transportation classes to be compliant
- `CityJSONFeature` is defined, which can be used for streaming and handling large files
### Removed
- `GenericCityObject` has been removed, Extensions should be used instead
### Changed
- change the property `"lod"` of geometries from number to string 
- core metadata is smaller, extended/advanced metadata are now in the MetadataExtended Extension: https://github.com/cityjson/metadata-extended
- CityJSON files are always compressed, that is their vertices are integers and the `"transform"` property is mandatory
- City Objects do not have to have a `"geometry"` property anymore
- `CityObjectGroup` has the role added as a property, to define what the role of each object in the group is
- CRS now use the OGC Name Type Specification (https://docs.opengeospatial.org/pol/09-048r5.html)


## [1.0.3] - 2021-07-06
### Changed
- fixed bugs in metadata.schema.json that made files with Extensions not having valid "metadata" (if certain properties were used)
- (specification text stays exactly the same)


## [1.0.2] - 2021-04-21
### Changed
- small improvements in the specs document to make concepts clearer
- schemas are the same, with some simplifications to make their maintenance easier in the future


## [1.0.1] - 2019-07-09
### Changed
- schemas go to json-schema version draft-07; will not change much in practice
- a "minified" schema is now available, easier to validate with external tools
- small improvements in the specs document: figure for City Objects, clarifications for parent-children, TINRelief can be any CompositeSurface


## [1.0.0] - 2019-04-26
### Changed
- [Semantic versioning](https://semver.org/) is from on now used, with MAJOR.MINOR.PATCH
- The `"extensions"` property now documents the version of the Extension that is used for this file; since an Extension will most likely be updated it should be possible to link to a specific version (X.Y)
- new logo!


## [0.9] - 2019-01-28
### Changed
- the `"parent"` property of City Objects is now named `"parents": []`, and is an array. This is to allow new City Objects in Extensions to have more than one parent; for the core objects this doesn't change anything (except that the property must be renamed and put an array; cjio `upgrade_version()` takes care of this)
- the schemas have been revamped. The content is the same, but there are now abstract objects and city objects reuse these. It's thus way easier to modify and update and understand the schemas
- the Extensions are not schemas files anymore, but JSON files with a specific syntax. The idea is the same, but now creating Extensions is simpler.
- Extensions now allow new attributes for already existing City Objects, ie it's possible to add an attribute "+myattribute" to a "Building" and document it in a schema. Before it was only possible if one created a new specific City Object.
- Extensions now allow root properties to be added and documented.
- Transportation module now has LoD0 with MultiLineString geometries.


## [0.8] - 2018-09-24
### Added
- Extensions, aka ADEs in the CityGML world, are now supported. The page 'Extensions' explains how to create one (its schema), how to validate your data, etc.

### Changed
- introduction of a concept that was implicit but never mentioned: City Objects can be either of type '1st-level' or '2nd-level' (a Building would be 1st-level, and a BuildingPart 2nd-level). This concept is now for all City Object, including those that are defined in an Extension. This means that the properties "Parts" and "Installations" dissapear from the schema of Building/Bridge/Tunnel, and are replaced by a single one: "children". Also, a child (2nd-level City Object) needs to have a link to its parent City Object (with "parent").
- metadata/crs and metadata/bbox have been replaced by metadata/geographicReference and metadata/geographicExtent. Longer to type (file sizes will increase!), but ISO19115-compliant. Also, bbox-&gt;geographicExtent for each of the City Object too. cjio's upgrade\_version() takes care of all these automatically.
- metadata has more possibilities, it's been extended to 3D city models needs. Some fields from the previous metadata properties were modified, be aware.
- Observe that the software cjio (<https://github.com/tudelft3d/cjio>) offers an option `--upgrade\_version` (from v0.6 --&gt; v0.8) that takes care of the changes above (except the metadata, you'll need to adjust manually, sorry I am not coding all this). Simply run a v0.6 file and it's upgraded.


## [0.6] - 2018-04-11
### Added
- support for Geometry Templates (aka Implicit Geometries)
- support for City Object Groups
- any members at the root of CityJSON are now allowed (but might be ignored by parser)
- each City Object can have a "bbox" member

### Changed
- the schema is now split into different schemas that are linked together
- the schema validator (software cjvalschema) is now in Python and improved, thus everyone can run it easily
- cjcompress does a better job and bugs have been removed
- textures are not forced to be in the folder 'appearances' anymore, any link to an image will do (useful for WFS for instance).


## [0.5] - 2017-11-14
### Added
- handling of `null` values at any level in a nested arrays is now supported in the schema
- CityGML module 'Transportation' added
- CityGML module 'Bridge' added, thus the 4 classes: Bridge, BridgePart, BridgeInstallation, and BridgeConstructionElement
- CityGML module 'Tunnel' added, thus the 4 classes: Tunnel, TunnelPart, and TunnelInstallation

## Changed
- the way semantics is stored for the surface is completely changed and breaks previous version
- material now can be per surface `"values": []` or `"value": 2`


## [0.3] - 2017-10-12
### Added
- utility cityjson-valschema now ensures that no two City Objects have the same ID/key
- the schema now has depths of Geometry Objects for "texture", "material", and "semantics" arrays.
- the schema and the validator are generally better
- clear use of null and empty {} where appropriate

### Changed
- change to "header" and versioning of the file
- 1 textures and materials per geometry


## [0.2] - 2017-09-05
### Added
- Semantics Objects, so that Semantic Surfaces have specific attributes
- software to validate the schema + other properties impossible represent with the schema, eg warning users of "soft" errors like attributes not in CityGML

### Changed
- metadata now ISO19115 compliant 💥
- materials now use X3D mechanism, same as CityGML
- textures now use COLLADA mechanism, same as CityGML
- improved greatly the schema (more is validated)


## [0.1] - 2017-08-01
### Added
- first beta release of CityJSON


[2.0.0]: https://github.com/tudelft3d/cityjson/compare/1.1.3...2.0.0
[1.1.3]: https://github.com/tudelft3d/cityjson/compare/1.1.2...1.1.3
[1.1.2]: https://github.com/tudelft3d/cityjson/compare/1.1.1...1.1.2
[1.1.1]: https://github.com/tudelft3d/cityjson/compare/1.1.0...1.1.1
[1.1.0]: https://github.com/tudelft3d/cityjson/compare/1.0.3...1.1.0
[1.0.3]: https://github.com/tudelft3d/cityjson/compare/1.0.2...1.0.3
[1.0.2]: https://github.com/tudelft3d/cityjson/compare/1.0.1...1.0.2
[1.0.0]: https://github.com/tudelft3d/cityjson/compare/0.9...1.0.0
[0.9]:   https://github.com/tudelft3d/cityjson/compare/0.8...0.9
[0.8]:   https://github.com/tudelft3d/cityjson/compare/0.6...0.8
[0.6]:   https://github.com/tudelft3d/cityjson/compare/0.5...0.6
[0.5]:   https://github.com/tudelft3d/cityjson/compare/0.3...0.5
[0.3]:   https://github.com/tudelft3d/cityjson/compare/0.2...0.3
[0.2]:   https://github.com/tudelft3d/cityjson/compare/0.1...0.2
[0.1]:   https://github.com/tudelft3d/cityjson/releases/0.1
