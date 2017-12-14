import json
import jsonschema
from jsonref import JsonRef
from pprint import pprint

def dict_raise_on_duplicates(ordered_pairs):
    """Reject duplicate keys."""
    d = {}
    for k, v in ordered_pairs:
        if k in d:
           raise ValueError("duplicate key: %r" % (k,))
        else:
           d[k] = v
    return d

def main():
    # filename = '../../../example-datasets/dummy-values/example.json'
    filename = '../../../example-datasets/dummy-values/invalid.json'
    fin = open(filename)

    try:
        j = json.loads(fin.read(), object_pairs_hook=dict_raise_on_duplicates)
    except:
        print "ERROR: Duplicate keys!"
        return

    # schema = '../../../schema/cityjson-v06.schema.json'
    schema = '/Users/hugo/temp/schemas/schema.json'
    js = json.loads(open(schema).read())
    pprint(JsonRef.replace_refs(js))

# with open(os.path.join(absolute_path_to_base_directory, base_filename)) as file_object:
#     schema = json.load(file_object)
# resolver = jsonschema.RefResolver('file://' + absolute_path_to_base_directory + '/', schema)
# jsonschema.Draft4Validator(schema, resolver=resolver).validate(data)
    
    try:
        jsonschema.validate(j, js)
    except jsonschema.ValidationError as e:
        print e.message
    except jsonschema.SchemaError as e:
        print e

    for id in j["CityObjects"]:
        print j['CityObjects'][id]['type']




if __name__ == '__main__':
    main()


