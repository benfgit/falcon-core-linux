.. _generic_client:

Generic control app
===================

Installation
************

The falcon-client ui is used to remote control of the falcon system.

Install in your python path :

.. code-block:: console

    conda install -c kloostermanlab falcon-clients 

The ui can be launched when the falcon program is already running.

.. code-block:: console

    simple_client


Graph display app
=================

In the same repository as installed the previous step, you have 4 python clients :

- Plot mua stats in live with the command `live_plot_mua_stats`
- Plot behavior in live with the command `live_plot_behavior`
- Plot decoding error in live with the command `live_plot_decoding_error`
- Plot ripple stats in live with the command `live_plot_ripple_stats`
