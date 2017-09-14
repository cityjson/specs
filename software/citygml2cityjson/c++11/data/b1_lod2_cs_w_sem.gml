<?xml version="1.0" encoding="utf-8"?>
<!-- hand-coded with <3 by the 3D Geoinformation group at the TU Delft -->
<!-- https://3d.bk.tudelft.nl -->
<CityModel xmlns:smil20="http://www.w3.org/2001/SMIL20/" xmlns:grp="http://www.opengis.net/citygml/cityobjectgroup/2.0" xmlns:pfx0="http://www.citygml.org/citygml/profiles/base/2.0" xmlns:luse="http://www.opengis.net/citygml/landuse/2.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:frn="http://www.opengis.net/citygml/cityfurniture/2.0" xmlns:smil20lang="http://www.w3.org/2001/SMIL20/Language" xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:tex="http://www.opengis.net/citygml/texturedsurface/2.0" xmlns:dem="http://www.opengis.net/citygml/relief/2.0" xmlns:tran="http://www.opengis.net/citygml/transportation/2.0" xmlns:wtr="http://www.opengis.net/citygml/waterbody/2.0" xmlns:xAL="urn:oasis:names:tc:ciq:xsdschema:xAL:2.0" xmlns:bldg="http://www.opengis.net/citygml/building/2.0" xmlns:sch="http://www.ascc.net/xml/schematron" xmlns:app="http://www.opengis.net/citygml/appearance/2.0" xmlns:veg="http://www.opengis.net/citygml/vegetation/2.0" xmlns:gml="http://www.opengis.net/gml" xmlns:gen="http://www.opengis.net/citygml/generics/2.0" xmlns="http://www.opengis.net/citygml/2.0" xsi:schemaLocation="http://www.opengis.net/citygml/2.0 http://schemas.opengis.net/citygml/2.0/cityGMLBase.xsd  http://www.opengis.net/citygml/appearance/2.0 http://schemas.opengis.net/citygml/appearance/2.0/appearance.xsd http://www.opengis.net/citygml/building/2.0 http://schemas.opengis.net/citygml/building/2.0/building.xsd http://www.opengis.net/citygml/cityfurniture/2.0 http://schemas.opengis.net/citygml/cityfurniture/2.0/cityFurniture.xsd http://www.opengis.net/citygml/vegetation/2.0 http://schemas.opengis.net/citygml/vegetation/2.0/vegetation.xsd http://www.opengis.net/citygml/generics/2.0 http://schemas.opengis.net/citygml/generics/2.0/generics.xsd http://www.opengis.net/citygml/transportation/2.0 http://schemas.opengis.net/citygml/transportation/2.0/transportation.xsd http://www.opengis.net/citygml/waterbody/2.0 http://schemas.opengis.net/citygml/waterbody/2.0/waterBody.xsd http://www.opengis.net/citygml/landuse/2.0 http://schemas.opengis.net/citygml/landuse/2.0/landUse.xsd http://www.opengis.net/citygml/relief/2.0 http://schemas.opengis.net/citygml/relief/2.0/relief.xsd">
	<cityObjectMember>
		<bldg:Building>
			<bldg:lod2Solid>
				<!-- notice that here the volume of the building is subdivided into 2 solids, and a new surface representing the ceiling of the first flooris added (gml:id="internal-ceiling") -->
				<gml:CompositeSolid>
					<gml:solidMember>
						<gml:Solid>
							<gml:exterior>
								<gml:CompositeSurface>
									<gml:surfaceMember xlink:href="#b_ground"/>
									<gml:surfaceMember xlink:href="#b_wall_1"/>
									<gml:surfaceMember xlink:href="#b_wall_2"/>
									<gml:surfaceMember xlink:href="#b_wall_3"/>
									<gml:surfaceMember xlink:href="#b_wall_4"/>
									<gml:surfaceMember xlink:href="#internal-ceiling"/>
								</gml:CompositeSurface>
							</gml:exterior>
						</gml:Solid>
					</gml:solidMember>
					<gml:solidMember>
						<gml:Solid>
							<gml:exterior>
								<gml:CompositeSurface>
									<!-- here the internal-ceiling surface is reused, but its orientation is flipped -->
									<gml:surfaceMember>
										<gml:OrientableSurface orientation="-">
											<gml:baseSurface xlink:href="#internal-ceiling"/>
										</gml:OrientableSurface>
									</gml:surfaceMember>
									<gml:surfaceMember xlink:href="#b_roof_1"/>
									<gml:surfaceMember xlink:href="#b_roof_2"/>
									<gml:surfaceMember xlink:href="#b_roof_3"/>
									<gml:surfaceMember xlink:href="#b_roof_4"/>
								</gml:CompositeSurface>
							</gml:exterior>
						</gml:Solid>
					</gml:solidMember>					
				</gml:CompositeSolid>		
			</bldg:lod2Solid>
			<!-- the extra surface used to subdivide the volume of the building into a CompositeSolid -->
			<bldg:lod2MultiSurface>
				<gml:MultiSurface>
					<gml:surfaceMember>
						<gml:Polygon gml:id="internal-ceiling">
							<gml:exterior>
								<gml:LinearRing>
									<gml:pos>0.00 0.00 100.00</gml:pos>
									<gml:pos>100.00 0.00 100.00</gml:pos>
									<gml:pos>100.00 100.00 100.00</gml:pos>
									<gml:pos>0.00 100.00 100.00</gml:pos>
									<gml:pos>0.00 0.00 100.00</gml:pos>
								</gml:LinearRing>
							</gml:exterior>
						</gml:Polygon>
					</gml:surfaceMember>
				</gml:MultiSurface>
			</bldg:lod2MultiSurface>
			<!-- all the boundary surfaces -->
			<bldg:boundedBy>
				<bldg:GroundSurface gml:id="b_ground_sem">
					<bldg:lod2MultiSurface>
						<gml:MultiSurface>
							<gml:surfaceMember>
								<gml:Polygon gml:id="b_ground">
									<gml:exterior>
										<gml:LinearRing>
											<gml:pos>0.00 0.00 0.00</gml:pos>
											<gml:pos>0.00 100.00 0.00</gml:pos>
											<gml:pos>100.00 100.00 0.00</gml:pos>
											<gml:pos>100.00 0.00 0.00</gml:pos>
											<gml:pos>0.00 0.00 0.00</gml:pos>
										</gml:LinearRing>
									</gml:exterior>
								</gml:Polygon>
							</gml:surfaceMember>
						</gml:MultiSurface>
					</bldg:lod2MultiSurface>
				</bldg:GroundSurface>
			</bldg:boundedBy>
			<bldg:boundedBy>
				<bldg:WallSurface gml:id="b_wall_1_sem">
					<bldg:lod2MultiSurface>
						<gml:MultiSurface>						
							<gml:surfaceMember>
								<gml:Polygon gml:id="b_wall_1">
									<gml:exterior>
										<gml:LinearRing>
											<gml:pos>0.00 0.00 0.00</gml:pos>
											<gml:pos>100.00 0.00 0.00</gml:pos>
											<gml:pos>100.00 0.00 100.00</gml:pos>
											<gml:pos>0.00 0.00 100.00</gml:pos>
											<gml:pos>0.00 0.00 0.00</gml:pos>
										</gml:LinearRing>
									</gml:exterior>
								</gml:Polygon>
							</gml:surfaceMember>
						</gml:MultiSurface>
					</bldg:lod2MultiSurface>
				</bldg:WallSurface>
			</bldg:boundedBy>
			<bldg:boundedBy>
				<bldg:WallSurface gml:id="b_wall_2_sem">
					<bldg:lod2MultiSurface>
						<gml:MultiSurface>						
							<gml:surfaceMember>
								<gml:Polygon gml:id="b_wall_2">
									<gml:exterior>
										<gml:LinearRing>
											<gml:pos>100.00 0.00 0.00</gml:pos>
											<gml:pos>100.00 100.00 0.00</gml:pos>
											<gml:pos>100.00 100.00 100.00</gml:pos>
											<gml:pos>100.00 0.00 100.00</gml:pos>
											<gml:pos>100.00 0.00 0.00</gml:pos>
										</gml:LinearRing>
									</gml:exterior>
								</gml:Polygon>
							</gml:surfaceMember>
						</gml:MultiSurface>
					</bldg:lod2MultiSurface>
				</bldg:WallSurface>
			</bldg:boundedBy>
			<bldg:boundedBy>
				<bldg:WallSurface gml:id="b_wall_3_sem">
					<bldg:lod2MultiSurface>
						<gml:MultiSurface>						
							<gml:surfaceMember>
								<gml:Polygon gml:id="b_wall_3">
									<gml:exterior>
										<gml:LinearRing>
											<gml:pos>100.00 100.00 0.00</gml:pos>
											<gml:pos>0.00 100.00 0.00</gml:pos>
											<gml:pos>0.00 100.00 100.00</gml:pos>
											<gml:pos>100.00 100.00 100.00</gml:pos>
											<gml:pos>100.00 100.00 0.00</gml:pos>
										</gml:LinearRing>
									</gml:exterior>
								</gml:Polygon>
							</gml:surfaceMember>
						</gml:MultiSurface>
					</bldg:lod2MultiSurface>
				</bldg:WallSurface>
			</bldg:boundedBy>
			<bldg:boundedBy>
				<bldg:WallSurface gml:id="b_wall_4_sem">
					<bldg:lod2MultiSurface>
						<gml:MultiSurface>						
							<gml:surfaceMember>
								<gml:Polygon gml:id="b_wall_4">
									<gml:exterior>
										<gml:LinearRing>
											<gml:pos>0.00 100.00 0.00</gml:pos>
											<gml:pos>0.00 0.00 0.00</gml:pos>
											<gml:pos>0.00 0.00 100.00</gml:pos>
											<gml:pos>0.00 100.00 100.00</gml:pos>
											<gml:pos>0.00 100.00 0.00</gml:pos>
										</gml:LinearRing>
									</gml:exterior>
								</gml:Polygon>
							</gml:surfaceMember>
						</gml:MultiSurface>
					</bldg:lod2MultiSurface>
				</bldg:WallSurface>
			</bldg:boundedBy>
			<bldg:boundedBy>
				<bldg:RoofSurface gml:id="b_roof_1_sem">
					<bldg:lod2MultiSurface>
						<gml:MultiSurface>						
							<gml:surfaceMember>
								<gml:Polygon gml:id="b_roof_1">
									<gml:exterior>
										<gml:LinearRing>
											<gml:pos>0.00 0.00 100.00</gml:pos>
											<gml:pos>100.00 0.00 100.00</gml:pos>
											<gml:pos>50.00 0.00 150.00</gml:pos>
											<gml:pos>0.00 0.00 100.00</gml:pos>
										</gml:LinearRing>
									</gml:exterior>
								</gml:Polygon>
							</gml:surfaceMember>
						</gml:MultiSurface>
					</bldg:lod2MultiSurface>
				</bldg:RoofSurface>
			</bldg:boundedBy>
			<bldg:boundedBy>
				<bldg:RoofSurface gml:id="b_roof_2_sem">
					<bldg:lod2MultiSurface>
						<gml:MultiSurface>						
							<gml:surfaceMember>
								<gml:Polygon gml:id="b_roof_2">
									<gml:exterior>
										<gml:LinearRing>
											<gml:pos>100.00 100.00 100.00</gml:pos>
											<gml:pos>0.00 100.00 100.00</gml:pos>
											<gml:pos>50.00 100.00 150.00</gml:pos>
											<gml:pos>100.00 100.00 100.00</gml:pos>
										</gml:LinearRing>
									</gml:exterior>
								</gml:Polygon>
							</gml:surfaceMember>
						</gml:MultiSurface>
					</bldg:lod2MultiSurface>
				</bldg:RoofSurface>
			</bldg:boundedBy>
			<bldg:boundedBy>
				<bldg:RoofSurface gml:id="b_roof_3_sem">
					<bldg:lod2MultiSurface>
						<gml:MultiSurface>						
							<gml:surfaceMember>
								<gml:Polygon gml:id="b_roof_3">
									<gml:exterior>
										<gml:LinearRing>
											<gml:pos>100.00 0.00 100.00</gml:pos>
											<gml:pos>100.00 100.00 100.00</gml:pos>
											<gml:pos>50.00 100.00 150.00</gml:pos>
											<gml:pos>50.00 0.00 150.00</gml:pos>
											<gml:pos>100.00 0.00 100.00</gml:pos>
										</gml:LinearRing>
									</gml:exterior>
								</gml:Polygon>
							</gml:surfaceMember>
						</gml:MultiSurface>
					</bldg:lod2MultiSurface>
				</bldg:RoofSurface>
			</bldg:boundedBy>
			<bldg:boundedBy>
				<bldg:RoofSurface gml:id="b_roof_4_sem">
					<bldg:lod2MultiSurface>
						<gml:MultiSurface>						
							<gml:surfaceMember>
								<gml:Polygon gml:id="b_roof_4">
									<gml:exterior>
										<gml:LinearRing>
											<gml:pos>0.00 0.00 100.00</gml:pos>
											<gml:pos>50.00 0.00 150.00</gml:pos>
											<gml:pos>50.00 100.00 150.00</gml:pos>
											<gml:pos>0.00 100.00 100.00</gml:pos>
											<gml:pos>0.00 0.00 100.00</gml:pos>
										</gml:LinearRing>
									</gml:exterior>
								</gml:Polygon>
							</gml:surfaceMember>
						</gml:MultiSurface>
					</bldg:lod2MultiSurface>
				</bldg:RoofSurface>
			</bldg:boundedBy>
		</bldg:Building>
	</cityObjectMember>
</CityModel>
