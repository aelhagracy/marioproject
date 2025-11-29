# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Mario__autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Mario__autogen.dir\\ParseCache.txt"
  "Mario__autogen"
  )
endif()
