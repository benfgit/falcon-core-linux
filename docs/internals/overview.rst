Code organization and style guide
=================================

Falcon core is containing the core code to create and run the graph, as well as the logging system. 
The falcon folder is containing core code to create and run the graph. 

The lib folder is serving as an interface between external processors and falcon:

- cmdline
- disruptor
- factory
- logging
- options
- utilities

All processors, datatype and lib specific to one set of processors needs to be packaged in a separate extension repository. 

Style guide
===========

When coding with falcon tool, thanks to enforce the cpplint coding style. 
