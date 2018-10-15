#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Sep 11 11:39:04 2018

@author: kavisha
"""

#validate cityjson

import json
import jsonschema
from jsonschema import Draft7Validator
#import os

filename =  r'example-datasets/extensions/montreal_energy.json'
schemaname = r'schema/v09/extensions/energy.json'

fin = open(filename)
data = fin.read()
j = json.loads(data)
#print (j)

fins = open(schemaname)
schema = fins.read()
js = json.loads(schema)
#print js

#print ('file://%s/' % os.path.abspath(os.path.dirname(__file__)) +"cityjson/schema/v07/")
resolver = jsonschema.RefResolver('file:///Users/kavisha/Desktop/githubpvt/CityJSON_Energy_Extension/schema/v09/', None)
#print (resolver)
try:
#    jsonschema.validate(j,js,resolver=resolver)
    Draft7Validator.check_schema(js)
    print("Passed derived schema.")
#except Exception as ex:
#    print("Failed derived schema: %s" % ex)
except jsonschema.ValidationError as e:
    print ("hi",e.message)
except jsonschema.SchemaError as e:
    print ("ho",e)

try:
#    jsonschema.validate(j,js,resolver=resolver)
    Draft7Validator(js,resolver=resolver).validate(j)
    print("Passed derived schema.")
#except Exception as ex:
#    print("Failed derived schema: %s" % ex)
except jsonschema.ValidationError as e:
    print ("hi",e.message)
except jsonschema.SchemaError as e:
    print ("ho",e)
#print (Draft7Validator.check_schema(schema))