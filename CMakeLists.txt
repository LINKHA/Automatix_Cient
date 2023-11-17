cmake_minimum_required(VERSION 3.20)
project(automatix
    VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -g")
set(AUTO_ROOT_PATH ${CMAKE_SOURCE_DIR})

# -- includes ------------------------------------------------------------------
include(CMakeDependentOption)
include(CMakePackageConfigHelpers)
include(CheckCXXSourceCompiles)
include(CheckCXXSourceRuns)
include(GNUInstallDirs)
include(GenerateExportHeader)

# Macro to add files from a directory grouped to the solution file
macro(source_group_by_dir source_files)
    if(MSVC)
        set(sgbd_cur_dir ${CMAKE_CURRENT_SOURCE_DIR})
        foreach(sgbd_file ${${source_files}})
            string(REGEX REPLACE ${sgbd_cur_dir}/\(.*\) \\1 sgbd_fpath ${sgbd_file})
            string(REGEX REPLACE "\(.*\)/.*" \\1 sgbd_group_name ${sgbd_fpath})
            string(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)
            string(REPLACE "/" "\\" sgbd_group_name ${sgbd_group_name})
            if(sgbd_nogroup)
                set(sgbd_group_name "\\")
            endif(sgbd_nogroup)
            source_group(${sgbd_group_name} FILES ${sgbd_file})
        endforeach(sgbd_file)
    endif(MSVC)
endmacro(source_group_by_dir)

# -- CAF options with non-boolean values ---------------------------------------

set(CAF_CXX_VERSION 17 CACHE STRING "Set the C++ version to use for CAF")
set(CAF_LOG_LEVEL "QUIET" CACHE STRING "Set log verbosity of CAF components")
set(CAF_EXCLUDE_TESTS "" CACHE STRING "List of excluded test suites")
set(CAF_SANITIZERS "" CACHE STRING
    "Comma separated sanitizers, e.g., 'address,undefined'")
set(CAF_BUILD_INFO_FILE_PATH "" CACHE FILEPATH
  "Optional path for writing CMake and compiler version information")
  
# -- base target setup ---------------------------------------------------------

# This target propagates compiler flags, extra dependencies, etc. All other CAF
# targets pull this target in as a PRIVATE dependency. Users that embed CAF into
# their own CMake scaffold (e.g., via FetchContent) may pass this target in with
# some properties predefined in order to force compiler flags or dependencies.
if(NOT TARGET caf_internal)
  message(STATUS "Build CAF with C++${CAF_CXX_VERSION}")
  add_library(caf_internal INTERFACE)
  target_compile_features(caf_internal INTERFACE cxx_std_${CAF_CXX_VERSION})
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  target_compile_options(caf_internal INTERFACE -Wall -Wextra -pedantic
                         -ftemplate-depth=512 -ftemplate-backtrace-limit=0)
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(caf_internal INTERFACE -Wdocumentation)
  else()
    target_compile_options(caf_internal INTERFACE
                           -Wno-missing-field-initializers)
  endif()
elseif(MSVC)
  target_compile_options(caf_internal INTERFACE /EHsc)
endif()


# -- export internal target (may be useful for re-using compiler flags) --------

set_target_properties(caf_internal PROPERTIES EXPORT_NAME internal)

add_library(CAF::internal ALIAS caf_internal)

install(TARGETS caf_internal EXPORT CAFTargets)

# generates the implementation file for the enum that contains to_string,
# from_string and from_integer
function(caf_add_enum_type target enum_name)
  string(REPLACE "." "/" path "${enum_name}")
  set(hpp_file "${CMAKE_CURRENT_SOURCE_DIR}/caf/${path}.hpp")
  set(cpp_file "${CMAKE_CURRENT_BINARY_DIR}/src/${path}_strings.cpp")
  set(gen_file "${PROJECT_SOURCE_DIR}/cmake/caf-generate-enum-strings.cmake")
  add_custom_command(OUTPUT "${cpp_file}"
                     COMMAND ${CMAKE_COMMAND}
                       "-DINPUT_FILE=${hpp_file}"
                       "-DOUTPUT_FILE=${cpp_file}"
                       -P "${gen_file}"
                     DEPENDS "${hpp_file}" "${gen_file}")
  target_sources(${target} PRIVATE "${cpp_file}")
endfunction()

function(caf_export_and_install_lib component)
  add_library(CAF::${component} ALIAS libcaf_${component})
  string(TOUPPER "CAF_${component}_EXPORT" export_macro_name)
  target_include_directories(libcaf_${component} INTERFACE
                             $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                             $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
                             $<INSTALL_INTERFACE:include>)
  generate_export_header(
    libcaf_${component}
    EXPORT_MACRO_NAME ${export_macro_name}
    EXPORT_FILE_NAME "caf/detail/${component}_export.hpp")
  set_target_properties(libcaf_${component} PROPERTIES
                        EXPORT_NAME ${component}
                        SOVERSION ${CAF_VERSION}
                        VERSION ${CAF_LIB_VERSION}
                        OUTPUT_NAME caf_${component})
  install(TARGETS libcaf_${component}
          EXPORT CAFTargets
          ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT ${component}
          RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT ${component}
          LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT ${component})
  install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/caf"
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
          COMPONENT ${component}
          FILES_MATCHING PATTERN "*.hpp")
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/caf/detail/${component}_export.hpp"
          DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/caf/detail/")
endfunction()
# -- convenience function for automating our component setup -------------------

# Usage:
# caf_add_component(
#   foo
#   DEPENDENCIES
#     INTERFACE
#       ...
#     PUBLIC
#       ...
#     PRIVATE
#       ...
#   HEADERS
#     ...
#   SOURCES
#     ...
#   LEGACY_TEST_SOURCES
#     ...
#   LEGACY_TEST_SUITES
#     ...
# )
function(caf_add_component name)
  set(varargs DEPENDENCIES HEADERS SOURCES ENUM_TYPES LEGACY_TEST_SOURCES
              LEGACY_TEST_SUITES)
  cmake_parse_arguments(CAF_ADD_COMPONENT "" "" "${varargs}" ${ARGN})
  if(NOT CAF_ADD_COMPONENT_HEADERS)
    message(FATAL_ERROR "Cannot add CAF component without at least one header.")
  endif()
  if(NOT CAF_ADD_COMPONENT_SOURCES)
    message(FATAL_ERROR "Cannot add CAF component without at least one source.")
  endif()
  foreach(param DEPENDENCIES HEADERS SOURCES)
    if(NOT CAF_ADD_COMPONENT_${param})
      message(FATAL_ERROR "caf_add_component(): missing parameter ${param}")
    endif()
  endforeach()
  set(pub_lib_target "libcaf_${name}")
  set(obj_lib_target "libcaf_${name}_obj")
  set(targets ${pub_lib_target} ${obj_lib_target})
  add_library(${obj_lib_target} OBJECT ${CAF_ADD_COMPONENT_HEADERS})
  set_property(TARGET ${obj_lib_target} PROPERTY POSITION_INDEPENDENT_CODE ON)
  target_link_libraries(${obj_lib_target} ${CAF_ADD_COMPONENT_DEPENDENCIES})
  if(CAF_ENABLE_TESTING)
    add_executable("caf-${name}-test" ${CMAKE_CURRENT_SOURCE_DIR}/main.test.cpp
                   $<TARGET_OBJECTS:${obj_lib_target}>)
    target_compile_definitions(
      "caf-${name}-test" PRIVATE
      CAF_TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/tests/data")
    target_link_libraries("caf-${name}-test" PRIVATE libcaf_test
                          ${CAF_ADD_COMPONENT_DEPENDENCIES})
    target_include_directories("caf-${name}-test" PRIVATE
                               "${CMAKE_CURRENT_SOURCE_DIR}"
                               "${CMAKE_CURRENT_BINARY_DIR}")
  endif()
  foreach(source_file ${CAF_ADD_COMPONENT_SOURCES})
    get_filename_component(ext ${source_file} EXT)
    if(ext STREQUAL ".cpp")
      target_sources(${obj_lib_target} PRIVATE ${source_file})
    elseif(ext STREQUAL ".test.cpp" AND CAF_ENABLE_TESTING)
      get_filename_component(test_name ${source_file} NAME_WE)
      get_filename_component(test_path ${source_file} DIRECTORY)
      string(REPLACE "/" "." test_name "${test_path}/${test_name}")
      target_sources("caf-${name}-test" PRIVATE ${source_file})
      set_property(SOURCE ${source_file} PROPERTY COMPILE_DEFINITIONS
                   CAF_TEST_SUITE_NAME=${test_name})
      add_test(NAME ${test_name} COMMAND "caf-${name}-test" -s "^${test_name}$" -n -v5)
      set_tests_properties(${test_name} PROPERTIES TIMEOUT 300)
    endif()
  endforeach()
  if(NOT TARGET ${pub_lib_target})
    add_library(${pub_lib_target}
                "${PROJECT_SOURCE_DIR}/cmake/dummy.cpp"
                $<TARGET_OBJECTS:${obj_lib_target}>)
  else()
    target_sources(
      ${pub_lib_target}
      PRIVATE
        "${PROJECT_SOURCE_DIR}/cmake/dummy.cpp"
        $<TARGET_OBJECTS:${obj_lib_target}>)
  endif()
  if(CAF_ENABLE_TESTING AND CAF_ADD_COMPONENT_LEGACY_TEST_SOURCES)
    set(tst_bin_target "caf-${name}-legacy-test")
    list(APPEND targets ${tst_bin_target})
    add_executable(${tst_bin_target}
                   ${CAF_ADD_COMPONENT_LEGACY_TEST_SOURCES}
                   $<TARGET_OBJECTS:${obj_lib_target}>)
    target_link_libraries(${tst_bin_target} PRIVATE libcaf_test
                          ${CAF_ADD_COMPONENT_DEPENDENCIES})
    target_include_directories(${tst_bin_target} PRIVATE
                               "${CMAKE_CURRENT_SOURCE_DIR}/tests/legacy")
    if(CAF_ADD_COMPONENT_LEGACY_TEST_SUITES)
      caf_add_legacy_test_suites(${tst_bin_target} ${CAF_ADD_COMPONENT_LEGACY_TEST_SUITES})
    endif()
  endif()
  target_link_libraries(${pub_lib_target} ${CAF_ADD_COMPONENT_DEPENDENCIES})
  if(CAF_ADD_COMPONENT_ENUM_TYPES)
    foreach(enum_name ${CAF_ADD_COMPONENT_ENUM_TYPES})
      if(obj_lib_target)
        caf_add_enum_type(${obj_lib_target} ${enum_name})
      else()
        caf_add_enum_type(${pub_lib_target} ${enum_name})
      endif()
    endforeach()
  endif()
  foreach(target ${targets})
    target_compile_definitions(${target} PRIVATE "libcaf_${name}_EXPORTS")
    target_include_directories(${target} PRIVATE
                               "${CMAKE_CURRENT_SOURCE_DIR}"
                               "${CMAKE_CURRENT_BINARY_DIR}")
    if(BUILD_SHARED_LIBS)
      set_target_properties(${target} PROPERTIES
                            CXX_VISIBILITY_PRESET hidden
                            VISIBILITY_INLINES_HIDDEN ON)
    endif()
  endforeach()
  caf_export_and_install_lib(${name})
endfunction()


list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Find/)
# add include path
include_directories(automatix_realtime)
include_directories(thirdparty)
include_directories(thirdparty/ylt/thirdparty)

find_package(Threads REQUIRED)
link_libraries(Threads::Threads)

include(cmake/utils.cmake)
include(cmake/struct_pb.cmake)
include(cmake/build.cmake)
include(cmake/develop.cmake)
include(cmake/install.cmake)
# add project config, such as enable_ssl.
include(cmake/config.cmake)
# add project's source such as unit test, example & benchmark
include(cmake/subdir.cmake)



add_subdirectory (automatix_realtime)
add_subdirectory (example)
add_subdirectory (thirdparty)

