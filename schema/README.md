 # Schema to validate a CityJSON file

 [https://json-schema.org](https://json-schema.org) is used to validate.

 To validate a file, either [install a validator](https://json-schema.org/implementations.html) or use the [online validator](https://jsonschemalint.com/#/version/draft-04/markup/json) and copy the schema in the left window.

 The simplest way is perhaps using Python:

    $ pip install jsonschema
    $ jsonschema -i ../example-datasets/dummy-values/example.json cityjson.schema.json

Should return nothing (no errors!). 