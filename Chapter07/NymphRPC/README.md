# NymphRPC #

**Author:** Maya Posch

**Date:** 2018/01/27

## Overview ##

NymphRPC is a compact, C++-based Remote Procedure Call (RPC) library. One can look at the example server and client implementations in the `test` folder to get an idea of how NymphRPC is integrated into an application.

## Planned ports ##

In addition to the current C++ implementation, an Ada and C version of the library are also planned.

## Building ##

NymphRPC's C++ port is based around the POCO library (see [http://wwww.pocoproject.org/](http://wwww.pocoproject.org/ "POCO Project") ). It is recommended to download the full source from the website and compile it using the instructions to obtain the current version of POCO instead of an ancient one from a Linux distro repository.

With the common C++ build tools installed (g++, make, etc.) and the optional OpenSSL dependency, building NymphRPC is done using:

`make lib` - Build just the library. Found under `lib`.

`make test-client` - Build the test client.

`make test-server` - Build the test server.

`make test` - Build both the test client & server.

`make clean` - Clean all targets.

`make clean-lib` - Clean just the `lib` target.

`make clean-test` - Clean just the `test` target.

`make clean-test-client` - Clean just the test client.

`make clean-test-server` - Clean just the test server.

## Installing ##

Installing the resulting `libnymphrpc` library has to be done by hand, currently. On Linux one would copy `libnymphrpc.a` to `/usr/lib/` or similar, and the header files to `/usr/include` or similar. Consult the documentation for your OS and development tools for more detailed instructions.

