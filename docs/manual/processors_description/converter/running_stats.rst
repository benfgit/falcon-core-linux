RunningStats
============

Compute running statistics

Input ports
...........

:data:  *type*: :ref:`MultiChannelData` <double>, 1-256 slot

Output port
...........

:data:  *type*: :ref:`MultiChannelData` <double>, 1-256 slot

Options
.......

:integration_time *(double)*:
  time window for exponential smoothing

  *Default value*: 1.0

:outlier_protection *(bool)*:
  enable outlier protection. Outliers are values larger than a predefined z-score.
  The contribution of an outlier is reduced by an amount that depends on the magnitude of the outlier

  *Default value*: False

:outlier_zscore *(double)*:
  z-score that defines an outlier

  *Default value*: 6.0

:outlier_half_life *(double)*:
  the number of standard deviations above the outlier z-score at which the influence of the outlier is halved.

  *Default value*: 2.0
