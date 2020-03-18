Frequent errors
===============

`ERR exception yaml-cpp: error at line 0, column 0: operator[] call on a scalar`

--> check that all processor options are set with `:` and not with `=`

.. code-block:: bash

    ERROR	Error handling command: buildfile Error: yaml-cpp: error at line 0, column 0: bad conversion
    ERR exception yaml-cpp: error at line 0, column 0: bad conversion


--> check that your using the correct option name in the config file and make sure all necessary options are listed in the config file

.. code-block:: bash

    ERR exception No storage context "repo" exists.
    ERROR	Error handling command: buildfile Error: No storage context "repo" exists.


--> Check that you have a custom "repo" path in your config file :

.. code-block:: console

    server_side_storage:
      environment: path/output_data
      resources: path/input_data
      custom:
        repo: Path/To/Repo/falcon


