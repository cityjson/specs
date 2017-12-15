import json
import jsonschema
import jsonref
import os
import urlparse
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

    filename = '/Users/hugo/projects/cityjson/example-datasets/dummy-values/example.json'
    # filename = '/Users/hugo/projects/cityjson/example-datasets/dummy-values/invalid.json'
    # filename = '../../../example-datasets/dummy-values/invalid.json'
    # filename = '/Users/hugo/temp/schemas/myfile.json'
    fin = open(filename)

    try:
        j = json.loads(fin.read(), object_pairs_hook=dict_raise_on_duplicates)
    except:
        print "ERROR: Duplicate keys!"
        return

    schema = '/Users/hugo/projects/cityjson/schema/v06/cityjson.schema.json'
    # schema = '/Users/hugo/temp/schemas/schema2.json'
    fin2 = open(schema)
    jtmp = json.loads(fin2.read())
    fin2.seek(0)

    if "$id" in jtmp:
        print "$id: ", jtmp['$id']
        u = urlparse.urlparse(jtmp['$id'])
        os.path.dirname(u.path)
        base_uri = u.scheme + "://" + u.netloc + os.path.dirname(u.path) + "/" 
    else:
        print "$id not defined, using local files"
        abs_path = os.path.dirname(schema)
        base_uri = 'file://{}/'.format(abs_path)
    print base_uri


    js = jsonref.loads(fin2.read(), jsonschema=True, base_uri=base_uri)
    # pprint(js)
    
    print "="*10    
    
    try:
        jsonschema.validate(j, js)
    except jsonschema.ValidationError as e:
        print "ERROR:", e.message
    except jsonschema.SchemaError as e:
        print "ERROR:", e

    # for id in j["CityObjects"]:
    #     print j['CityObjects'][id]['type']




if __name__ == '__main__':
    main()


