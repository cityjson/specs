
To compile and run:

    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
    $ ./cjcompress --help

    $ ./cjcompress Chicoutimi.json --tolerance 3

The `--tolerance 3` given as input is the number of digits to keep after the dot, thus "mm" precision here (to merge duplicate vertices); 3 is the default.

It will create a file `Chicoutimi.compress.json`.

    $ ./cjcompress Chicoutimi.json --v2int

To convert vertices to integers and add the ``"transform"`` to the CityJSON file

