find_package(GDCM PATHS ${GDCM_DIR} REQUIRED)

list(APPEND ALL_INCLUDE_DIRECTORIES ${GDCM_INCLUDE_DIRS})
list(APPEND ALL_LIBRARIES ${GDCM_LIBRARIES})
list(APPEND ALL_LIBRARY_DIRS ${GDCM_LIBRARY_DIRS})
