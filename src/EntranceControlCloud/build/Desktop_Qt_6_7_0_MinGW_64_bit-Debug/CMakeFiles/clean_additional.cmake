# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\EntranceControlCloud_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\EntranceControlCloud_autogen.dir\\ParseCache.txt"
  "EntranceControlCloud_autogen"
  )
endif()
