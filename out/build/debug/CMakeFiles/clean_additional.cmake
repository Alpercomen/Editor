# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "Source\\Application\\CMakeFiles\\Editor_autogen.dir\\AutogenUsed.txt"
  "Source\\Application\\CMakeFiles\\Editor_autogen.dir\\ParseCache.txt"
  "Source\\Application\\Editor_autogen"
  "_deps\\fmt-build\\CMakeFiles\\fmt_autogen.dir\\AutogenUsed.txt"
  "_deps\\fmt-build\\CMakeFiles\\fmt_autogen.dir\\ParseCache.txt"
  "_deps\\fmt-build\\fmt_autogen"
  "_deps\\spdlog-build\\CMakeFiles\\spdlog_autogen.dir\\AutogenUsed.txt"
  "_deps\\spdlog-build\\CMakeFiles\\spdlog_autogen.dir\\ParseCache.txt"
  "_deps\\spdlog-build\\spdlog_autogen"
  )
endif()
