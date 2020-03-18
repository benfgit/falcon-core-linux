Create new processor
====================

The new processor class needs to derive from the Iprocessor class.

Two important inputs are :

- const YAML: Node &node : The description from the graph of the node with the different parameters related to the processor
- Context : There is a GlobalContext and a ProcessingContext structure available to be use in the class. (see context documentation)

Virtual methods from the IProcessor class are available to be override:

-   *Configure(const YAML::Node &node, const GlobalContext& context)* : The graph (yaml file) describe the node
    with some parameters specific to the processor. This method is the time to read it, process it and store it for later.

-   *CreatePorts()* : This part make use of the internal available methods from Iprocessor (see the API documentation)
    for creating input port (*create_input_port*), readable sharable state (*create_readable_shared_state*)
    and output port (*create_output_port*).

-   *CompleteStreamInfo()* (?)

-   *Prepare(GlobalContext& context)* : prepare state of the node aka connecting servor ... etc.

-   *Unprepare(GlobalContext& context)* : undo the prepare method

-   *Preprocess(ProcessingContext& context)* : pre-process state of the note aka clear states

-   *Process(ProcessingContext& context)* : process state of the node : for loop while the context does not send a terminated signal

.. code-block:: console

    while(!context.terminated() && others conditions ...){
        // process

        // Don't forget to use LOG
    }

-   *Postprocess(ProcessingContext& context)* : post-process state of the node aka log info and close communication

-   *TestPrepare(ProcessingContext& context)* : use in case of integration test

-   *TestFinalize(ProcessingContext& context)* : use in case of integration test


Finally, don't forget to add your processor in the namespace by using

.. code-block:: console

    REGISTERPROCESSOR(ProcessorName)

Documentation :
---------------
The documentation of your processor will need to specify what it is doing, its inputs and outputs but also how to describe it in
the graph definition yaml file (available options ...etc.).

Documentation useful for the development
----------------------------------------

- graph system
- logging system
- build system

Development build
-----------------
While populating your extension in the falcon CMake, you can override the git location with the cache option:

.. code-block:: console

    FETCHCONTENT_BASE_DIR <local_path>

More information `here <https://cmake.org/cmake/help/git-stage/module/FetchContent.html>`_
