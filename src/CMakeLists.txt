# driver source files
SET(sources htc_vive_tracker.cpp)
# application header files
SET(headers htc_vive_tracker.h)

# create the shared library
ADD_LIBRARY(htc_vive_tracker SHARED ${sources})
target_include_directories(htc_vive_tracker
	PUBLIC
	.)

find_library(OPENVR_LIB openvr_api REQUIRED)

message(STATUS "Found libopenvr_api: ${OPENVR_LIB}")
# link necessary libraries

TARGET_LINK_LIBRARIES (htc_vive_tracker 
	PUBLIC 
	${OPENVR_LIB}
	${CMAKE_DL_LIBS}
	)
INSTALL(TARGETS htc_vive_tracker
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib/iridrivers
        ARCHIVE DESTINATION lib/iridrivers)
INSTALL(FILES ${headers} DESTINATION include/iridrivers)
INSTALL(FILES ../Findhtc_vive_tracker.cmake DESTINATION ${CMAKE_ROOT}/Modules/)
ADD_SUBDIRECTORY(examples)
