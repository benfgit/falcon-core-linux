ZMQSerializer
=============
Serializes data streams to cloud

Input ports
...........

:data *(IData, 1-256 slots)*:

Options
.......

:port *(unsigned int)*:

  *Default value* : 7777

:encoding *(string)*:
  Only two acceptable keyword: 'binary' or 'yaml'
  *Default value* : "binary"

:format *(string)*:
  Only tree acceptable keyword: 'full', 'nodata', 'compact' (see serializer.hpp for more informations on this mode)

  *Default value* : full

:interleaved *(bool)*:
  whether data streams from different input slots are interleaved

  *Default value* : False

