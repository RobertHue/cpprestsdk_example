########################################
# Author:	Robert Huemmer
########################################
cmake_minimum_required (VERSION 3.8 FATAL_ERROR)

########################################
# Define the project name
########################################
set(PROJECT_NAME "CPP_Rest_SDK_Test")
set(EXECUTABLE_NAME "CPP_Rest_SDK_Test")
project(${PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)

########################################
# generate a message
########################################
message(STATUS "~~~ ${PROJECT_NAME} v${PROJECT_VERSION} ~~~")
message(STATUS "Target: ${CMAKE_SYSTEM_PROCESSOR} (${CMAKE_SYSTEM_NAME})")
message(STATUS "Build Type : ${CMAKE_BUILD_TYPE}")

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

########################################
# set cmake module path
########################################
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")


########################################
# include directories for the compiler to search for
########################################
include_directories("include" )
include_directories(${Boost_INCLUDE_DIRS})



########################################
# configure and find BOOST
########################################
set(Boost_USE_STATIC_LIBS        ON) 	# only find static libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.69.0 REQUIRED COMPONENTS system filesystem unit_test_framework serialization)

########################################
# configure and find cpprestsdk
########################################
find_package(cpprestsdk REQUIRED)
find_package(Sqlite3 REQUIRED)

########################################
# configure and find QT5
########################################
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)
# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)
#find_package(Qt5 REQUIRED COMPONENTS Widgets)


#set C++ standart
set (CMAKE_CXX_STANDARD 14)

#add source directory
add_subdirectory("src")
