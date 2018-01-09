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

  j2 = copy.deepcopy(j)
  
  lProperties = ["type", "version", "CityObjects", "vertices", "transform", "metadata", "appearance", "geometry-templates"]

  for each in j:
    if each not in lProperties:
      del j2[each]
  del j2["metadata"]
  del j2["vertices"]
  del j2["appearance"]
  del j2["geometry-templates"]

  for each in j["CityObjects"]:
    if each not in ids:
      del j2["CityObjects"][each]

  for each in j2["CityObjects"]:
    for geom in j2['CityObjects'][each]['geometry']:
      modify_vertex_index(geom["boundaries"])
      print "test:", geom




  # print json.dumps(j2, indent=2)



  # json_str = json.dumps(cm)
  # json_str = json.dumps(cm, indent=2)
  # outname = sys.argv[1][:-3] + "json"
  # f = open(outname, "w")
  # f.write(json_str)
  # print "\nDone, output written to:", outname



def modify_vertex_index(a):
  for i, each in enumerate(a):
    if isinstance(each, list):
      modify_vertex_index(each)
    else:
      # print each
      a[i] = a[i] + 100



if __name__ == "__main__":
    main()  