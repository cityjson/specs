=================
Schema validation
=================

CityJSON schema download
------------------------

version 0.7: 
- see `metadata.json <https://github.com/tudelft3d/cityjson/tree/master/schema>`_

version 0.6: 
- :download:`download cityjson-v06.schema.zip <../schema/v06/cityjson-v06-schema.zip>`

version 0.5: 
- :download:`download cityjson-v05.schema.json <../schema/cityjson-v05.schema.json>`

version 0.3: 
- :download:`download cityjson-v03.schema.json <../schema/cityjson-v03.schema.json>`

version 0.2: 
- :download:`download cityjson-v02.schema.json <../schema/cityjson-v02.schema.json>`

version 0.1:
- :download:`download cityjson-v01.schema.json <../schema/cityjson-v01.schema.json>`


Software to validate a file against the schema
----------------------------------------------

The software `cjio <https://github.com/tudelft3d/cjio>`_ can validate a CityJSON against the schema and does extra validation, eg:

- are the attributes of City Objects and metadata according to CityGML?
- do the BuildingParts have a parent Building?
- do the BuildingInstallation have a parent Building?
- are the arrays for the boundaries and the semantics coherent? (same structure)
- are there duplicate keys (IDs) for the City Objects? If yes then an error is returned.
- are there duplicate or orphan vertices? If yes a warning is returned

To install you need Python 3 and just `pip install cjio`

To validate a file: `cjio myfile.json validate`

