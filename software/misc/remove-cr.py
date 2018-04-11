#!/usr/local/bin/python

# small script to remove carriage returns and blank spaces from an XML file
# used to compared file sizes with JSON (where we remove them too, fair is fair)

from lxml import etree
import sys

fin = sys.argv[1]

parser = etree.XMLParser(remove_blank_text=True)
tree = etree.parse(fin, parser)
root = tree.getroot()   

sys.stdout.write(etree.tostring(root, pretty_print=False, xml_declaration=True, encoding="UTF-8"))