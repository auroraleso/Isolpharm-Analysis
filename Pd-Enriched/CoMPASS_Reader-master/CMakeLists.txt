#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(Analysis)

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

#----------------------------------------------------------------------------
#--- Copying Macro Directory to the build directory
set(
  Analysis
  macro/gain.c
  macro/read_spectra.c
)

foreach(_script ${Analysis})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
#--- Locate the ROOT package and defines a number of variables
#--- (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS RIO Net)

#----------------------------------------------------------------------------
#--- Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

include_directories(${PROJECT_SOURCE_DIR}/include)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(Analysis analysis.cc ${sources} ${headers})
target_link_libraries(Analysis ${ROOT_LIBRARIES} -lSpectrum)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS Analysis DESTINATION bin)
