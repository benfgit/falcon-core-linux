Rebuffer
========

Rebuffer and downsample multiple MultiChannelData streams. No anti-aliasing filter is applied before downsampling.

Input ports
...........

:data:  *type*: :ref:`MultiChannelData` <double>, 1-256 slot

Output port
...........

:data:  *type*: :ref:`MultiChannelData` <double>, 1-256 slot

Options
.......

:downsample_factor:
  downsample factor

  *Default value*: 1

:buffer_unit:
  samples or seconds

  *Default value*: samples

:buffer_size:
  output buffer size in samples or time

  *Default value*: 10 samples or equivalent in second based on the downsample factor depending of the buffer unit.
