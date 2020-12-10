
========================
Installation from source
========================

The building of the falcon app is based on cmake and fetch_dependency to manage libraries and extensions. The plugin
can be specified by modifying the extensions.txt file at the root of falcon_core.

Download
--------

Download the latest source from https://bitbucket.org/kloostermannerflab/falcon-core/downloads/?tab=downloads.

Dependencies
------------

- **CMAKE**

The build system is based on CMake (minimum version 3.11).
Last version of CMake are available through pip.

.. code-block:: console

    pip install cmake

- **zeromq**

.. code-block:: console

    sudo apt-get install libzmq3-dev

- **g++-5 (or upper)**

G++ v.5 or upper is needed in order to have all libraries of the C++11 standard.
In order to install it type in a terminal:

.. code-block:: console

    sudo apt-get install g++


- **External libraries included in source tree** (just for information, you don't need to do anything normally)

    + cmdline (header only library)
    + disruptor
    + yaml-cpp : https://github.com/jbeder/yaml-cpp.git
    + cppzmq : https://github.com/zeromq/cppzmq.git
    + g3log : https://github.com/KjellKod/g3log.git
    + unit : https://github.com/LLNL/units.git

Build instructions
------------------

Compiling falcon has only been tested with GNU g++ compiler. You should use version 5 or upper.
The falcon-core repository does not contains any extensions. You will have to add, at least, the core extension to the extensions.txt.

*How extensions are found and added to build ?*

Extensions are added through the FetchContent feature of CMake. It allows to link in the Falcon CMake
the different git repository (or local folder) containing the extension. This extension needs to contain at least CMake.
This solution allows to use a specific version of an extension by adding a tag version in the option.

The CmakeList.txt will read the extensions.txt file described below :

.. code-block::

    enable , extension name , extension path , extension version (optional)
    1 , extensions , https://bitbucket.org/kloostermannerflab/falcon-fklab-extensions

Enable can be 3 different values : 0 (not build)/ 1 (build)/ dev (develop mode)

The build mode will import the repository in the commit state (when not specified, the commit is the last one on the master head).
The dev mode will build the repository in its actual local state.


Command line build
..................

So, to compile issue the following commands while in the falcon root directory:

.. code-block:: console

    mkdir build
    cd build
    cmake ..
    make install

For more information on how to integrate third party extension to the build, refer to the build system documentation.


Installation instructions
-------------------------

.. code-block:: console

    cd falcon
    sudo setcap 'cap_sys_nice=pe' ./falcon

The last step is optional and will allow falcon to more finely control CPU core utilization.
