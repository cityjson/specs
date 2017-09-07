#!/usr/bin/env ruby
#  _____ _ _          __ _____ _____ _____     _     ___     
# |     |_| |_ _ _ __|  |   __|     |   | |___|_|___|  _|___ 
# |   --| |  _| | |  |  |__   |  |  | | | |___| |   |  _| . |
# |_____|_|_| |_  |_____|_____|_____|_|___|   |_|_|_|_| |___|
#             |___|                                          

#   cityjson-info
#   Created by Ken Arroyo Ohori on 07/09/2017.
#   Copyright © 2017 Ken Arroyo Ohori. All rights reserved.

require 'json'
require 'set'
require 'pp'

ARGV.length >= 1 ? filename = ARGV[0] : filename = "../../../example-datasets/Rotterdam/Delfshaven/3-20-DELFSHAVEN.json"
input = File.read(filename)
j = JSON.parse(input)

puts "Printing (some) information about:"
puts "  " + filename + "\n"

# CityJSON version
version = j["version"][-3, 3]
puts "CityJSON version: " + version

# CityObjects
puts "===== CityObjects ====="
puts "Total : " + j["CityObjects"].length.to_s
d = Set.new
j["CityObjects"].each do |id, co|
	d.add(co["type"])
end
puts "Types:"
d.each do |each|
	puts "  " + each.to_s
end
d.clear
j["CityObjects"].each do |id, co|
	co["geometry"].each do |g|
		d.add(g["type"])
	end
end
puts "Geometries present:"
d.each do |each|
	puts "  " + each.to_s
end
d.clear

# metadata
puts "===== Metadata ====="
if !j.has_key?("metadata")
	puts "  none"
else
	j["metadata"].each do |id, it|
		if id == "crs" 
			puts "  crs: EPSG:" + j["metadata"]["crs"]["epsg"].to_s
		else
			puts "  " + id
		end
	end
end

# vertices
puts "===== Vertices ====="
puts "Total: " << j["vertices"].length.to_s

# appearance
puts "===== Appearance ====="
if !j.has_key?("appearance")
  puts "  none"
else
  if j["appearance"].has_key?("textures")
    puts "  textures: " + j["appearance"]["textures"].length.to_s
  end
  if (j["appearance"].count("materials") > 0)
    puts "  materials: " + j["appearance"]["materials"].length.to_s
  end
end