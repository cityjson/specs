
This is a [bikeshed](https://github.com/tabatkins/bikeshed) file, very similar to Markdown.


To compile with a nice CC0 logo and not the awful one by default in Bikeshed `python make.py` and this will generate a file `specs.html`.
You must `pip install beautifulsoup4`.


To convert it to an HTML file with the ugly CC0 image directly: `curl https://api.csswg.org/bikeshed/ -F file=@specs.bs -F force=1 > specs.html`

