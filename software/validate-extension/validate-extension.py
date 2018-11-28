
import os
import sys
import json
import jsonschema
import jsonref

schema = '../../schema/v09/extensions/extension.schema.json'
js = json.loads(open(schema).read())

dataset = '/Users/hugo/projects/cityjson/schema/v09/extensions/misc.json'
j = json.loads(open(dataset).read())

try:
    jsonschema.validate(j, js)
except jsonschema.ValidationError as e:
    print ("ERROR:   ", e.message)
except jsonschema.SchemaError as e:
    print ("ERROR:   ", e)


