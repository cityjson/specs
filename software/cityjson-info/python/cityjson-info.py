#
#  _____ _ _          __ _____ _____ _____     _     ___     
# |     |_| |_ _ _ __|  |   __|     |   | |___|_|___|  _|___ 
# |   --| |  _| | |  |  |__   |  |  | | | |___| |   |  _| . |
# |_____|_|_| |_  |_____|_____|_____|_|___|   |_|_|_|_| |___|
#             |___|                                          
# 
#   cityjson-info.py
#   Created by Hugo Ledoux on 14/06/2017.
#   Copyright 2017 Hugo Ledoux. All rights reserved.
# 

import json

# fin = open('mycity.json')
fin = open('/Users/hugo/projects/cityjson-new/example-datasets/dummy-values/example2.json')
cm = json.loads(fin.read())

print "CRS:", cm['metadata']['crs']['epsg']

print "There are", len(cm['CityObjects']), "CityObjects"
# list all ids
for id in cm['CityObjects']:
    print "\t", id

print "-"*10

print cm['CityObjects']['2929']

# list all BuildingParts    
for id in cm['CityObjects']:
    if cm['CityObjects'][id]['type'] == 'BuildingPart':
        print "BuildingPart", id


for id in cm['CityObjects']:
    if cm['CityObjects'][id]['type'] == 'TINRelief':
        g = cm['CityObjects'][id]['geometry']
        if 'semantics' in g:
            print "yessss:", g['semantics']
        else:
            print "no semantics"

for id in cm['CityObjects']:
    if cm['CityObjects'][id]['type'] == 'WaterSurface':
        geoms = cm['CityObjects'][id]['geometry']
        for g in geoms:
            if ('semantics' in g) and (g['semantics'] != None):
                print "yessss:", g['semantics']
            else:
                print "no semantics"

# find all Solids
for id in cm['CityObjects']:
    for geom in cm['CityObjects'][id]['geometry']:
        # print geom
        if geom['type'] == "Solid":
            print "Solid LoD:", geom['lod'], id

# vertices
print "number of vertices:", len(cm['vertices'])

# appearance
if 'appearance' in cm:
    print "Appearance:"
    if 'materials' in cm['appearance']:
        print "\t", len(cm['appearance']['materials']), "materials"
    if 'textures' in cm['appearance']:
        print "\t", len(cm['appearance']['textures']), "textures"
    if 'vertex-texture' in cm['appearance']:
        print "\t", len(cm['appearance']['vertex-texture']), "vertex-texture"


for id in cm['CityObjects']:
    geoms = cm['CityObjects'][id]['geometry']
    for g in geoms:
        # print g['texture']
        for each in g['texture']:
            # print each[0]
            if not each[0]:
                print "empty"
            else:
                print each[0]

        break
    break
