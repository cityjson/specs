import sys
import StringIO
import json
import copy

def main():
  # fIn = open(sys.argv[1])
  # fIn = '../../../example-datasets/Rotterdam/Delfshaven/3-20-DELFSHAVEN_solids.json'
  fIn = '../../../example-datasets/dummy-values/example.json'
  ids = ["102636712", "2929"]
  j = json.loads(open(fIn).read())

  #-- sliced CityJSON object
  j2 = {}
  j2["type"] = j["type"]
  j2["version"] = j["version"]
  j2["CityObjects"] = {}
  
  #-- copy each CO to the output json
  for each in j["CityObjects"]:
    if each in ids:
      j2["CityObjects"][each] = j["CityObjects"][each]

  #-- update vertex indices
  oldnewids = {}
  newvertices = []    
  for each in j2["CityObjects"]:
    for geom in j2['CityObjects'][each]['geometry']:
      update_vertex_index(geom["boundaries"], oldnewids, j["vertices"], newvertices)
  j2["vertices"] = newvertices

  # TODO : how to deal with BuildingParts for instance? keep in the same file?

  #-- materials
  dOldNewIDs = {}
  newmats = []
  for each in j2["CityObjects"]:
    for geom in j2['CityObjects'][each]['geometry']:
      if "material" in geom:
        print "MATERIAL"
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
            update_array_indices(geom["material"][each]['values'], dOldNewIDs, j["appearance"]["materials"], newmats)
  if len(newmats) > 0:
    j2["appearance"] = {}
    j2["appearance"]["materials"] = newmats


  #-- save sliced CityJSON file
  json_str = json.dumps(j2, indent=2)
  f = open("/Users/hugo/temp/z.json", "w")
  f.write(json_str)
  # outname = sys.argv[1][:-3] + "json"
  # json_str = json.dumps(cm)
  # json_str = json.dumps(cm, indent=2)
  # print "\nDone, output written to:", outname


def update_array_indices(a, dOldNewIDs, oldarray, newarray):
  for i, each in enumerate(a):
    if isinstance(each, list):
      update_array_indices(each, dOldNewIDs, oldarray, newarray)
    elif each is not None:
      if each in dOldNewIDs:
        a[i] = dOldNewIDs[each]
      else:
        a[i] = len(newarray)
        dOldNewIDs[each] = len(newarray)
        newarray.append(oldarray[each])      


def update_vertex_index(a, oldnewids, oldvertices, newvertices):
  for i, each in enumerate(a):
    if isinstance(each, list):
      update_vertex_index(each, oldnewids, oldvertices, newvertices)
    else:
      if each in oldnewids:
        a[i] = oldnewids[each]
      else:
        a[i] = len(newvertices)
        oldnewids[each] = len(newvertices)
        newvertices.append(oldvertices[each])



if __name__ == "__main__":
    main()  