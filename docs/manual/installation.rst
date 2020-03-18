Installation
============

Download
********

Download the latest source from https://bitbucket.org/kloostermannerflab/falcon-core/downloads/?tab=downloads.

Dependencies
************

- **zeromq**

::
    sudo apt-get install libzmq3-dev

- **C++ boost libraries**

.. code-block:: console

    sudo apt-get install libboost-all-dev

- **CMAKE**

.. code-block:: console

    pip install cmake

- **GSL libraries**

.. code-block:: console

    sudo apt-get install libgsl23 libgslcblas0:i386 libgsl23:i386 libgslcblas0
    sudo apt-get install libgsl0-dev

- **Z-lib (needed by cnpy)**

.. code-block:: console

    sudo apt-get install zlib1g
    sudo apt-get install zlib1g-dev


- **g++-5 (or upper)**

G++ v.5 or upper is needed in order to have all libraries of the C++11 standard.
In order to install it type in a terminal:

.. code-block:: console

    sudo apt-get install g++-7

If it does not work type the following command:

.. code-block:: console

    sudo add-apt-repository ppa:ubuntu-toolchain-r/test && sudo apt-get update && sudo apt-get install g++-7

- **External libraries included in source tree** (just for information, you don't need to do anything normally)

g3log
cppzmq (header only library)
cmdline (header only library)
disruptor
cnpy
yaml-cpp 0.6.0

Build instructions
******************

Compiling falcon has only been tested with GNU g++ compiler.
You should use version 5 or upper.
So, to compile issue the following commands while in the falcon root directory:

.. code-block:: console

    mkdir build
    cd build
    cmake -DCMAKE_CXX_COMPILER=`which g++-7` ..
    make

For more informations on how to integrate third party extension to the build, refere to the build system documentation.

Installation instructions
*************************

.. code-block:: console

    cd falcon
    sudo setcap 'cap_sys_nice=pe' ./falcon

The last step is optional and will allow falcon to more finely control CPU
core utilization.

