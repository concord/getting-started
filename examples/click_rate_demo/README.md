# Click Through Rate Demo

This demo is a simulation of a click through rate calculation performed in
real time, using the concord framework. There are two source computations
which generate random ad events and another computation that joins the streams 
aggregating on a publisher name.

## Running

The easiest way to run this example would be to run each computation in
our runtime_executor dockerized environment. It contains all of the necessary
dependencies to launch and run C++ Concord computations on mesos. Other
languages will only need to install the desired Concord library. The manifest
files in this project should include the following line in order to launch
inside of this container.

```
...
  "docker_container" : "concord/runtime_executor:0.3.13"
}
```

Running the computations is easy with the concord CLI. If you aren't running
in our vagrant box and you don't have the CLI installed you can easily install
the latest version via pip.

```
$ sudo pip install concord
...
$ concord deploy computation.json
...
```

## Building

Building this project is not yet supported in the getting-started project. You may
however build this yourself if you install the necessary deps on your local machine.

- Thrift 0.9.3 (headers, shared libs and the binary)
- CMake (2.6)
- C++ compiler (g++)
- Concord C++ client lib v0.3.13 (Check out ansible scripts in this repo for more info)
- Project deps:

```
set (CPP_CLIENT_LIBRARIES
  :libbolt_cpp_client.so
  :libglog.so.0
  :libthriftnb-0.9.3.so
  :libthrift-0.9.3.so
  :libthriftz-0.9.3.so
  :libglog.so.0.0.0
  :libpthread.so
  :libdouble-conversion.so.1.0
  :libboost_system.so.1.55.0
  :libevent.a # Hack couldn't get libevent to link
  :libglog.so.0
  :libstdc++.so.6
  :libssl.so.1.0.0
  :libcrypto.so.1.0.0
  :libgflags.so.2
  :libunwind.so.8
  :libpthread.so.0
  :libz.so.1.2.8
  )
```
