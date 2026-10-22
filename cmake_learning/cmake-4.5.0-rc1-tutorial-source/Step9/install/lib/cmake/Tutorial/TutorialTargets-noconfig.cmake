#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Tutorial::Tutorial" for configuration ""
set_property(TARGET Tutorial::Tutorial APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(Tutorial::Tutorial PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/bin/Tutorial"
  )

list(APPEND _cmake_import_check_targets Tutorial::Tutorial )
list(APPEND _cmake_import_check_files_for_Tutorial::Tutorial "${_IMPORT_PREFIX}/bin/Tutorial" )

# Import target "Tutorial::MathFunctions" for configuration ""
set_property(TARGET Tutorial::MathFunctions APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(Tutorial::MathFunctions PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libMathFunctions.a"
  )

list(APPEND _cmake_import_check_targets Tutorial::MathFunctions )
list(APPEND _cmake_import_check_files_for_Tutorial::MathFunctions "${_IMPORT_PREFIX}/lib/libMathFunctions.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
