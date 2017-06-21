
To compile and run:

    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
    $ ./cityjson-compress Chicoutimi.json 3

The "3" given as input is the number of digits to keep after the dot, thus "mm" precision here (to merge duplicate vertices).

It will create a file `Chicoutimi.compress.json`.

