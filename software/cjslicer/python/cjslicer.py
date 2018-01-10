import sys
import StringIO
import json
import copy

def main():
  # fIn = open(sys.argv[1])
  # fIn = '../../../example-datasets/Rotterdam/Delfshaven/3-20-DELFSHAVEN_solids.json'
  ids = ["102636712"]
  fIn = '../../../example-datasets/dummy-values/example.json'
  j = json.loads(open(fIn).read())

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

  print json.dumps(j2, indent=2)

  # json_str = json.dumps(cm)
  # json_str = json.dumps(cm, indent=2)
  # outname = sys.argv[1][:-3] + "json"
  # f = open(outname, "w")
  # f.write(json_str)
  # print "\nDone, output written to:", outname



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