
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


import math
import random

TOLERANCE = 1e-3

class Point:
    def __init__(self, cx=0.0, cy=0, cz=0, cid=0):
        self.x = float(cx)
        self.y = float(cy)
        self.z = float(cz)
        self.id = int(cid)
    def __repr__(self):
        return str(self.x) + "," + str(self.y) + "," + str(self.z)
    def str_poly(self):
        return str(self.id) + " " + "{:f}".format(self.x) + " " + "{:f}".format(self.y) + " " + "{:f}".format(self.z)
    def str_off(self):
        return "{:f}".format(self.x) + " " + "{:f}".format(self.y) + " " + "{:f}".format(self.z)
    def get_array(self):
        return [self.x, self.y, self.z]
    def __str__(self):
        return "{:f}".format(self.x) + "," + "{:f}".format(self.y) + "," + "{:f}".format(self.z)
    def __getitem__(self, index):
        if index < 0 or index > 2:
            raise Exception("out of bound for Point access.")
        if index == 0:
            return self.x
        if index == 1:
            return self.y
        if index == 2:
            return self.z
    def __eq__(self, other):
        if (cmp_doubles(self.x, other.x) == 0 and 
            cmp_doubles(self.y, other.y) == 0 and
            cmp_doubles(self.z, other.z) == 0 ):
            return True
        else:
            return False
    def __ne__(self, other):
        return not self.__eq__(other)
    def __neg__(self):
        self.x = -(self.x)
        self.y = -(self.y)
        self.z = -(self.z)
        return self
    def cmp_one_dim(self, other, dim):
        if dim == 'x':
            return cmp_doubles(self.x, other.x)
        if dim == 'y':
            return cmp_doubles(self.y, other.y)
        if dim == 'z':
            return cmp_doubles(self.z, other.z)
    def distance_to(self, other):
        return math.sqrt( pow(other.x - self.x, 2) +
                          pow(other.y - self.y, 2) + 
                          pow(other.z - self.z, 2) )  
    def distance_to_proj(self, other, m, n):
        return math.sqrt( pow(other[m] - self[m], 2) +
                          pow(other[n] - self[n], 2) )
    def translate_random(self, dist):
        a = random.uniform(-dist, dist)
        self.x += a
        self.y += a
        self.z += a
    def copy(self):
        return Point(self.x, self.y, self.z)

    
class Vector(Point):
    def __str__(self):
        return "VECTOR(" + str(self.x) + " " + str(self.y) + " " + str(self.z) + ")"
    def __add__(self, o):
        r = Vector()
        r.x = self.x + o.x
        r.y = self.y + o.y
        r.z = self.z + o.z
        return r
    def set_vector(self, a, b):
        self.x = b.x - a.x
        self.y = b.y - a.y
        self.z = b.z - a.z        
    def length(self):
        return math.sqrt((self.x * self.x) +
                         (self.y * self.y) +
                         (self.z * self.z))
    def cross_product(self, o):
        r = Vector()
        r.x = (self.y * o.z) - (self.z * o.y)
        r.y = -( (self.x * o.z) - (self.z * o.x) )
        r.z = (self.x * o.y) - (self.y * o.x)
        return r
    def dot_product(self, o):
        return (self.x * o.x) + (self.y * o.y) + (self.z * o.z) 
    def normalise(self):
        length = self.length()
        #-- raise an  if the lenght of the vector is 0 (or near 0)
        if abs(length) < TOLERANCE:
            1.0 / 0 #-- will raise a ZeroDivisionError :)
        else:
            self.x = self.x / length
            self.y = self.y / length
            self.z = self.z / length
        return self
            
            
def get_projection_plane(p1, p2, p3):
    normal = geomtools.get_normal_rhr(p1, p2, p3)
    #-- check if the plane if vertical, and assign the projection plane
    m = 0 #-- xy plane
    n = 1
    if geomtools.cmp_doubles(normal.z, 0.0) == 0:
        if  geomtools.cmp_doubles(normal.y, 0.0) == 00:
            m = 1 #-- yz plane
            n = 2
        else:
            m = 0 #-- xz plane
            n = 2 
    return m, n
            

def cmp_doubles(a, b):
    if abs(a-b) <= TOLERANCE:
        return 0
    else:
        if a - b > 0:
            return 1
        else:
            return -1
    
    
def orient2D(a, b, p):
    """ 
    Determine if a Point_25 pt is above or below the plane defined  by a-b-c (anti-clockwise order)
    
    Input:      a,b,c : the Point_25 in anti-clockwise order
                p : the point to test

    Output:     1  -> pt is BELOW of the plane (OK for left-hand rule)
                0  -> 4 points are coplanar
               -1  -> pt is ABOVE of the plane (NOT OK for left-hand rule)

    Note:       "above and below" means when looking from above;
                or when using the left-hand rule
    """
    re = det3x3t(a, b, p)
    if abs(re) < TOLERANCE:
        return 0
    elif re > 0:
        return 1
    else:
        return -1

 
def orient2D_proj(a, b, p, m, n):
    """ 
    Determine if a Point_25 pt is above or below the plane defined  by a-b-c (anti-clockwise order)
    
    Input:      a,b,c : the Point_25 in anti-clockwise order
                p : the point to test

    Output:     1  -> pt is BELOW of the plane (OK for left-hand rule)
                0  -> 4 points are coplanar
               -1  -> pt is ABOVE of the plane (NOT OK for left-hand rule)

    Note:       "above and below" means when looking from above;
                or when using the left-hand rule
    """
    re = det3x3t_expand(a[m], a[n], b[m], b[n], p[m], p[n])
    if abs(re) < TOLERANCE:
        return 0
    elif re > 0:
        return 1
    else:
        return -1 
    
  
def det3x3t(a, b, c):
    at = Point(a.x - c.x, a.y - c.y)
    bt = Point(b.x - c.x, b.y - c.y)
    return (at.x * bt.y) - (at.y * bt.x)


def det3x3t_expand(ax, ay, bx, by, cx, cy):
    at = Point(ax - cx, ay - cy)
    bt = Point(bx - cx, by - cy)
    return (at.x * bt.y) - (at.y * bt.x)


def det3x3_point(a, b, c):
    return det3x3(a.x, a.y, a.z, 
                  b.x, b.y, b.z, 
                  c.x, c.y, c.z)

def det3x3(ax, ay, az, bx, by, bz, cx, cy, cz):
    temp1 = ax * (by * cz - bz * cy)
    temp2 = ay * (bx * cz - bz * cx)
    temp3 = az * (bx * cy - by * cx)
    return temp1 - temp2 + temp3


def det4x4t(a, b, c, d):
    return det3x3(a.x-d.x, a.y-d.y, a.z-d.z, 
                  b.x-d.x, b.y-d.y, b.z-d.z,
                  c.x-d.x, c.y-d.y, c.z-d.z) 

 
def orient3D(a, b, c, p):
    """ 
    Determine if a Point p is above or below the plane defined by the Points
    abc (in anti-clockwise order looking from above)
    
    Input:      a,b,c : the Points in anti-clockwise order looking from above
                p : the Point to test

    Output:     1  -> pt is BELOW of the plane (OK for left-hand rule)
                0  -> 4 points are coplanar
               -1  -> pt is ABOVE of the plane (NOT OK for left-hand rule)

    Note:       "above and below" means when looking from above;
                or when using the left-hand rule
    """
    re = det4x4t(a, b, c, p)
    if abs(re) < TOLERANCE:
        return 0
    elif re > 0:
        return 1
    else:
        return -1


#def get_area_triangle(a, b, c):
#    """Area of triangle (projected on the 2D plane.)
#    
#    Input:   a,b,c: the Points
#    Ouput:   value of area
#    
#    """
#    return abs(det3x3t(a, b, c) / 2)


def get_volume_tetra(a, b, c, d):
    """Volume of a tetrahedron."""
    return abs(det4x4t(a, b, c, d) / 6)


def get_normal_rhr(a, b, c):
    """ 
    Return the normal Vector to the 3 Points a-b-c, acc. to a right-hand rule.
    If a-b-c are CCW viewed from above, then the Vector points above
    
    Input:      a,b,c : the Points in anti-clockwise order looking from above

    Output:     the Vector
    
    """
    v1 = Vector(b.x - a.x, b.y - a.y, b.z - a.z)
    v2 = Vector(c.x - a.x, c.y - a.y, c.z - a.z)
    return v1.cross_product(v2)


def point_in_triangle_3D(a, b, c, p, m, n):
    re = True
    oTr = orient2D_proj(a, b, c, m, n)
    if ((orient2D_proj(a, b, p, m, n) != oTr) or
        (orient2D_proj(b, c, p, m, n) != oTr) or
        (orient2D_proj(c, a, p, m, n) != oTr)):    
        re = False
    return re
    

def get_midpoint_of_segment(a, b):
    mid = Point()
    mid.x = (a.x + b.x) / 2.0
    mid.y = (a.y + b.y) / 2.0
    mid.z = (a.z + b.z) / 2.0
    return mid


def point_in_tetra(a, b, c, d, p):
    otetra = orient3D(a, b, c, d)
    assert(otetra != 0)
    if ( (orient3D(a, b, c, p) == otetra) and
         (orient3D(b, d, c, p) == otetra) and
         (orient3D(c, d, a, p) == otetra) and
         (orient3D(d, b, a, p) == otetra) ):
        return True
    else:
        return False
    
    
    oTr = orient2D_proj(a, b, c, m, n)
    if ((orient2D_proj(a, b, p, m, n) != oTr) or
        (orient2D_proj(b, c, p, m, n) != oTr) or
        (orient2D_proj(c, a, p, m, n) != oTr)):    
        re = False
    return re

def intersection_plane_segment(ring, a, b):
    pass            
        
  
def get_circumsphere(a, b, c, d):
    pass
    
    
        
    
    

if __name__ == "__main__":
    p1 = Point(3, 0, 1)
    print p1[0], p1.x
    print p1[-1]
    
#    p1 = Point(1.0, 2.00000000001, 3)
#    p2 = Point(1, 2, 3)
#    print p1 == p2
