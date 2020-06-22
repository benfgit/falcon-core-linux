Build system
============

Configuration options
---------------------

?

How extensions are found and added to build ?
---------------------------------------------

Extensions are added through the FetchContent feature of CMake. It allows to link in the Falcon CMake
the different git repository (or local folder) containing the extension. This extension needs to contain
a CMake.
This solution allows to use a specific version of an extension by adding a tag version in the option.
The core extension are listed in the extension.yaml at the root of the repository.

Manual install
..............

.. code-block::
    


Python install
..............

TBD



