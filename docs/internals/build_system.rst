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

To manage the extension, before the build, open the CMakeLists.txt at the root of falcon-core.
Does not modify the part "Basic libraries" as it included libraries used by falcon core.
However, the section ## External extensions ## is the part interesting for us.

As it shows below, there is two important parts:

- **Declare** : link to the git repository
- **Populate** : Set the test option (specific to the extension) to false as building the unitary test
  for each extension could take times and throw errors not interesting in this case.

.. code-block:: console

    ###################################
    # Define every external extension #
    ###################################

    FetchContent_Declare(
        extension
        GIT_REPOSITORY Local/path/extensions or http:git/path/extensions
        GIT_TAG version_tag
    )

    ## Add definitions here ...

    ###############################################
    # Populate -- only populate extensions wanted #
    # Dont forget to disable tests                #
    ###############################################

    FetchContent_GetProperties(extension)
    if(NOT extension_POPULATED)
        message(STATUS "Populating extension...")
        FetchContent_Populate(extension)
        set(TEST_OPTION OFF CACHE BOOL "disable tests")
        add_subdirectory(${extension_SOURCE_DIR} ${extension_BINARY_DIR})
        message(STATUS "Done.")
    endif()

    include_directories(BEFORE SYSTEM ${extension_SOURCE_DIR} ${extension_BINARY_DIR}/include)

    ## Populate new extensions here

Python install
..............

TBD



