Installation
============

Download
--------

Download the latest source from https://bitbucket.org/kloostermannerflab/falcon-core/downloads/?tab=downloads.

Requirements
------------

yaml-cpp (version 0.5 or higher)
zeromq
cmake (version 3.5 or higher)
g++ (version 5.4 or higher)

.. code-block:: bash

    sudo apt-get install libyaml-cpp-dev libzmq3-dev cmake build-essential

To check your g++ version:

.. code-block:: bash

    g++ --version

Compile
-------

Compiling falcon has only been tested with GNU g++ compiler. Version 5 is
required, as it contains all C++11 libraries. To compile issue the following
commands while in the falcon root directory:

.. code-block:: bash

    mkdir build
    cd build
    cmake -DCMAKE_CXX_COMPILER=`which g++-5` ..
    make

Compile extra extensions
------------------------

Falcon is compile-time extendable with new data types and processor nodes.
The *extensions* folder in the source tree contains data types, processor
nodes and tools that ship with Falcon. To compile 3rd party extensions,
execute the cmake command with a list of extension directories (in quotes,
and separated by semi-colon):

.. code-block:: bash

    cd build
    cmake -DFALCON_EXTENSIONS="/path/to/extensions;/path/to/more/extensions" \
          -DCMAKE_CXX_COMPILER=`which g++-5` ..
    make


Install
-------

.. code-block:: bash

    cd build/src
    cp falcon ~/bin
    sudo setcap 'cap_sys_nice=pe' ~/bin/falcon

The last step is optional and will allow falcon to more finely control CPU
core utilization.
