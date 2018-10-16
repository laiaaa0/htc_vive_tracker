#edit the following line to add the librarie's header files
FIND_PATH(htc_vive_tracker_INCLUDE_DIR htc_vive_tracker.h /usr/include/iridrivers /usr/local/include/iridrivers)

FIND_LIBRARY(htc_vive_tracker_LIBRARY
    NAMES htc_vive_tracker
    PATHS /usr/lib /usr/local/lib /usr/local/lib/iridrivers) 

IF (htc_vive_tracker_INCLUDE_DIR AND htc_vive_tracker_LIBRARY)
   SET(htc_vive_tracker_FOUND TRUE)
ENDIF (htc_vive_tracker_INCLUDE_DIR AND htc_vive_tracker_LIBRARY)

IF (htc_vive_tracker_FOUND)
   IF (NOT htc_vive_tracker_FIND_QUIETLY)
      MESSAGE(STATUS "Found htc_vive_tracker: ${htc_vive_tracker_LIBRARY}")
   ENDIF (NOT htc_vive_tracker_FIND_QUIETLY)
ELSE (htc_vive_tracker_FOUND)
   IF (htc_vive_tracker_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find htc_vive_tracker")
   ENDIF (htc_vive_tracker_FIND_REQUIRED)
ENDIF (htc_vive_tracker_FOUND)

