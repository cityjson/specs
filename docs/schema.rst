=================
Schema validation
=================

CityJSON schema download
------------------------

`Download all schemas on GitHub <https://github.com/tudelft3d/cityjson/tree/master/schema/>`_


Software to validate a file against the schema
----------------------------------------------

The software `cjio <https://github.com/tudelft3d/cjio>`_ can validate a CityJSON against the schema and does extra validation, eg:

- are the attributes of City Objects and metadata according to CityGML?
- do the BuildingParts have a parent Building?
- do the BuildingInstallation have a parent Building?
- are the arrays for the boundaries and the semantics coherent? (same structure)
- are there duplicate keys (IDs) for the City Objects? If yes then an error is returned.
- are there duplicate or orphan vertices? If yes a warning is returned

To install you need Python 3 and just ``pip install cjio``, the schemas of versions 0.6+ are included with cjio.

To validate a file: 

``cjio myfile.json validate``

To validate against schemas stored locally:

``cjio myfile.json validate --folder_schemas /home/elvis/cityjson/schema/v09/``


Validation of an Extension
--------------------------

We provide a small Python program (:download:`download validate-extension.py <../software/validate-extension/validate-extension.py>`) that verifies that the Extensions are according to the JSON schemas for the Extension files (:download:`download extension.schema.json <../schema/v09/extensions/extension.schema.json>`)

