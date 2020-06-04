.. _ripple_graph:

Ripple detection graph
======================

.. code-block::

    processors:
        source:
            class: NlxReader
            advanced:
                threadpriority: 100
                threadcore: 0
            options:
                address: 192.168.3.101
                port: 26090
                batch_size: 3
                update_interval: 10
                channelmap:
                    hp1: [13,20]
                    cx1: [9]
    ​
        ripple_filter(1-2):
            class: MultiChannelFilter
            options:
                filter:
                    file: //:resource/iir_ripple_low_delay.filter
    ​
        HIPPOCAMPUS_detector:
            class: RippleDetector
            options:
                threshold_dev: 14
                smooth_time: 7 # in seconds
                detection_lockout_time_ms: 50 #ms
                stream_events: true
                stream_statistics: true
                statistics_buffer_size: 1.0 # sec
                statistics_downsample_factor: 4
                use_power: true
    ​
        CORTEX_detector:
            class: RippleDetector
            options:
                threshold_dev: 12
                smooth_time: 8 # in seconds
                detection_lockout_time_ms: 40 #ms
                stream_events: true
                stream_statistics: false
                statistics_buffer_size: 1.0 # sec
                statistics_downsample_factor: 4
                use_power: true
    ​
        eventfilter:
            class: EventFilter
            options:
                target_event: ripple
                blockout_time_ms: 40
                synch_time_ms: 1.5
                time_in_ms: 4 # below 3.5, asynch can occur
                detection_criterion: "any" # 'any', 'all' or number
                discard_warnings: false
    ​
        networksink:
            class: ZMQSerializer
            options:
                encoding: binary
                format: compact
    ​
        eventsink:
            class: EventSink
            options:
                target_event: ripple
    ​
        datasink_ev:
            class: FileSerializer
            options:
                encoding: yaml
                format: full
    ​
        datasink_ripplestats:
            class: FileSerializer
            options:
                encoding: binary
                format: compact

        ttl_output:
            class: SerialOutput
            options:
                enabled: true
                target_event: ripple
                message: "d" # d for delay stim, o for ontime
                lockout_period_ms: 250
    ​
    connections:
        - source.hp1=ripple_filter1.data
        - source.cx1=ripple_filter2.data
        - ripple_filter1.data=HIPPOCAMPUS_detector.data
        - ripple_filter2.data=CORTEX_detector.data
        - HIPPOCAMPUS_detector.events=eventfilter.events
        - CORTEX_detector.events=eventfilter.blocking_events
        - HIPPOCAMPUS_detector.statistics.0=networksink.data
        - eventfilter.events.0=ttl_output.events
        - eventfilter.events.0=eventsink.events
        - eventfilter.events.0=datasink_ev.data
        - HIPPOCAMPUS_detector.statistics.0=datasink_ripplestats.data