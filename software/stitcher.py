
import os
import json
import jsonschema
import jsonref

# root_schema = '/Users/hugo/projects/cityjson/schema/v09/cityjson.json'
# fins = open(root_schema)

# abs_path = os.path.abspath(os.path.dirname(root_schema))
# base_uri = 'file://{}/'.format(abs_path)

jsotf = {}
jsotf["$schema"] = "http://json-schema.org/draft-07/schema#"
jsotf["type"] = "object"
jsotf["$ref"] = "file:///Users/hugo/projects/cityjson/schema/v09/extensions/noise2.json#/extraCityObjects/+NoiseBuilding"
base_uri = 'file:///Users/hugo/projects/cityjson/schema/v09/extensions/'

js = jsonref.loads(json.dumps(jsotf), jsonschema=True, base_uri=base_uri)

print(js)

#-- output stitched_schema
json_str = json.dumps(js, indent=2)
f = open('/Users/hugo/temp/out.json', "w")
f.write(json_str)





