#        _ _     ___     
#  ___  |_|_|___|  _|___ 
# |  _| | | |   |  _| . |
# |___|_| |_|_|_|_| |___|
#     |___|              
# 
# 
#   cjinfo.py
#   Created by Hugo Ledoux on 14/06/2017.
#   Copyright 2017 Hugo Ledoux. All rights reserved.
# 

import json

filename = '../../../example-datasets/Rotterdam/Delfshaven/3-20-DELFSHAVEN_solids.json'
fin = open(filename)
j = json.loads(fin.read())

print "Printing (some) information about:"
print "   ", filename

#-- CityJSON version
print "CityJSON version: ", j["version"] 

#-- CityObjects
print "===== CityObjects ====="
print "Total : ", len(j["CityObjects"])

d = set();
for id in j["CityObjects"]:
    d.add(j['CityObjects'][id]['type'])
print "Types:" 
for each in d:
    print "\t", each
d.clear()
for id in j["CityObjects"]:
    for geom in j['CityObjects'][id]['geometry']:
        d.add(geom["type"])
print "Geometries present:"
for each in d:
    print "\t", each


#-- metadata
print "===== Metadata =====" 
if "metadata" not in j:
    print "  none" 
else:
    for each in j["metadata"]:
        if each == 'crs':
            print "  crs: EPSG:", j["metadata"]["crs"]["epsg"]
        else:
            print " ", each

#--  vertices
print "===== Vertices ====="
print "Total:", len(j["vertices"])

#-- appearance
print "===== Appearance ====="
if 'appearance' not in j:
    print "  none"
else:
    if 'textures' in j['appearance']:
        print "  textures:", len(j['appearance']['textures'])
    if 'materials' in j['appearance']:
        print "  materials:", len(j['appearance']['materials'])

# # list all BuildingParts    
# for id in cm['CityObjects']:
#     if cm['CityObjects'][id]['type'] == 'BuildingPart':
#         print "BuildingPart", id

# # find all Solids
# for id in cm['CityObjects']:
#     for geom in cm['CityObjects'][id]['geometry']:
#         # print geom
#         if geom['type'] == "Solid":
#             print "Solid LoD:", geom['lod'], id

# for id in cm['CityObjects']:
#     geoms = cm['CityObjects'][id]['geometry']
#     for g in geoms:
#         # print g['texture']
#         for each in g['texture']:
#             # print each[0]
#             if not each[0]:
#                 print "empty"
#             else:
#                 print each[0]

#         break
#     break
