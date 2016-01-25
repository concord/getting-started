# getting-started - cpp

In the latest iteration of the cpp word count example we have migrated
away from SCons and moved to CMake for managing the build process in this
project. Before you begin, you will need to have CMake installed in order to
compile this example. If you are running this example inside of the vagrant
box then your already set; otherwise follow the directions
[here](https://cmake.org/install/) to install CMake.

To run the example, first compile the project using the compile script:
```
$ ./compile.bash
```

Finally, launch the computations:
```
$ for file in $(ls ./*.json); do concord deploy $file; done
```
