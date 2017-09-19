
# val3dity - Copyright (c) 2011-2016, Hugo Ledoux.  All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of the authors nor the
#       names of its contributors may be used to endorse or promote products
#       derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL HUGO LEDOUX BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS


from geomtools import Point, Vector
import geomtools
import StringIO
import string
from lxml import etree



class Ring:
    def __init__(self, lsPts=None):
        self.cur = 0
        self.l = [] #-- list of Point
        if lsPts is not None:
            for p in lsPts:
                self.l.append(p)
    def append(self, p):
        assert(isinstance(p, Point))
        self.l.append(p)
    def __getitem__(self, index):
        return self.l[index]
    def __len__(self):
        return len(self.l)
    def set_cur_i(self, index):
        self.cur = index
    def set_cur_p(self, p):
        self.cur = self.index(p)
    def index(self, p):
        assert(isinstance(p, Point))
        try:
            return self.l.index(p)
        except ValueError, err:
            return -1
    def get_cur(self):
        return self[self.cur]
    def get_next(self):
        if self.cur == (len(self) - 1):
            return self[0]
        else:
            return self[self.cur+1]
    def validate_first_last_coords_are_same(self):
        if self.l[0] != self.l[-1]:
            return False
        return True
    def get_array(self, offset):
        ls = []
        for i in range(0, len(self.l) - 1):
            ls.append(self.l[i].id + offset)
        return ls

    def str_poly(self):
        if (self.validate_first_last_coords_are_same() == True):
            ls = []
            ls.append(str(len(self) - 1))
            for i in range(0, len(self.l) - 1):
                ls.append(str(self.l[i].id))
            ls.append("\n")
            return " ".join(ls)
        else:
            return "-1\n"
            



class Surface:
    def __init__(self, oring=None):
        self.oring = oring #-- outer Ring
        self.irings = [] #-- list of inner Ring(s)
        self.semantics = ""
    def get_gml_repr(self, ns):
        topnode = etree.Element('{%s}surfaceMember' % ns['gml'], nsmap=ns)
        polynode = etree.SubElement(topnode, '{%s}Polygon' % ns['gml'], nsmap=ns)
        temp = etree.SubElement(polynode, '{%s}exterior' % ns['gml'], nsmap=ns)
        temp.append(self.oring.get_gml_repr(ns))
        if self.get_number_inner_rings() > 0:
            temp = etree.SubElement(polynode, '{%s}interior' % ns['gml'], nsmap=ns)
            for each in self.irings:
                temp.append(each.get_gml_repr(ns))
        return topnode
    def set_outer_ring(self, ring):
        assert(isinstance(ring, Ring))
        self.oring = ring
    def set_semantics(self, sem):
        i = sem.find("}")
        if (i != -1):
            sem = sem[i+1:]
        # print sem
        self.semantics = sem
    def get_semantics(self):
        return self.semantics
    def append_inner_ring(self, ring):
        assert(isinstance(ring, Ring))
        self.irings.append(ring)
    def get_number_inner_rings(self):
        return len(self.irings)
    def get_array_surface(self, offset):
        l = []
        l.append(self.oring.get_array(offset))
        for i in self.irings:
            l.append(i.get_array(offset))
        return l
    def str_poly(self):
        ls = []
        ls.append(str(self.get_number_inner_rings() + 1) + " 0\n")
        # ls.append(str(self.get_number_inner_rings()))
        # ls.append("\n")
        ls.append(self.oring.str_poly())
        for i in self.irings:
            ls.append(i.str_poly())
            # pt = i.get_a_point_inside()
            # ls.append("1 " + str(pt.x) + " " + str(pt.y) + " " + str(pt.z))
            # ls.append("\n")
        return "".join(ls)
    def str_off(self):
        if (self.get_number_inner_rings() > 0):
            print "ERROR, cannot write inner rings to OFF file."
            return ""
        else:
            return self.oring.str_poly()

#####################################################################

class Shell:
    def __init__(self, gmlnode, dxlinks, ns):
        self.ns = ns
        self.dxlinks = dxlinks
        self.lsNodes = self.fetch_all_unique_points_in_shell(gmlnode)
        self.s = []
        self.parse_gml_shell(gmlnode)
    def append(self, s1):
        assert(isinstance(s1, Surface))
        self.s.append(s1)
    def __getitem__(self, index):
        return self.s[index]
    def __len__(self):
        return len(self.s)
    def fetch_all_unique_points_in_shell(self, gmlnode):
        lsNodes = []
        for s in gmlnode.findall(".//{%s}surfaceMember" % self.ns['gml']):
            if (self.dxlinks is not None) and ("{%s}href" % self.ns['xlink'] in s.attrib):
                link = s.attrib["{%s}href" % self.ns['xlink']]
                if link[0] == '#':
                    link = link[1:]
                p = self.dxlinks[link]
            else:
                p = s.find(".//{%s}Polygon" % self.ns['gml'])
            posList = p.findall(".//{%s}posList" % self.ns['gml'])
            if len(posList) > 0:
                for pos in posList:
                    coords = pos.text.split()
                    assert(len(coords) % 3 == 0)
                    for i in range(0, len(coords), 3):
                        temp = Point(coords[i], coords[i+1], coords[i+2])
                        if lsNodes.count(temp) == 0:
                            lsNodes.append(temp)
                            temp.id = len(lsNodes) - 1
            else: #-- a series of gml:pos
                lpos = p.findall(".//{%s}pos" % self.ns['gml'])
                for i in lpos:
                    coords = i.text.split()
                    temp = Point(coords[0], coords[1], coords[2])
                    if lsNodes.count(temp) == 0:
                        lsNodes.append(temp)
                        temp.id = len(lsNodes) - 1
        return lsNodes
    def parse_gml_shell(self, shellnode):
        for i in shellnode.findall(".//{%s}surfaceMember" % self.ns['gml']):
            self.s.append(self.parse_gml_surfaceMember(i))
    def parse_gml_surfaceMember(self, surfacenode):
        surf = Surface()
        if (self.dxlinks is not None) and ("{%s}href" % self.ns['xlink'] in surfacenode.attrib):
            link = surfacenode.attrib["{%s}href" % self.ns['xlink']]
            if link[0] == '#':
                link = link[1:]
            polynode = self.dxlinks[link]
            
            curp = polynode.getparent()
            while (True):
                if ( (curp.tag.find("GroundSurface") == -1) and
                     (curp.tag.find("WallSurface")   == -1) and
                     (curp.tag.find("RoofSurface")   == -1) ):
                    curp = curp.getparent()
                else:
                    break
            surf.set_semantics(curp.tag)
        else:
            polynode = surfacenode.find(".//{%s}Polygon" % self.ns['gml'])
        self.parse_gml_polygon(polynode, surf)
        return surf
    def parse_gml_polygon(self, poly, surf):
        extnode = poly.find(".//{%s}exterior" % self.ns['gml'])
        surf.set_outer_ring(self.parse_gml_polygon_ring(extnode))
        intnodes = poly.findall(".//{%s}interior" % self.ns['gml'])
        if len(intnodes) != 0:
            irings = []
            for i in range(len(intnodes)):
                surf.append_inner_ring(self.parse_gml_polygon_ring(intnodes[i]))
    def parse_gml_polygon_ring(self, ringnode):
        #-- 2 choices: (1) series of gml:pos; (2) one gml:posList
        posList = ringnode.find(".//{%s}posList" % self.ns['gml'])
        ring = Ring()
        if posList != None:
            coords = posList.text.split()
            assert(len(coords) % 3 == 0)
            for i in range(0, len(coords), 3):
                temp = Point(coords[i], coords[i+1], coords[i+2])
                assert(self.lsNodes.count(temp) > 0)
                j = self.lsNodes.index(temp)
                ring.append(self.lsNodes[j])
        else: #-- a series of gml:pos
            posList = ringnode.findall(".//{%s}pos" % self.ns['gml'])
            for i in posList:
                coords = i.text.split()
                temp = Point(coords[0], coords[1], coords[2])
                j = self.lsNodes.index(temp)
                ring.append(self.lsNodes[j])
        return ring
    def number_of_points(self):
        return len(self.lsNodes)
    def number_of_surfaces(self):
        return len(self.s)
    def get_array_points(self):
        l = []        
        for v in self.lsNodes:
            l.append(v.get_array())
        return l
    def get_array_surfaces(self, offset):
        a = []        
        for s in self.s:
            a.append(s.get_array_surface(offset))
        return a
    def str_poly(self):
        sOut = StringIO.StringIO()
        sOut.write(str(self.number_of_points()) + ' 3 0 0\n')
        for i in self.lsNodes:
            sOut.write(i.str_poly() + "\n")
        sOut.write(str(self.number_of_surfaces()) + " 0\n")
        for s in self.s:
            sOut.write(s.str_poly())
        sOut.write('0\n0\n')
        return sOut



def max_in_list_wrt_dim(ls, dim):
    re = []
    temp = ls[0]
    for i in ls:
        if geomtools.cmp_doubles(i[dim], temp[dim]) == 1:
                temp = i
    re.append(temp)
    #-- check if another with same value
    for i in ls:
        if i != temp and geomtools.cmp_doubles(i[dim], temp[dim]) == 0:
            re.append(i)
    return re




