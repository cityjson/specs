require 'json'
require 'pp'

test01file = File.read('test01ken.json')
test01json = JSON.parse(test01file)

# Get all objects
print "Getting all objects\n"
pp test01json["CityObjects"]

# Get all surfaces for visualisation
print "Getting all surfaces for visualisation\n"
test01json["CityObjects"].each do |key,value|
	print "object " + key + ":\n"
	if value["geometry"]["type"] == "MultiSurface" || value["geometry"]["type"] == "Solid" then
		value["geometry"]["surfaces"].each do |surface|
			print "\tsurface:\n"
			surface.each do |polygon|
				print "\t\tpolygon:\n"
				polygon.each do |vertex|
					print "\t\t\t" + test01json["vertices"][vertex].to_s + "\n"
				end
			end
		end
	else
		print "type " + value["geometry"]["type"] + " unsupported\n"
	end
end

# Get all surfaces of object "102636712"
print "Getting all surfaces of object 102636712\n"
test01json["CityObjects"]["102636712"]["geometry"]["surfaces"].each do |surface|
	print "\tsurface:\n"
	surface.each do |polygon|
		print "\t\t\t" + polygon.to_s + "\n"
	end
end
test01json["CityObjects"]["102636712"]["properties"]["parts"].each do |part|
	test01json["CityObjects"][part]["geometry"]["surfaces"].each do |surface|
	print "\tsurface:\n"
	surface.each do |polygon|
		print "\t\t\t" + polygon.to_s + "\n"
	end
end
end