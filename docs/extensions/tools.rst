Tool extension
==============

How to package an extension
===========================

Example of an extension structure :

.. code-block::

    extension_repository :
        - processors
            - processor_name
                - doc.yaml
                - processor_name.cpp
                - processor_name.hpp
                - CMakeList.txt
        - datatypes
            (similar structure to processors)
        - lib
            - Lib1
                - code
                - CMakeList.txt
            - Lib2
            - CMakeList.txt
        - resources
            - graphs
            - filters
            - others folder (but it will need to configure its own uri to reach it in falcon)
        - tools

Minimal CMakeList.txt in each extension (lib, processor, datatype) :

.. code-block::

    add_library( name "name.cpp" )
    TARGET_LINK_LIBRARIES( name lib_name )

.. note:: lib_name could be lib added in the extension but also already present in the falcon-core.