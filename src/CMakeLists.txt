# driver source files
SET(sources htc_vive_tracker.cpp)
# application header files
SET(headers htc_vive_tracker.h)
# locate the necessary dependencies
# add the necessary include directories
INCLUDE_DIRECTORIES(.)
# create the shared library
ADD_LIBRARY(htc_vive_tracker SHARED ${sources})
set(LOCALPATH "/usr/local/")

find_library(OPENVR_LIB openvr_api "${LOCALPATH}/lib/" NO_DEFAULT_PATH)
message("Found libopenvr_api: ${OPENVR_LIB}")
#include_directories(${OPENVR_INCLUDE_DIRS})
# link necessary libraries

TARGET_LINK_LIBRARIES (htc_vive_tracker ${OPENVR_LIB})
target_link_libraries(htc_vive_tracker ${CMAKE_DL_LIBS})
INSTALL(TARGETS htc_vive_tracker
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib/iridrivers
        ARCHIVE DESTINATION lib/iridrivers)
INSTALL(FILES ${headers} DESTINATION include/iridrivers)
INSTALL(FILES ../Findhtc_vive_tracker.cmake DESTINATION ${CMAKE_ROOT}/Modules/)
ADD_SUBDIRECTORY(examples)
