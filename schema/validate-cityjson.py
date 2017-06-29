from jsonschema import validate
import json

fs = open('cityjson.schema.json')
cjschema = json.loads(fs.read())

fi = open('../example-datasets/dummy-values/example.json')
myfile = json.loads(fi.read())

validate(myfile, cjschema)

