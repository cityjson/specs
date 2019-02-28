
#-- a simple "extruder" to obtain CityJSON LoD1 Buildings from footprints
#-- Hugo Ledoux <h.ledoux@tudelft.nl>
#-- 2019-02-28

import fiona
import shapely.geometry as sg
import json
import copy


def main():
    #-- read the input footprints
    c = fiona.open('somebuildings.gpkg')
    print("# of features: ", len(c))
    lsgeom = [] #-- list of the geometries
    lsattributes = [] #-- list of the attributes
    for each in c:
        lsgeom.append(sg.shape(each['geometry'])) #-- geom are casted to Fiona's 
        lsattributes.append(each['properties'])
    #-- extrude to CityJSON
    cm = output_citysjon(lsgeom, lsattributes)
    #-- save the file to disk 'mycitymodel.json'
    json_str = json.dumps(cm, indent=2)
    fout = open("mycitymodel.json", "w")
    fout.write(json_str)
    print("done.")


def output_citysjon(lsgeom, lsattributes):
    #-- create the JSON data structure for the City Model
    cm = {}
    cm["type"] = "CityJSON"
    cm["version"] = "0.9"
    cm["CityObjects"] = {}
    cm["vertices"] = []

    for (i,geom) in enumerate(lsgeom):
        footprint = geom[0]
        #-- one building
        oneb = {}
        oneb['type'] = 'Building'
        oneb['attributes'] = {}
        oneb['attributes']['local-id'] = lsattributes[i]['lokaalid']
        oneb['attributes']['bgt_status'] = lsattributes[i]['bgt_status']
        oneb['geometry'] = [] #-- a cityobject can have >1
        #-- the geometry
        g = {} 
        g['type'] = 'Solid'
        g['lod'] = 1
        allsurfaces = [] #-- list of surfaces forming the oshell of the solid
        #-- exterior ring of each footprint
        oring = list(footprint.exterior.coords)
        oring.pop() #-- remove last point since first==last
        if footprint.exterior.is_ccw == False:
            #-- to get proper orientation of the normals
            oring.reverse() 
        extrude_walls(oring, lsattributes[i]['height'], allsurfaces, cm)
        #-- interior rings of each footprint
        irings = []
        interiors = list(footprint.interiors)
        for each in interiors:
            iring = list(each.coords)
            iring.pop() #-- remove last point since first==last
            if each.is_ccw == True:
                #-- to get proper orientation of the normals
                iring.reverse() 
            irings.append(iring)
            extrude_walls(iring, lsattributes[i]['height'], allsurfaces, cm)
        #-- top-bottom surfaces
        extrude_roof_ground(oring, irings, lsattributes[i]['height'], False, allsurfaces, cm)
        extrude_roof_ground(oring, irings, 0, True, allsurfaces, cm)
        #-- add the extruded geometry to the geometry
        g['boundaries'] = []
        g['boundaries'].append(allsurfaces)
        #-- add the geom to the building 
        oneb['geometry'].append(g)
        #-- insert the building as one new city object
        cm['CityObjects'][lsattributes[i]['gml_id']] = oneb
    return cm


def extrude_roof_ground(orng, irngs, height, reverse, allsurfaces, cm):
    oring = copy.deepcopy(orng)
    irings = copy.deepcopy(irngs)
    if reverse == True:
        oring.reverse()
        for each in irings:
            each.reverse()
    for (i, pt) in enumerate(oring):
        cm['vertices'].append([pt[0], pt[1], height])
        oring[i] = (len(cm['vertices']) - 1)
    for (i, iring) in enumerate(irings):
        for (j, pt) in enumerate(iring):
            cm['vertices'].append([pt[0], pt[1], height])
            irings[i][j] = (len(cm['vertices']) - 1)
    # print(oring)
    output = []
    output.append(oring)
    for each in irings:
        output.append(each)
    allsurfaces.append(output)


def extrude_walls(ring, height, allsurfaces, cm):
    #-- each edge become a wall, ie a rectangle
    for (j, v) in enumerate(ring[:-1]):
        l = []
        cm['vertices'].append([ring[j][0],   ring[j][1],   0])
        cm['vertices'].append([ring[j+1][0], ring[j+1][1], 0])
        cm['vertices'].append([ring[j+1][0], ring[j+1][1], height])
        cm['vertices'].append([ring[j][0],   ring[j][1],   height])
        t = len(cm['vertices'])
        allsurfaces.append([[t-4, t-3, t-2, t-1]])    
    #-- last-first edge
    l = []
    cm['vertices'].append([ring[-1][0], ring[-1][1], 0])
    cm['vertices'].append([ring[0][0],  ring[0][1],  0])
    cm['vertices'].append([ring[0][0],  ring[0][1],  height])
    cm['vertices'].append([ring[-1][0], ring[-1][1], height])
    t = len(cm['vertices'])
    allsurfaces.append([[t-4, t-3, t-2, t-1]])    
    

if __name__ == '__main__':
    main()