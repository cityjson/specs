#citygml to cityjson 

import json
import pprint
import numpy as np
import xml.etree.ElementTree as ET
from itertools import islice
import io

try:
    to_unicode = unicode
except NameError:
    to_unicode = str
    
ns="http://www.opengis.net/citygml/2.0"
ns_gml  = "http://www.opengis.net/gml"
ns_dem="http://www.opengis.net/citygml/relief/2.0"
ns_bldg = "http://www.opengis.net/citygml/building/2.0"
ns_xAL="urn:oasis:names:tc:ciq:xsdschema:xAL:2.0"
ns_xsi="http://www.w3.org/2001/XMLSchema-instance"
ns_xlink="http://www.w3.org/1999/xlink"
ns_smil20lang="http://www.w3.org/2001/SMIL20/Language"
ns_sch="http://www.ascc.net/xml/schematron"


nsmap = {None : ns,
        'gml' : ns_gml,
        'dem':ns_dem,
        'bldg': ns_bldg,
        'xAL':ns_xAL,
        'xsi':ns_xsi,
        'xlink':ns_xlink,
        'smil20lang':ns_smil20lang,
        'sch':ns_sch        
        }

def remove_reccuring(list_vertices):
    list_vertices_without_last = list_vertices[:-1]
    found = set()
    for item in list_vertices_without_last:
        if str(item) not in found:
            yield item
            found.add(str(item))
            
def group(lst, n):
     return zip(*[lst[i::n] for i in range(n)]) 
  
def process_lod1Solid(lod1solid):
    print "---- processing lod1Solid ----"
    polys = []
    for child in lod1solid.getchildren():
        if child.tag == ('{%s}Solid' %ns_gml):
            if child.tag.find('.//{%s}exterior' %ns_gml):
               print "Solid has exterior"
            cs = child.find('.//{%s}CompositeSurface' %ns_gml)
            polys = cs.findall('.//{%s}Polygon' %ns_gml)
            print "Solid has ",len(polys)," faces"
        
        # for holes in solids
        if (child.find('.//{%s}interior' %ns_gml)):
            print "Solid has interior"
        else:
            print "Solid has no interiors"
       
    
                
def citygml2json(tinReliefs,buildings):
    
    cm = {}
    cm["type"] = "CityJSON"
    cm["version"] = "0.5"
    cm["CityObjects"] = {}
    
    listPoints=[]
    kindex=0
    newl=[]
    count = 0
    listIndices=[]
    seclist = []
    j = 0
    for tinRelief in tinReliefs:
            
        ts = tinRelief.find('.//{%s}TriangulatedSurface' %ns_gml)
        triangles = []
        triangles = ts.findall('.//{%s}Triangle' %ns_gml)
        seclist.append(len(triangles))
        for t in triangles:
            posL=t.find('.//{%s}posList' %ns_gml)
            coords=posL.text.split()
            newcoords=coords[:-3]  
            assert(len(coords) % 3 == 0)
            for i in range(0, len(newcoords), 3):
                listPoints.append([float(newcoords[i]), float(newcoords[i+1]), float(newcoords[i+2])])
                listIndices.append(kindex)
                kindex=kindex+1            
            
    for p in listPoints:
        if p not in newl:
            newl.append(p)
            listIndices[j]= newl.index(p)
        else:
            listIndices[j]= newl.index(p)
            #print listIndices[j], newl.index(p)
        j=j+1                        
        
    ipoints_clean = list(remove_reccuring(listPoints))
    triIndices = group(listIndices,3)
    it = iter(triIndices)
    sliced =[list(islice(it, 0, i)) for i in seclist]
        
    
    for tR in tinReliefs:
        print "\n---------Processing #", count+1, "tinRelief------------ "        
        g = {'type': 'CompositeSurface'}
        for child in tR.getchildren():
            if child.tag == '{%s}lod' %ns_dem:
                g['lod'] = int(child.text)
                print "TINRelief LOD: ", child.text
        
        triangles = ts.findall('.//{%s}Triangle' %ns_gml)
        print "# of triangles : ", len(triangles)
        
        
        shell = []
        for f in sliced[count]:
            shell.append([f])
            #print [list(f)]
        
        g['boundaries'] = shell
        o = {'type': 'TINRelief'}
        o['geometry'] = [g]
        cm["CityObjects"]["MyTerrain_"+str(count)] = o
        count = count + 1    
    
    cm["vertices"] = ipoints_clean
    
    # --- building --- #
    
    bcount = 0
    for building in buildings:
        print "\n---------Processing #", bcount+1, "building------------ "    
        for child in building.getchildren():
            
            if child.tag == '{%s}lod1Solid' %ns_bldg:
                print "Building is solid with LOD : 1 "
                process_lod1Solid(child)                               
                print "---- lod1Solid processed ----"
            
            if child.tag == '{%s}lod2Solid' %ns_bldg:
                print "Building is solid with LOD : 2 "
            if child.tag == '{%s}lod3Solid' %ns_bldg:
                print "Building is solid with LOD : 3 "
            if child.tag == '{%s}lod4Solid' %ns_bldg:
                print "Building is solid with LOD : 4 "
        bcount = bcount + 1
    
    return cm     

#-------------start of program-------------------#

#CityGML Relief source
citygml_src ="sample/TINterrain.gml"
tree = ET.parse(citygml_src)

#cityObjects in CityGML
cityObjects = []
root = tree.getroot()

for obj in root.getiterator('{%s}cityObjectMember'% ns):
    cityObjects.append(obj)
print "# of cityObject(s):", len(cityObjects)

#ReliefFeature in CityGML
reliefFeatures = []
for cityObject in cityObjects:
    for child in cityObject.getchildren():
        if child.tag == '{%s}ReliefFeature' %ns_dem:
            reliefFeatures.append(child)
print "# of ReliefFeature(s): ", len(reliefFeatures)

#reliefComponent in CityGML
reliefComponents = []
for reliefFeature in reliefFeatures:
    for child in reliefFeature.getchildren():
        if child.tag == '{%s}name' %ns_gml:
            print "ReliefFeature name: ", child.text
        if child.tag == '{%s}lod' %ns_dem:
            print "ReliefFeature LOD: ", child.text
        if child.tag == '{%s}reliefComponent' %ns_dem:
            reliefComponents.append(child)
print "# of reliefComponent(s): ", len(reliefComponents)


#TINRelief in CityGML
tinReliefs = []
for reliefComponent in reliefComponents:
    for child in reliefComponent.getchildren():
        if child.tag == '{%s}TINRelief' %ns_dem:
            tinReliefs.append(child)           
print "# of TINRelief(s): ", len(tinReliefs)
 
#-----------------------------------#

#Buildings in CityGML
buildings = []
for cityObject in cityObjects:
    for child in cityObject.getchildren():
        if child.tag == '{%s}Building' %ns_bldg:
            buildings.append(child)
print "\n# of Buildings(s): ", len(buildings)

for building in buildings:
    for child in building.getchildren():
        if child.tag == '{%s}name' %ns_gml:
            print "Building name: ", child.text
        if child.tag == '{%s}function' %ns_bldg:
            print "Building function code: ", child.text
        if child.tag == '{%s}yearOfConstruction' %ns_bldg:
            print "Building year of construction: ", child.text
        if child.tag == '{%s}roofType' %ns_bldg:
            print "Building roof type code: ", child.text
        if child.tag == '{%s}measuredHeight' %ns_bldg:
            print "Building measured height: ", child.text
        if child.tag == '{%s}storeysAboveGround' %ns_bldg:
            print "Building storeys above ground : ", child.text
        if child.tag == '{%s}storeyHeightsAboveGround' %ns_bldg:
            print "Building storey heights above ground : ", child.text #relatiefhoogte
        


#-----------------------------------#       
#JSON Output
cityjson_dump ="sampledata/output.json"
count =1

cm = citygml2json(tinReliefs)
json_str = json.dumps(cm, indent = None,sort_keys=True)
print "\n#-------------CityJSON output-------------------#\n"
#print json_str

f = open(cityjson_dump, "w")
f.write(json_str.replace("\n\n",""))
print "\nDone, output written to:", cityjson_dump
