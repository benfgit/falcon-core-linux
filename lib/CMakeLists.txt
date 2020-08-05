add_subdirectory( utilities )
add_subdirectory( options )

# disruptor library
add_library( disruptor disruptor/claim_strategy.cpp disruptor/wait_strategy.cpp disruptor/sequence.cpp)


