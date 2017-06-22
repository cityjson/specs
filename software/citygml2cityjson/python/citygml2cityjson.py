
from optparse import OptionParser
import sys
import string
import StringIO
import os
import shutil
from lxml import etree
import subprocess
from geomtools import Point, Vector
import geomtools
from gmltypes import Ring, Surface, Shell
import json

ns = {}
bVerbose = False

def main():
    options, args = parse_arguments()
    st = 1e-3
    if options.snap_tolerance is not None:
        st = options.snap_tolerance
    #-- the CityModel JSON object
    cm = {}
    cm["type"] = "CityModel"
    cm["version"] = "http://www.cityjson.org/version/0.1"
    cm["metadata"] = {}
    cm["metadata"]["crs"] = {'epsg': 1234}
    cm["metadata"]["bbox"] = [4424648.79, 5482614.69, 310.19, 4424852.33, 5482813.83, 330.28] 

    cm["CityObjects"] = {}
    cm["vertices"] = []
    process(args[0], cm, st)
    json_str = json.dumps(cm)
    # json_str = json.dumps(cm, indent=2)
    outname = args[0][:-3] + "json"
    f = open(outname, "w")
    f.write(json_str)
    print "\nDone, output written to:", outname


def process(fIn, cm, snap_tolerance = '1e-3'):
    try:
        t = float(snap_tolerance)
        if (t >= 0.0):
            print "Using tolerance for snapping of", snap_tolerance
            geomtools.TOLERANCE = float(snap_tolerance)
        else:
            print "snap tolerance can't be negative, using 1e-3."
    except:
        print "snap tolerance impossible, using 1e-3."

    parser = etree.XMLParser(ns_clean=True)
    tree = etree.parse(fIn, parser)
    root = tree.getroot()
    for key in root.nsmap.keys():
        if root.nsmap[key].find('www.opengis.net/gml') != -1:
            ns['gml'] = "%s" % root.nsmap[key]
        if root.nsmap[key].find('http://www.opengis.net/citygml/1.0') != -1:
            ns['cgml'] = "%s" % root.nsmap[key]
        if root.nsmap[key].find('http://www.opengis.net/citygml/2.0') != -1:
            ns['cgml'] = "%s" % root.nsmap[key]
        if root.nsmap[key].find('http://www.opengis.net/citygml/building/') != -1:
            ns['cgmlb'] = "%s" % root.nsmap[key]
        if root.nsmap[key].find('http://www.opengis.net/citygml/generics/') != -1:
            ns['gen'] = "%s" % root.nsmap[key]
        if root.nsmap[key].find('http://www.opengis.net/citygml/appearance/') != -1:
            ns['app'] = "%s" % root.nsmap[key]
        if root.nsmap[key].find('www.w3.org/1999/xlink') != -1:
            ns['xlink'] = "%s" % root.nsmap[key]    
        if root.nsmap[key].find('http://www.w3.org/2001/XMLSchema-instance') != -1:
            ns['xsi'] = "%s" % root.nsmap[key]

    #-- handling of (potential) xlinks
    dxlinks = {}
    if 'xlink' in ns:
        print "Oh no, the file might contain xlinks :/"
        print "Parsing whole file to find and store them."
        nodes = root.findall(".//{%s}surfaceMember[@{%s}href]" % (ns['gml'], ns['xlink']))
        if nodes is not None:
            print "There are %d xlinks for gml:surfaceMember" % len(nodes)
            # get all polygons with an id
            allp = {}
            for n in root.findall(".//{%s}Polygon[@{%s}id]" % (ns['gml'], ns['gml'])):
                allp[n.attrib["{%s}id" % ns['gml']]] = n
                
            for node in nodes:
                x = node.attrib["{%s}href" % ns['xlink']]
                if x[0] == '#':
                    x = x[1:]
                dxlinks[x] = allp[x]
        print "Resolved successfully", len(dxlinks), "xlinks."
    else:
        dxlinks = None

    # buildings = root.findall(".//{%s}Building" % ns['cgmlb'])
    # for building in buildings:
    #     lrs = building.findall(".//{%s}LinearRing" % ns['gml'])
    #     for lr in lrs:
    #         print "oring", lr.get("{%s}id" % ns['gml'])

    # return 1

    # #-- APPEARANCE
    # buildings = root.findall(".//{%s}Building" % ns['cgmlb'])
    # for building in buildings:
    #     print "Building", building.get("{%s}id" % ns['gml'])
    #     stexture = building.findall(".//{%s}surfaceDataMember" % ns['app'])
    #     for each in stexture:
    #         r = each.findall(".//{%s}imageURI" % ns['app'])
    #         print r[0].text
    #         # coords = each.findall(".//{%s}textureCoordinates" % ns['app'])
    #         # tl = coords[0].text.split()
    #         # print tl[:-2]
    #         # print "iring:clear", coords[0].get("ring")
    #         # coords = posList.text.split()



    buildings = root.findall(".//{%s}Building" % ns['cgmlb'])
    print "# Buildings:", len(buildings)
    for building in buildings:
        b = {'type': 'Building'}
        b["attributes"] = {}
        b["generic-attributes"] = {}
        for child in building:
            if child.tag.endswith("Attribute"):
                b['generic-attributes'][child.get("name")] = child[0].text
            elif child.tag == "{%s}function" % ns['cgmlb']:
                b['attributes']['function'] = child.text
            elif child.tag == "{%s}roofType" % ns['cgmlb']:
                b['attributes']['roofType'] = child.text
        b['geometry'] = []
        bsolids = building.findall(".//{%s}Solid" % ns['gml'])
        if len(bsolids) != 0:
            for each in bsolids:
                g = {'type': 'Solid'}
                g['lod'] = 2
                shells = [Shell(each.find("{%s}exterior" % ns['gml']), dxlinks, ns)]
                thepts = shells[0].get_array_points()
                thes = shells[0].get_array_surfaces(len(cm['vertices']))
                g['boundaries'] = [thes]
                thesem = []
                for s in thes:
                    thesem.append("Wall")
                g['semantics'] = [thesem]
                for v in thepts:
                    cm['vertices'].append(v)
                b['geometry'].append(g)
            cm["CityObjects"][building.get("{%s}id" % ns['gml'])] = b
            print "Building", building.get("{%s}id" % ns['gml'])
        else:
            for each in building.findall(".//{%s}MultiSurface" % ns['gml']):
                g = {'type': 'MultiSurface'}
                g['lod'] = 2
                shell = Shell(each, dxlinks, ns)
                thepts = shell.get_array_points()
                thes = shell.get_array_surfaces(len(cm['vertices']))
                g['boundaries'] = thes
                thesem = []
                for s in thes:
                    thesem.append("Wall")
                g['semantics'] = [thesem]
                for v in thepts:
                    cm['vertices'].append(v)
                b['geometry'].append(g)
            cm["CityObjects"][building.get("{%s}id" % ns['gml'])] = b
            print "Building", building.get("{%s}id" % ns['gml'])

    return 1


def clean_xml_tag(tag):
    """Desc:  Method to clean a xml tag, so that the {} in the ns are removed

       In:    tag: string of the namespace
       Out:   the cleaned tag

    """
    tag = tag.lstrip('{')
    tag = tag.rstrip('}')
    return tag


def parse_arguments():
    usage = "Usage: %prog [options] input\n(--help for a list of options)"
    parser = OptionParser(usage)
    parser.add_option("--snap_tolerance", dest="snap_tolerance",
                      help="tolerance for snapping of vertices (default: 1e-3 unit)")
    parser.add_option("-v", "--verbose",
                      action="store_true", dest="verbose")
    parser.add_option("-q", "--quiet",
                      action="store_false", dest="verbose", default=False)
    (options, args) = parser.parse_args()

    if len(args) != 1:
        parser.error("The input file (*.gml or *.xml) must be specified.")
    fIn = args[0]
    if fIn[-3:] != "gml" and fIn[-3:] != "xml":
        parser.error("The input file must be a GML/XML file.")
    return options, args


if __name__ == "__main__":
    main()

