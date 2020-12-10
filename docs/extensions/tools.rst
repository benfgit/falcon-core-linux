Add Tools and Resources in an extension
=======================================

Tools are separable executable built at the same time of falcon-core. For example, the core extension is providing
two tools :

- NlxTestBench : generate input signals to use in test mode the neuralynx reader processors.
- FilterTest : allow to examine and re-create new filters to be use in the multichannelfilter processor.

Resources are a separable folder in each extension which will be combine in only one after build and added in the share
folder of the installation. Falcon has some URI path setup to this resource folder =

- resources://path = resources/path
- graphs://path = resources/graphs/path
- filters://path = resource/filters/path

You can also add your own folder in the resources folder with your own URI.


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
            - others folder (can be reach in falcon via its own uri or with resources://folder_name)
        - tools

Minimal CMakeList.txt in each extension (lib, processor, datatype) :

.. code-block::

    add_library( name "name.cpp" )
    TARGET_LINK_LIBRARIES( name lib_name )

.. note::

    lib_name could be a lib added in the extension but also already present in the falcon-core or others extensions
    also present in extensions.txt