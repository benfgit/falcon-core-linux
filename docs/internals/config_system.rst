Configuration system
====================

Configuration base class in utilities library.

FalconConfiguration class.

Configuration YAML file.

.. code-block:: console

    gedit /home/user/.falcon/config.yaml`  => modify the username : example fklab on the lab computer / Also you can run the alias falcon_config


.. code-block:: console

    debug:

    - enabled: false

    testing:

    - enabled: false

    graph:

    - autostart: false    => turn it to true if you want your graph start immediatly when launching falcon
    - file: " "           => specify the path to your config file describing the graph ( see the graph bank here : tests/graphs/*")

    network:

    - port: 5555

    logging:

    - path: ./	      => where to save the logging file

    screen:

    - enabled: true

    cloud:

    - enabled: true
    - port: 5556

    server_side_storage:

    - environment: ./              => where to save the different outputs if not specified in the graph description
    - resources: $HOME/.falcon     => where the "input" are if not specified in the graph description
    - custom:                                     => add the path of your repository
    - repo: Path/To/Repo/falcon

