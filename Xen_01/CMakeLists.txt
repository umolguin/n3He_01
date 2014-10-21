cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
set(name XenExe)
project(XenExe)


option(WITH_GEANT4_UIVIS "Build Xen, Based in B1 and Optical with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#set(Geant4_DIR $ENV{G4INSTALL}/build)
find_package(Geant4 REQUIRED COMPONENTS gdml qt ui_all vis_all)
include(${Geant4_USE_FILE})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB my_project_sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
set(CMAKE_MODULE_PATH /home/umolguin/geant/init/cmake/Modules)

message(${CMAKE_MODULE_PATH})
find_package(ROOT REQUIRED)
include_directories(${ROOT_INCLUDE_DIR})
#ROOT_GENERATE_DICTIONARY(${CMAKE_CURRENT_BINARY_DIR}/HistoManager_Dict
#	${CMAKE_CURRENT_SOURCE_DIR}/include/HistoManager.hh
#	OPTIONS -p)
#set(my_project_sources ${my_project_sources} ${CMAKE_CURRENT_BINARY_DIR}/HistoManager_Dict.cxx)


#set(Geant4_DIR $ENV{G4INSTALL}/build)
find_package(Geant4 REQUIRED COMPONENTS gdml qt ui_all vis_all)
include(${Geant4_USE_FILE})

add_executable(${name} ${name}.cc ${my_project_sources})
target_link_libraries(${name} ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build B1. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(XENEXE_SCRIPTS
  XenExe.in
  #XenExe.out
  init.mac
  init_vis.mac
  run1.mac
  run2.mac
  vis.mac
  )

foreach(_script ${XENEXE_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()
