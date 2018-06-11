# +-------------------------------------------------------+
# |                                                       |
# |        ____ _ _             _ ____   ___  _   _       |
# |       / ___(_) |_ _   _    | / ___| / _ \| \ | |      |
# |      | |   | | __| | | |_  | \___ \| | | |  \| |      |
# |      | |___| | |_| |_| | |_| |___) | |_| | |\  |      |
# |       \____|_|\__|\__, |\___/|____/ \___/|_| \_|      |
# |                   |___/                               |
# |                                                       |
# +-------------------------------------------------------+
# +-------------------------------------------------------+
# |                +-+-+-+-+-+-+-+-+-+-+-+                |
# |                |c|j|v|a|l|s|c|h|e|m|a|                |
# |                +-+-+-+-+-+-+-+-+-+-+-+                |
# |                                                       |
# +-------------------------------------------------------+


from __future__ import print_function
import os
import sys
import json
import jsonschema
import jsonref
import urlparse
import argparse



def dict_raise_on_duplicates(ordered_pairs):
    d = {}
    for k, v in ordered_pairs:
        if k in d:
           raise ValueError("Duplicate key for City Object IDs: %r" % (k,))
        else:
           d[k] = v
    return d

def byebye(isValid, woWarnings):
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


def city_object_groups(j):
    isValid = True
    for id in j["CityObjects"]:
        # print j['CityObjects'][id]['type']
        if j['CityObjects'][id]['type'] == 'CityObjectGroup':
            for each in j['CityObjects'][id]['members']:
                if each in j['CityObjects']:
                    pass
                else:
                    sys.stdout.write("ERROR:   CityObjectGroup (#" + id + ") contains member #" + each + ", but it doesn't exist.\n") 
                    isValid = False
    return isValid


def building_parts(j):
    isValid = True
    for id in j["CityObjects"]:
        # print j['CityObjects'][id]['type']
        if (j['CityObjects'][id]['type'] == 'Building') and ('Parts' in j['CityObjects'][id]):
            for each in j['CityObjects'][id]['Parts']:
                if (each in j['CityObjects']) and (j['CityObjects'][each]['type'] == 'BuildingPart'):
                    pass
                else:
                    sys.stdout.write("ERROR:   BuildingPart #" + each + " doesn't exist.\n")
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
                    sys.stdout.write("ERROR:   BuildingInstallation #" + each + " doesn't exist.\n")
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
                    if pid in pis:
                        pis.remove(pid)
        if j['CityObjects'][id]['type'] == 'Building':
            if 'Installations' in j['CityObjects'][id]:
                for pid in j['CityObjects'][id]['Installations']:
                    if pid in pis:
                        pis.remove(pid)
    if len(pis) > 0:
        isValid = False
        sys.stdout.write("ERROR:   BuildingParts and/or BuildingInstallations don't have a parent:\n")
        for each in pis:
            sys.stdout.write("\t#" + each + "\n")
    return isValid


def duplicate_vertices(j):
    isValid = True
    thev = set()
    duplicates = set()
    for v in j["vertices"]:
        s = str(v[0]) + " " + str(v[1]) + " " + str(v[2])
        if s in thev:
            duplicates.add(s)
        else:
            thev.add(s)
    if len(duplicates) > 0:
        sys.stdout.write('WARNING: there are ' + str(len(duplicates)) + ' duplicate vertices in j["vertices"]\n') 
    if len(duplicates) < 10:
        for v in duplicates:
            sys.stdout.write('\t(' + v + ')\n')
    return isValid


def orphan_vertices(j):
    def recusionvisit(a, ids):
      for each in a:
        if isinstance(each, list):
            recusionvisit(each, ids)
        else:
            ids.add(each)
    isValid = True
    ids = set()
    for co in j["CityObjects"]:
        for g in j['CityObjects'][co]['geometry']:
            recusionvisit(g["boundaries"], ids)
    noorphans = len(j["vertices"]) - len(ids)
    if noorphans > 0:
        sys.stdout.write('WARNING: there are ' + str(noorphans) + ' orphan vertices in j["vertices"]\n') 
        isValid
    if noorphans > 5:
        all = set()
        for i in range(len(j["vertices"])):
            all.add(i)
        symdiff = all.symmetric_difference(ids)
        sys.stdout.write('\t[')
        for each in symdiff:
            sys.stdout.write(str(each) + ', ')
        sys.stdout.write(']\n')
    return isValid


def metadata(j, js):
    isValid = True
    jtmp = js['properties']['metadata']['properties']
    if 'metadata' in j:
        for each in j['metadata']:
            if each not in jtmp:
                isValid = False
                sys.stdout.write("WARNING: Metadata '" + each + "' not in CityJSON schema.\n")
    return isValid

def cityjson_properties(j, js):
    isValid = True
    thewarnings = {}
    for property in j:
        if property not in js["properties"]:
            isValid = False
            sys.stdout.write("WARNING: root property '" + property + "' not in CityJSON schema, might be ignored by some parsers\n")
    return isValid

def citygml_attributes(j, js):
    isValid = True
    thewarnings = {}
    for id in j["CityObjects"]:
        cotype = j['CityObjects'][id]['type']
        tmp = js[str(cotype)]["properties"]["attributes"]["properties"]
        if 'attributes' in j['CityObjects'][id]:
            for a in j['CityObjects'][id]['attributes']:
                if a not in tmp:
                    isValid = False;
                    s = "WARNING: attributes '" + a + "' not in CityGML schema"
                    if s not in thewarnings:
                        thewarnings[s] = [id]
                    else:
                        thewarnings[s].append(id)
        if 'address' in j['CityObjects'][id]:
            tmp = js[str(cotype)]["properties"]["address"]["properties"]                        
            for a in j['CityObjects'][id]['address']:
                if a not in tmp:
                    isValid = False;
                    s = "WARNING: address attributes '" + a + "' not in CityGML schema"
                    if s not in thewarnings:
                        thewarnings[s] = [id]
                    else:
                        thewarnings[s].append(id)                        
    for each in thewarnings:
        sys.stdout.write(each)
        if len(thewarnings[each]) < 3:
            sys.stdout.write(" (")
            for coid in thewarnings[each]:
                sys.stdout.write(" #" + coid + " ")
            sys.stdout.write(")\n")
        else:
            sys.stdout.write(" (" + str(len(thewarnings[each])) + " CityObjects have this warning)\n")
    return isValid


def geometry_empty(j):
    isValid = True
    for id in j["CityObjects"]:
        if (j['CityObjects'][id]['type'] != 'CityObjectGroup') and (len(j['CityObjects'][id]['geometry']) == 0):
            isValid = False
            sys.stdout.write("WARNING: " + j['CityObjects'][id]['type'] + " #" + id + " has no geometry.\n")
    return isValid


def semantics(j):
    isValid = True
    for id in j["CityObjects"]:
        geomid = 0
        for g in j['CityObjects'][id]['geometry']:
            if 'semantics' not in g:
                continue
            else:
                sem = g['semantics']
                if g['type'] == 'Solid':
                    shellid = 0
                    for shell in g["boundaries"]:
                        surfaceid = 0
                        # print "surfaceid", surfaceid
                        # print "---", sem['values']
                        for surface in shell:
                            i = None
                            if sem['values'] is not None:
                                if sem['values'][shellid] is not None:
                                    i = sem['values'][shellid][surfaceid]
                            if i is not None:
                                if i > (len(sem['surfaces']) - 1):
                                    sys.stdout.write("ERROR:   semantics arrays problems ( #" + id)
                                    sys.stdout.write("; geom=" + str(geomid) + ",shell=" + str(shellid) + ",surface=" + str(surfaceid) + " )\n")
                                    isValid = False;
                                    break

                            surfaceid += 1
                        shellid += 1
                if g['type'] == 'MultiSurface' or g['type'] == 'CompositeSurface':
                    surfaceid = 0
                    for surface in g["boundaries"]:
                        i = None
                        # print "surfaceid", surfaceid
                        # print "---", sem['values']
                        if sem['values'] is not None:
                            if sem['values'][surfaceid] is not None:
                                i = sem['values'][surfaceid]
                        if i is not None:
                            if i > (len(sem['surfaces']) - 1):
                                sys.stdout.write("ERROR:   semantics arrays problems ( #" + id)
                                sys.stdout.write("; geom=" + str(geomid) + ",surface=" + str(surfaceid) + " )\n")
                                isValid = False;
                                break
                        surfaceid += 1
                # TODO if g['type'] == 'MultiSolid' or g['type'] == 'CompositeSolid':

            geomid += 1            
    return isValid


def main():
    #-- CLI stuff
    parser = argparse.ArgumentParser()
    parser.add_argument('inputfile', help='input CityJSON file')
    parser.add_argument('--skip_schema', help='skip schema (can be loooooonnng', action="store_true")
    args = parser.parse_args()

    fin = open(args.inputfile)
    print("Input file:", os.path.abspath(args.inputfile))

    isValid = True
    woWarnings = True
    
    #-- check if CityObjects have duplicate keys at reading time
    #-- otherwise it's too late: one object has been overwritten!
    try:
        j = json.loads(fin.read(), object_pairs_hook=dict_raise_on_duplicates)
    except ValueError, Argument:
        print ("ERROR:   ", Argument)
        isValid = False
        byebye(isValid, woWarnings)
        return

    #-- make sure it's a CityJSON file
    if (j["type"] != "CityJSON"):
        print ("ERROR:   not a CityJSON file")
        isValid = False
        byebye(isValid, woWarnings)
        return

    #-- fetch proper schema
    if j["version"] == "0.7":
        schema = '../../../schema/v07/cityjson.json'
    elif j["version"] == "0.6":
        schema = '../../../schema/v06/cityjson.json'
    elif j["version"] == "0.5":
        schema = '../../../schema/cityjson-v05.schema.json'
    else:
        sys.stdout.write("ERROR:   version not supported.\n")
        isValid = False
        byebye(isValid, woWarnings)
        return
    fins = open(schema)
    abs_path = os.path.abspath(os.path.dirname(schema))
    base_uri = 'file://{}/'.format(abs_path)
    print ("-->", base_uri)
    print ("Schema used:", os.path.abspath(schema))
    js = jsonref.loads(fins.read(), jsonschema=True, base_uri=base_uri)

    # json_str = json.dumps(js, indent=2)
    # f = open("/Users/hugo/temp/js.json", "w")
    # f.write(json_str)
    # return
    #-- load the schema for the cityobjects.json
    # sco_path = os.path.abspath(os.path.dirname(schema))
    # sco_path += '/cityobjects.json'
    # jsco = json.loads(open(sco_path).read())
    # print jsco
    print ("==========")    

    #-- validate the file against the schema
    if (args.skip_schema is False):
        try:
            jsonschema.validate(j, js)
        except jsonschema.ValidationError as e:
            print ("ERROR:   ", e.message)
            isValid = False
            byebye(isValid, woWarnings)
            return
        except jsonschema.SchemaError as e:
            print ("ERROR:   ", e)
            isValid = False
            byebye(isValid, woWarnings)
            return
    else:
        print ("WATCH OUT: validation against schema is skipped.")

    print ("Schema done.")

    if "extensions" in j:
        print ("===\nextension validation\n===")
        schema = '/Users/hugo/projects/cityjson/schema/v07/e_noise2.json'
        # schema = '/Users/hugo/temp/0000/e_noise2.json'
        fins = open(schema)
        abs_path = os.path.abspath(os.path.dirname(schema))
        base_uri = 'file://{}/'.format(abs_path)
        # base_uri = 'file:///Users/hugo/projects/cityjson/schema/v07/'

        
        jse = jsonref.loads(fins.read(), jsonschema=True, base_uri=base_uri)
        
        print (jse)
        
        # json_str = json.dumps(jse, indent=2)
        # f = open("/Users/hugo/temp/js.json", "w")
        # f.write(json_str)
        # return

        # jeval = {}
        # jeval["$schema"] = "http://json-schema.org/draft-04/schema#"
        # jeval["type"] = "object"
        # jeval["$ref"] = "file:///Users/hugo/projects/cityjson/schema/v07/e_noise.json#/+NoiseBuilding"
        # # jeval["$ref"] = "file:///Users/hugo/temp/0000/e_noise.json#/+NoiseBuilding"
        
        # try:
        #     print(jsonref.JsonRef.replace_refs(jeval, jsonschema=True, base_uri=base_uri))
        # except jsonref.JsonRefError, e:
        #     print ("FUK")
        # print ("doie")
        # return

        # print("-->", base_uri)
        # jeval2 = jsonref.loads(json.dumps(jeval), jsonschema=True, base_uri=base_uri)
        # json_str = json.dumps(jeval2, indent=2)
        # f = open("/Users/hugo/temp/js.json", "w")
        # f.write(json_str)
        # return

        for theid in j["CityObjects"]:
            if j["CityObjects"][theid]["type"][0] == '+':
                oneco = j["CityObjects"][theid]
                # print (oneco)
                try:
                    # print("here")
                    jsonschema.validate(oneco, jse)
                    # print("here2")
                except jsonschema.ValidationError as e:
                    print ("ERROR:   ", e.message)
                except jsonschema.SchemaError as e:
                    print ("ERROR:   ", e)

    byebye(isValid, woWarnings)
    return

    if city_object_groups(j) == False:
        isValid = False
    if building_parts(j) == False:
        isValid = False
    if building_installations(j) == False:
        isValid = False
    if building_pi_parent(j) == False:
        isValid = False
    if semantics(j) == False:
        isValid = False
    if metadata(j, js) == False:
        woWarnings = False
    if cityjson_properties(j, js) == False:
        woWarnings = False
    if citygml_attributes(j, jsco) == False:
        woWarnings = False
    if geometry_empty(j) == False:
        woWarnings = False
    if duplicate_vertices(j) == False:
        woWarnings = False
    if orphan_vertices(j) == False:
        woWarnings = False

    byebye(isValid, woWarnings)
    return


if __name__ == '__main__':
    main()


