import sys
import StringIO
import json

def main():
  fIn = open(sys.argv[1])
  cm = convert(fIn)
  # json_str = json.dumps(cm)
  json_str = json.dumps(cm, indent=2)
  outname = sys.argv[1][:-3] + "json"
  f = open(outname, "w")
  f.write(json_str)
  print "\nDone, output written to:", outname

def convert(fIn):
  #-- skip the header
  l = fIn.next()
  while (len(l) == 0) or (l[0] == '#') or (l[:3] == 'OFF'):
    l = fIn.next()
  numVertices = int(l.split()[0])
  numFaces    = int(l.split()[1])
  lstVertices = []
  for i in range(numVertices):
    lstVertices.append(map(float, fIn.next().split()))
  lstFaces = []
  for i in range(numFaces):
    lstFaces.append(map(int, fIn.next().split()[1:]))
  cm = {}
  cm["type"] = "CityModel"
  cm["version"] = "http://www.cityjson.org/version/0.2"
  cm["CityObjects"] = {}
  cm["vertices"] = []
  for v in lstVertices:
    cm["vertices"].append(v)
  g = {'type': 'Solid'}
  shell = []
  for f in lstFaces:
    shell.append([f])
  g['boundaries'] = [shell]
  g['lod'] = 1
  o = {'type': 'GenericCityObject'}
  o['geometry'] = [g]
  cm["CityObjects"]["id-1"] = o
  return cm


if __name__ == "__main__":
    main()  