FileSerializer
==============

Serializes data streams to file

Input ports
...........

:data *(IData, 1-256 slots)*:

Options
.......

:path *(string)*:
  server-side path

  *Default value* : "run://"

:encoding *(string)*:
  Only two acceptable keyword: 'binary' or 'yaml'

  *Default value* : "binary"

:format *(string)*:
  Only tree acceptable keyword: 'full', 'nodata', 'compact' (see serializer.hpp for more informations on this mode)

  *Default value* : full

:overwrite *(bool)*:
  overwrite existing file

  *Default value* : False

:throttle *(bool)*:
  throttle saving if we can't keep up

  *Default value* : False

:throttle_threshold *(double)*:
  upstream ringbuffer fill fraction (between 0-1) at which throttling takes effect

  *Default value* : 0.3

:throttle_smooth *(double)*:
  smooth level of throttle level (between 0-1)

  *Default value* : 0.5