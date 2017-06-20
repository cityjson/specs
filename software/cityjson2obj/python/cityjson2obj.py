
#-- TODO:   support irings! Now they are ignored, and the oring covers them
#--         this means CDT of the faces, which means more machinery...

import json
import sys


fin = open(sys.argv[1])
cm = json.loads(fin.read())
fout = open('out.obj', 'w')
foutm = open('out.mtl', 'w')
themtls = []


def process_list_surfaces(ls, lt, lm, cm, curtexture, curmtl, fout, foutm):
    for i, surface in enumerate(ls):
        lv = []
        lvt = []
        for j in surface[0]: #-- only
            lv.append(j + 1)
        if lm is not None:
            if lm[i] != curmtl:
                curmtl = lm[i]
                fout.write('usemtl ' + cm['appearance']['materials'][lm[i]]['name'] + '\n') 
        if lt is not None:
            if len(lt[i][0]) > 0:
                if lt[i][0][0] != curtexture: #-- write to OBJ only new material
                    curtexture = lt[i][0][0]
                    fout.write('usemtl ' + themtls[lt[i][0][0]] + '\n')
                for j in lt[i][0][1:]: #-- save the vt
                    lvt.append(j + 1)
        o = []
        o.append('f')
        if len(lvt) == 0: #-- no texture
            for v in lv:
                o.append(str(v))
        else:
            for i in range(len(lv)):
                o.append(str(lv[i]) + "/" + str(lvt[i]))
        fout.write(" ".join(o))
        fout.write('\n')



def write_obj():
    #-- appearance == MTL file
    if 'appearance' in cm:
        fout.write('mtllib ./out.mtl\n')
    #-- vertices
    for v in cm['vertices']:
        fout.write('v ' + str(v[0]) + ' ' + str(v[1]) + ' ' + str(v[2]) + '\n')
    #-- vertices for the textures
    if 'appearance' in cm:
        if 'materials' in cm['appearance']:
            for each in cm['appearance']['materials']:
                foutm.write('newmtl ' + each['name'] + '\n')
                foutm.write('Ka ' + str(each['ambient'][0]) + ' ' + str(each['ambient'][1]) + ' ' + str(each['ambient'][2])  + '\n')            
                foutm.write('Kd ' + str(each['diffuse'][0]) + ' ' + str(each['diffuse'][1]) + ' ' + str(each['diffuse'][2])  + '\n')            
                foutm.write('Ks ' + str(each['specular'][0]) + ' ' + str(each['specular'][1]) + ' ' + str(each['specular'][2])  + '\n')            
                foutm.write('illum ' + str(each['illumination']) + '\n')            
                foutm.write('Ns ' + str(each['specular-exponent']) + '\n')            
                foutm.write('\n')
        if 'textures' in cm['appearance']:
            for each in cm['appearance']['textures']:
                themtls.append(each['image'][:-4])
                foutm.write('newmtl ' + each['image'][:-4] + '\n')
                foutm.write('map_Kd ./appearances/' + each['image'] + '\n\n')
        if 'vertex-texture' in cm['appearance']:
            for v in cm['appearance']['vertex-texture']:
                fout.write('vt ' + str(v[0]) + ' ' + str(v[1]) + '\n')
    #-- the surfaces (with textures potentially)
    curtexture = -1
    curmtl = -1
    for theid in cm['CityObjects']:
        for geom in cm['CityObjects'][theid]['geometry']:
            fout.write('o ' + str(theid) + '\n')
            t = geom['texture'] if 'texture' in geom else None
            m = geom['material'] if 'material' in geom else None
            if ( (geom['type'] == 'MultiSurface') or (geom['type'] == 'CompositeSurface') ):
                process_list_surfaces(geom['boundaries'], t, m, cm, curtexture, curmtl, fout, foutm)
            elif geom['type'] == 'Solid':
                tt = t[i] if t is not None else None
                mm = m[i] if m is not None else None
                for i, shell in enumerate(geom['boundaries']):
                    process_list_surfaces(shell, tt, m, cm, curtexture, curmtl, fout, foutm)
            elif ( (geom['type'] == 'MultiSolid') or (geom['type'] == 'CompositeSolid') ):
                for i, solid in enumerate(geom['boundaries']):
                    for j, shell in enumerate(solid):
                        process_list_surfaces(shell, t[i][j], m[i][j], cm, curtexture, curmtl, fout, foutm)
                    

if __name__ == '__main__':
    write_obj()