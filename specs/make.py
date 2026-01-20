import subprocess

from bs4 import BeautifulSoup

subprocess.run(["bikeshed", "spec", "specs.bs", "temp.html"])

with open("temp.html", "r") as f:
    a = f.read()

soup = BeautifulSoup(a, features="lxml")
re = soup.find("p", class_="copyright")
newp = BeautifulSoup(
    '<p class="copyright" xmlns:cc="http://creativecommons.org/ns#" xmlns:dct="http://purl.org/dc/terms/"><img style="height:16px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/cc.svg?ref=chooser-v1"><img style="height:16px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/zero.svg?ref=chooser-v1"> The editors have waived all copyright and related or neighbouring rights to this work. The CityJSON Specifications are marked with <a href="http://creativecommons.org/publicdomain/zero/1.0" target="_blank" rel="license noopener noreferrer" style="display:inline-block;">CC0 1.0 Universal</a>.</p>',
    features="lxml",
)
re.insert_after(newp)
re.decompose()
fout = open("specs.html", "w")
fout.write(str(soup))
fout.close()
