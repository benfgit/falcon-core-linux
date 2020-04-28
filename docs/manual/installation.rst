Installation
============

User guide
**********

Who is concerned by this guide ?
--------------------------------
You just want to run falcon with a set of selected extensions and you don't want to look at the code ? This part is for you.
In parallel, we developed a cmake-client python gui to automatically create an installation of Falcon.

Installation
------------
.. code-block::

    git clone https://bitbucket.org/kloostermannerflab/fklab-cmake-gui
    cd fklab-cmake-gui
    conda env create -f falcon.yaml
    conda activate falcon
    python setup.py build_ext --inplace
    pip install -e . --no-deps

This environment will install the needed dependencies in a conda environment. You can read the readme.md in the fklab-cmake-gui repository
for more information on how the gui is working.
Some informations asked in the app :
 - repository path: https://bitbucket.org/kloostermannerflab/falcon-core.git
 - (last) version : 1.1.0   => See the changelog to see other available versions.

A grid with available extensions will be display. You can add your own extensions if needed but note that falcon-core does
not contains any extensions. If you want to use the core extensions, you need to have the "falcon-fklab-extension" selected.
The extensions are stored in this `repository <https://bitbucket.org/kloostermannerflab/falcon-fklab-extension.git>`_.

This step is optional and will allow falcon to more finely control CPU core utilization.

.. code-block::

    sudo setcap 'cap_sys_nice=pe' `which falcon`

Once, the app has been installed (without errors) you can continue to the section `usage <usage.rst>`_.

Developer guide
***************

The building of the falcon app is based on cmake and fetch_dependency to manage libraries and extensions.

Download
--------

Download the latest source from https://bitbucket.org/kloostermannerflab/falcon-core/downloads/?tab=downloads.

Dependencies
************

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

g3log
cmdline (header only library)
disruptor

Build instructions
******************

Compiling falcon has only been tested with GNU g++ compiler. You should use version 5 or upper.

The falcon-core repository does not contains any extensions. You will have to add, at least, the core extension to the CMakeList.txt.

.. toctree::

   internal/build_system

Once your cmakefile is all configurated, you can build the system.

So, to compile issue the following commands while in the falcon root directory:

.. code-block:: console

    mkdir build
    cd build
    cmake ..
    make install

For more informations on how to integrate third party extension to the build, refer to the build system documentation.

Installation instructions
*************************

.. code-block:: console

    cd falcon
    sudo setcap 'cap_sys_nice=pe' ./falcon

The last step is optional and will allow falcon to more finely control CPU
core utilization.