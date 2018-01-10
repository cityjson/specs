

import sys
import json
import argparse


def main():

  # parser = argparse.ArgumentParser()
  # parser.add_argument('inputfile', help='input CityJSON file')
  # parser.add_argument('ids', help='list IDs')
  # args = parser.parse_args()
  
  # fIn = open(sys.argv[1])
  # fIn = '../../../example-datasets/Rotterdam/Delfshaven/3-20-DELFSHAVEN_solids.json'
  fIn = '../../../example-datasets/dummy-values/example.json'
  ids = ["102636712", "2929"]
  j = json.loads(open(fIn).read())

  #-- new sliced CityJSON object
  j2 = {}
  j2["type"] = j["type"]
  j2["version"] = j["version"]
  j2["CityObjects"] = {}
  
  #-- copy CO to the j2
  for each in j["CityObjects"]:
    if each in ids:
      j2["CityObjects"][each] = j["CityObjects"][each]

  #-- update vertex indices
  oldnewids = {}
  newvertices = []    
  for each in j2["CityObjects"]:
    for geom in j2['CityObjects'][each]['geometry']:
      update_array_indices(geom["boundaries"], oldnewids, j["vertices"], newvertices, -1)
  j2["vertices"] = newvertices

  # TODO : how to deal with BuildingParts for instance? keep in the same file?

  if "appearance" in j:
    j2["appearance"] = {}
  #-- materials
  dOldNewIDs = {}
  newmats = []
  for each in j2["CityObjects"]:
    for geom in j2['CityObjects'][each]['geometry']:
      if "material" in geom:
        for each in geom["material"]:
          if 'value' in geom["material"][each]:
            v = geom["material"][each]["value"]
            if v in dOldNewIDs:
              geom["material"][each]["value"] = dOldNewIDs[v]
            else:
              geom["material"][each]["value"] = len(newmats)
              dOldNewIDs[v] = len(newmats)
              newmats.append(j["appearance"]["materials"][v])      
          if 'values' in geom["material"][each]:
            update_array_indices(geom["material"][each]['values'], dOldNewIDs, j["appearance"]["materials"], newmats, -1)
  if len(newmats) > 0:
    j2["appearance"]["materials"] = newmats

  #-- textures references (first int in the arrays)
  dOldNewIDs = {}
  newtextures = []
  for each in j2["CityObjects"]:
    for geom in j2['CityObjects'][each]['geometry']:
      if "texture" in geom:
        for each in geom["texture"]:
          if 'values' in geom["texture"][each]:
            update_array_indices(geom["texture"][each]['values'], dOldNewIDs, j["appearance"]["textures"], newtextures, 0)
  if len(newtextures) > 0:
    j2["appearance"]["textures"] = newtextures
  #-- textures vertices references (1+ int in the arrays)
  dOldNewIDs = {}
  newtextures = []
  for each in j2["CityObjects"]:
    for geom in j2['CityObjects'][each]['geometry']:
      if "texture" in geom:
        for each in geom["texture"]:
          if 'values' in geom["texture"][each]:
            update_array_indices(geom["texture"][each]['values'], dOldNewIDs, j["appearance"]["vertices-texture"], newtextures, 1)
  if len(newtextures) > 0:
    j2["appearance"]["vertices-texture"] = newtextures

  #-- metadata
  j2["metadata"] = j["metadata"]
  j2["metadata"]["bbox"] = update_bbox(j["vertices"])


  #-- save sliced CityJSON file
  json_str = json.dumps(j2, indent=2)
  f = open("/Users/hugo/temp/z.json", "w")
  f.write(json_str)
  # outname = sys.argv[1][:-3] + "json"
  # json_str = json.dumps(cm)
  # json_str = json.dumps(cm, indent=2)
  # print "\nDone, output written to:", outname


def update_bbox(vertices):
  bbox = [9e9, 9e9, 9e9, -9e9, -9e9, -9e9]
  for v in vertices:
    for i in range(3):
      if v[i] < bbox[i]:
        bbox[i] = v[i]
    for i in range(3):
      if v[i] > bbox[i+3]:
        bbox[i+3] = v[i]
  return bbox


def update_array_indices(a, dOldNewIDs, oldarray, newarray, slicearray):
  #-- slicearray: -1=none ; 0=use-only-first (for textures) ; 1=use-1+ (for textures)
  #-- a must be an array
  #-- issue with passing integer is that it's non-mutable, thus can't update
  #-- (or I don't know how...)
  for i, each in enumerate(a):
    if isinstance(each, list):
      update_array_indices(each, dOldNewIDs, oldarray, newarray, slicearray)
    elif each is not None:
      if ( (slicearray == -1) or (slicearray == 0 and i == 0) or (slicearray == 1 and i > 0) ):
        if each in dOldNewIDs:
          a[i] = dOldNewIDs[each]
        else:
          a[i] = len(newarray)
          dOldNewIDs[each] = len(newarray)
          newarray.append(oldarray[each])      


if __name__ == "__main__":
    main()  