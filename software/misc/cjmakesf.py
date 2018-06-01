
#-- script to make a CityJSON be "Simple Features", ie no "vertices" array
#-- and the coordinates of each geom are replaced by [x,y,z] 

import os
import sys
import json
import argparse

def main():

  # filename = '/Users/hugo/projects/cityjson/example-datasets/dummy-values/example.json'    
  # filename = '/Users/hugo/Dropbox/data/cityjson/claus/Railway/v0.6/railway_cityjson_0.6/LoD3_railway_uncompressed.json'
  # filename = '/Users/hugo/Dropbox/data/cityjson/size-index-vs-sf/s5.json'
  filename = '/Users/hugo/Dropbox/data/cityjson/size-index-vs-sf/VM05_2009.json'
  j = json.loads(open(filename).read())

  for each in j["CityObjects"]:
    for geom in j['CityObjects'][each]['geometry']:
      replace(geom["boundaries"], j["vertices"])
  del j["vertices"]

  #-- save sliced CityJSON file
  json_str = json.dumps(j, separators=(',',':'))
  s = os.path.abspath(filename)
  p = s.rfind(".json")
  outputfile = s[:p] + ".sf" + ".json"
  f = open(outputfile, "w")
  f.write(json_str)
  print ("\nDone, output written to:", outputfile)



def replace(a, lsv):
  for i, each in enumerate(a):
    if isinstance(each, list):
      replace(each, lsv)
    elif each is not None:
      a[i] = lsv[each]

if __name__ == "__main__":
    main()  