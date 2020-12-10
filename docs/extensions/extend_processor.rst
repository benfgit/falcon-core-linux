Create new processor
====================

The new processor class needs to derive from the IProcessor class.

Two important inputs are :

- const YAML: Node &node : The description from the graph of the node with the different parameters related to the processor
- Context : There is a GlobalContext and a ProcessingContext structure available to be use in the class. (see context documentation)

Virtual methods from the IProcessor class are available to be override:

-   *Class constructor* : Add options object to the options processor engine.

    .. code-block:: console

        ExampleProcessor::ExampleProcessor(): IProcessor() {
            add_option("option 1", options1_, "This option can be modify in the graph file with the keyword: option 1");
            add_option("option 2", options2_, "This option can be modify in the graph file with the keyword: option 2");
        }


-   *Configure( const GlobalContext& context)* : The graph (yaml file) describe the node
    with some parameters specific to the processor. These options are loaded internally between the creation of the processor
    and the call of this method. It is the time to do additional configurations based on the options (log in, derive some variables
    from it ... etc.).

    .. code-block:: console

        ExampleProcessor::Configure( const GlobalContext& context) {
            if(options1_() > 3){
                LOG(INFO) << "The option 1 is under 3" ;
            }

            useful_var_ = options1_() - options2_();
        }

-   *CreatePorts()* : This part make use of the internal available methods from Iprocessor (see the API documentation)
    for creating input port (*create_input_port*), readable sharable state (*create_readable_shared_state*)
    and output port (*create_output_port*).


-   *CompleteStreamInfo()* set extra information for output datastream and parameters specific to the datatype, check additional conditions as
    for example same numbers of input / output slot if there are related

-   *Prepare(GlobalContext& context)* : prepare state of the node aka connecting server ... etc.

-   *Unprepare(GlobalContext& context)* : undo the prepare method

-   *Preprocess(ProcessingContext& context)* : pre-process state of the note aka clear states
    The Preprocess part is synchronized between processor. So, all processor will wait for that others finished this part.
    At the difference of prepare step, it is done in their own thread.

-   *Process(ProcessingContext& context)* : process state of the node : for loop while the context does not send a terminated signal

.. code-block:: console

    while(!context.terminated() && others conditions ...){
        // process

        // Don't forget to use LOG
    }

-   *Postprocess(ProcessingContext& context)* : post-process state of the node aka log info, clean up and close communication

-   *TestPrepare(ProcessingContext& context)* : use in case of integration test

-   *TestFinalize(ProcessingContext& context)* : use in case of integration test


Finally, don't forget to add your processor in the namespace by using

.. code-block:: console

    REGISTERPROCESSOR(ProcessorName)

Documentation :
---------------
The documentation of your processor will need to specify what it is doing, its inputs and outputs but also how to describe it in
the graph definition yaml file (available options ...etc.).

To do this "doc.yaml" need to be added next to the .cpp with these entrees:

.. code-block:: yaml

    Description: short description

    Long description: long description (e.g. explanation of algorithm)

    Input ports:
      - name: name
        type: MultiChannelType
        slots: # or [#, #]
        description: description

    Output ports: ... same as input ports ...

    Options:
      - &options1                               #this option is also used as a shared state
        name: name
        type: double
        default: ...
        description: ...

    Methods:
      - name: name
        arguments:
          - name: default value
          - ...
        returns: ...
        description: ...

    States:
      static:
        - name: name
          type: double
          initial value: ...
          shared: true/false
          external access: read or write or none
          description: ...

      producer:
        - name: name
          type: double
          initial value: ...
          cooperative: true/false
          external access: read or write or none
          description: ...

      broadcaster:
        - name: name
          type: double
          initial value: ...
          external access: read or write or none
          description: ...

      follower:
        - name: name
          type: double
          initial value: ...
          external access: read or write or none
          description: ...

        - options: *options1                      #when the shared state was originally an option,
          external access: read or write or none  #the structure change a little to reuse the yaml option spec

To correctly build the documentation, this file needs to be in yaml format.

Documentation useful for the development
----------------------------------------

- `logging system <../internals/logging_system.html>`_
- `graph system <../internals/graph_system.html>`_

Development build
-----------------

An extension cannot be build by itself. You will need to rebuild falcon-core while adding your extension local path
in the extensions.txt in dev mode. It will automatically build falcon-core as well as your extension.
