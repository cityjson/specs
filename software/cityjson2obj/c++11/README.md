
- Nothing needs to be installed, this [JSON header-only library](https://github.com/nlohmann/json) is used
- C++11 is necessary

- materials and textures not handled (they are ignored)
- inner rings in polygon are also ignored (TODO: fix this, anyone volunteers?)

To compile and run:

    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
    $ ./cityjson-info ../../../../example-datasets/Munich/LOD2_4424_5482_solid.json