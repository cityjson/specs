import os
import sys
import json
import jsonref #-- works with json v0.2

#----------------------------
# cityjson.schema.json
#----------------------------
root_schema = os.path.abspath('../schemas/cityjson.schema.json')
fins = open(root_schema)

abs_path = os.path.abspath(os.path.dirname(root_schema))
base_uri = 'file://{}/'.format(abs_path)

js = jsonref.loads(fins.read(), jsonschema=True, base_uri=base_uri)

# -- output stitched_schema
json_str = jsonref.dumps(js, indent=2)
# json_str = jsonref.dumps(js, separators=(',',':')) #-- this doesn't work: WTF
opath = os.path.abspath('temp.json')
f = open(opath, "w")
f.write(json_str)

f2 = open('./temp.json')
j2 = json.loads(f2.read())
json_str2 = json.dumps(j2, separators=(',',':'))
f = open("../schemas/cityjson.min.schema.json", "w")
f.write(json_str2)


#----------------------------
# cityjsonfeature.schema.json
#----------------------------
root_schema = os.path.abspath('../schemas/cityjsonfeature.schema.json')
fins = open(root_schema)

abs_path = os.path.abspath(os.path.dirname(root_schema))
base_uri = 'file://{}/'.format(abs_path)

js = jsonref.loads(fins.read(), jsonschema=True, base_uri=base_uri)

# -- output stitched_schema
json_str = jsonref.dumps(js, indent=2)
# json_str = jsonref.dumps(js, separators=(',',':')) #-- this doesn't work: WTF
opath = os.path.abspath('temp.json')
f = open(opath, "w")
f.write(json_str)

f2 = open('./temp.json')
j2 = json.loads(f2.read())
json_str2 = json.dumps(j2, separators=(',',':'))
f = open("../schemas/cityjsonfeature.min.schema.json", "w")
f.write(json_str2)
