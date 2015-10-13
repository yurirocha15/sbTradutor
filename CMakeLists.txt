cmake_minimum_required(VERSION 2.8)

project(Tradutor)

#########################################################
# COMPILATION DIRECTIVES
#########################################################
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O2")
	set(warnings "-Wall -Wextra")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(warnings "/W4 /wd4512 /WX /EHsc")
    # Disabled Warnings:
    #   4512 "assignment operator could not be generated"
    #        This warning provides no useful information and will occur in
    #        well formed programs.
    #        <http://msdn.microsoft.com/en-us/library/hsyx7kbz.aspx>
endif()
if (NOT CONFIGURED_ONCE)
    set(CMAKE_CXX_FLAGS "${warnings}"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)
    set(CMAKE_C_FLAGS   "${warnings}"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)
endif()

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY 
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
      "Choose the type of build, options are: Debug Release
RelWithDebInfo MinSizeRel."
      FORCE)
endif(NOT CMAKE_BUILD_TYPE)

#########################################################
# FINDING BOOST
#########################################################
if (NOT DEFINED     BOOST_ROOT        AND
    NOT DEFINED ENV{BOOST_ROOT}       AND
    NOT DEFINED     BOOST_INCLUDEDIR  AND
    NOT DEFINED ENV{BOOST_INCLUDEDIR} AND
    NOT DEFINED     BOOST_LIBRARYDIR  AND
    NOT DEFINED ENV{BOOST_LIBRARYDIR})
    if (APPLE)
        set(BOOST_ROOT "../../../boost/boost_1_54_0/mac")
    elseif (WIN32)
        set(BOOST_INCLUDEDIR "C:/local/boost_1_55_0")
        set(BOOST_LIBRARYDIR "C:/local/boost_1_55_0/lib32-msvc-10.0")
    endif()
endif()
if (APPLE OR WIN32)
    set(Boost_USE_STATIC_LIBS TRUE)
endif()
#Add below all the used boost packages
find_package(Boost REQUIRED COMPONENTS program_options regex)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})


#########################################################
# INCLUDE DIRECTORIES
#########################################################
include_directories(
${CMAKE_CURRENT_SOURCE_DIR}/include 
)

#########################################################
# INCLUDE FILES
#########################################################
set(SOURCES 
	src/verboseLog.cpp
    src/defines.cpp
    src/preprocessing.cpp
    src/lexical.cpp
    src/fileLoader.cpp
		src/symbol.cpp
		src/parser.cpp
		src/token.cpp
		src/montador.cpp
	)

add_executable(${PROJECT_NAME} main.cpp ${SOURCES})

#########################################################
# LLINKING & STUFF
#########################################################
set(ALL_LIBS
	${Boost_LIBRARIES}
)

target_link_libraries(${PROJECT_NAME} ${ALL_LIBS})

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG_MODE)
endif (CMAKE_BUILD_TYPE MATCHES Debug)


set(CONFIGURED_ONCE TRUE CACHE INTERNAL
    "A flag showing that CMake has configured at least once.")


