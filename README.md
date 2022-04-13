# EESDS: Energy Efficient Secure Distributed Storage
The implementation of the paper ["Energy-efficient secure distributed storage in mobile cloud computing"](https://ieeexplore.ieee.org/document/7146311?arnumber=7146311).


# Abstract

In the mobile cloud computing, one of the main concerns is to preserve the confidentiality and integrity of the outsourced data. Trust relations always have been one of the key factors in designing a security architecture for outsourcing data in mobile cloud computing in order to offload some of the computation overhead like pre-encryptions to a third trusted party which in practical environment it is not a wise idea, especially when we have a security sensitive data. In this paper, we present a method to improve Rabin's IDA to further be used for secure dispersal of information by employing a lightweight energy-efficient pre-processing phase before application of the IDA. In the pre-processing phase, we produce a cipher key using a selfie picture taken by the user. Further we employ a method of key management such that in case of missing one file, there would be no way of reconstructing the file while relieving the user from key management complexities. Due to our method's low-energy consuming nature, it can be confidently used in mobile cloud computing.


## Structure
``` text
.
├── CMakeLists.txt
├── app
│   └── main.cpp
├── include
│   ├── include.h
│   ├── inputstream.h
│   ├── md5.h
│   ├── outputstream.h
│   ├── rabinida.h
│   ├── utility.h
├── src
│   ├── inputstream.cpp
│   ├── md5.cpp
│   ├── outputstream.cpp
│   ├── rabinida.cpp
│   ├── utility.cpp
└── tests
    ├── dummy.cpp
    └── main.cpp
```

Sources go in [src/](src/), header files in [include/](include/), main programs in [app/](app), and
tests go in [tests/](tests/) (compiled to `unit_tests` by default).

If you add a new executable, say `app/hello.cpp`, you only need to add the following two lines to [CMakeLists.txt](CMakeLists.txt):

``` cmake
add_executable(main app/main.cpp)   # Name of exec. and location of file.
target_link_libraries(main PRIVATE ${LIBRARY_NAME})  # Link the executable to lib built from src/*.cpp (if it uses it).
```

You can find the example source code that builds the `main` executable in [app/main.cpp](app/main.cpp) under the `Build` section in [CMakeLists.txt](CMakeLists.txt).
If the executable you made does not use the library in [src/](src), then only the first line is needed.



## Building

Build by making a build directory (i.e. `build/`), run `cmake` in that dir, and then use `make` to build the desired target.

Example:

``` bash
> mkdir build && cd build
> cmake .. -DCMAKE_BUILD_TYPE=[Debug | Coverage | Release]
> make
> ./main
> make test      # Makes and runs the tests.
> make coverage  # Generate a coverage report.   #TODO
> make doc       # Generate html documentation.  #TODO
```


