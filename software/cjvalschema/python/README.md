
# Validation of a CityJSON file against its schema

Requires:

  1. https://pypi.python.org/pypi/jsonschema
  2. https://pypi.python.org/pypi/jsonref

Simply run the file and the correct schema (only v0.5+) will be fetched automatically from `../../../schemas`

    $ python cjvalschema delft.json
