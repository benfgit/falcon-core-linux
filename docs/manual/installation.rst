Installation
============

Download
********

Download the latest source from https://bitbucket.org/kloostermannerflab/falcon-core/downloads/?tab=downloads.

Dependencies
************

- **zeromq**

.. code-block:: console

    sudo apt-get install libzmq3-dev

- **CMAKE**

.. code-block:: console

    pip install cmake


- **g++-5 (or upper)**

G++ v.5 or upper is needed in order to have all libraries of the C++11 standard.
In order to install it type in a terminal:

.. code-block:: console

    sudo apt-get install g++


- **External libraries included in source tree** (just for information, you don't need to do anything normally)

g3log
cmdline (header only library)
disruptor

Build instructions
******************

Compiling falcon has only been tested with GNU g++ compiler.
You should use version 5 or upper.
So, to compile issue the following commands while in the falcon root directory:

.. code-block:: console

    mkdir build
    cd build
    cmake ..
    make

For more informations on how to integrate third party extension to the build, refere to the build system documentation.

Installation instructions
*************************

.. code-block:: console

    cd falcon
    sudo setcap 'cap_sys_nice=pe' ./falcon

The last step is optional and will allow falcon to more finely control CPU
core utilization.

