
- Nothing needs to be installed, both dependencies are in the `include` folder
- C++11 is necessary

To compile and run:

    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
    $ ./cityjson-valschema ../../../../example-datasets/dummy/example.json

The appropriate version of the schema is read from the `version` of the input file and used to validate.    