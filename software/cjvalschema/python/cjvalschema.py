import os
import sys
import json
import jsonschema
import jsonref
import urlparse
from pprint import pprint


isValid = True
woWarnings = True

def dict_raise_on_duplicates(ordered_pairs):
    d = {}
    for k, v in ordered_pairs:
        if k in d:
           raise ValueError("Duplicate key for City Object IDs: %r" % (k,))
        else:
           d[k] = v
    return d

def byebye():
    global isValid
    global woWarnings
    sys.stdout.write("\n==========\n")
    if (isValid == True):
        sys.stdout.write("File is VALID")
        if (woWarnings == False):
            sys.stdout.write(", but has warnings.\n")
        else:
            sys.stdout.write(".\n")
    else:
        sys.stdout.write('File is INVALID\n')
    sys.stdout.write('\n')


def building_parts(j):
    isValid = True
    for id in j["CityObjects"]:
        # print j['CityObjects'][id]['type']
        if (j['CityObjects'][id]['type'] == 'Building') and ('Parts' in j['CityObjects'][id]):
            for each in j['CityObjects'][id]['Parts']:
                if (each in j['CityObjects']) and (j['CityObjects'][each]['type'] == 'BuildingPart'):
                    pass
                else:
                    sys.stdout.write("ERROR: BuildingPart #" + each + " doesn't exist.\n")
                    sys.stdout.write("\t(Building #" + id + " references it)\n")    
                    isValid = False
    return isValid


def building_installations(j):
    isValid = True
    for id in j["CityObjects"]:
        # print j['CityObjects'][id]['type']
        if (j['CityObjects'][id]['type'] == 'Building') and ('Installations' in j['CityObjects'][id]):
            for each in j['CityObjects'][id]['Installations']:
                if (each in j['CityObjects']) and (j['CityObjects'][each]['type'] == 'BuildingInstallation'):
                    pass
                else:
                    sys.stdout.write("ERROR: BuildingInstallation #" + each + " doesn't exist.\n")
                    sys.stdout.write("\t(Building #" + id + " references it)\n")    
                    isValid = False
    return isValid


def building_pi_parent(j):
    isValid = True
    pis = set()
    for id in j["CityObjects"]:
        if j['CityObjects'][id]['type'] == 'BuildingPart' or j['CityObjects'][id]['type'] == 'BuildingInstallation':
            pis.add(id)
    for id in j["CityObjects"]:
        if j['CityObjects'][id]['type'] == 'Building':
            if 'Parts' in j['CityObjects'][id]:
                for pid in j['CityObjects'][id]['Parts']:
                    pis.remove(pid)
        if j['CityObjects'][id]['type'] == 'Building':
            if 'Installations' in j['CityObjects'][id]:
                for pid in j['CityObjects'][id]['Installations']:
                    if pid in pis:
                        pis.remove(pid)
    if len(pis) > 0:
        isValid = False
        sys.stdout.write("ERROR: BuildingParts and/or BuildingInstallations don't have a parent:\n")
        for each in pis:
            sys.stdout.write("\t#" + each + "\n")
    return isValid


def main():
    global isValid
    global woWarnings

    filename = '../../../example-datasets/dummy-values/example2.json'
    # filename = '../../../example-datasets/dummy-values/invalid.json'
    # filename = '../../../example-datasets/dummy-values/invalid3.json'
    # filename = '/Users/hugo/temp/schemas/myfile.json'
    fin = open(filename)
    print "Input file:", os.path.abspath(filename)
    
    #-- check if CityObjects have duplicate keys at reading time
    #-- otherwise it's too late: one object has been overwritten!
    try:
        j = json.loads(fin.read(), object_pairs_hook=dict_raise_on_duplicates)
    except ValueError, Argument:
        print "ERROR: ", Argument
        isValid = False
        byebye()
        return

    #-- make sure it's a CityJSON file
    if (j["type"] != "CityJSON"):
        print "ERROR: not a CityJSON file"
        isValid = False
        byebye()
        return

    #-- fetch proper schema
    if j["version"] == "0.6":
        schema = '../../../schema/v06/cityjson.schema.json'
    elif j["version"] == "0.5":
        schema = '../../../schema/cityjson-v05.schema.json'
    else:
        sys.stdout.write("ERROR: version not supported.\n")
        isValid = False
        byebye()
        return
    fins = open(schema)
    jtmp = json.loads(fins.read())
    fins.seek(0)
    if "$id" in jtmp:
        # print "$id: ", jtmp['$id']
        u = urlparse.urlparse(jtmp['$id'])
        os.path.dirname(u.path)
        base_uri = u.scheme + "://" + u.netloc + os.path.dirname(u.path) + "/" 
    else:
        # print "$id not defined, using local files"
        abs_path = os.path.abspath(os.path.dirname(schema))
        base_uri = 'file://{}/'.format(abs_path)
    print "Schema used:", os.path.abspath(schema)
    js = jsonref.loads(fins.read(), jsonschema=True, base_uri=base_uri)
    
    print "="*10    
    
    #-- validate the file against the schema
    try:
        jsonschema.validate(j, js)
    except jsonschema.ValidationError as e:
        print "ERROR:", e.message
        isValid = False
        byebye()
        return
    except jsonschema.SchemaError as e:
        print "ERROR:", e
        isValid = False
        byebye()
        return


    if building_parts(j) == False:
        isValid = False
    if building_installations(j) == False:
        isValid = False
    if building_pi_parent(j) == False:
        isValid = False


    byebye()
    return

if __name__ == '__main__':
    main()


