
########################################
########################################
########################################
if( Boost_FOUND)
	message(STATUS "boost-include-dirs : ${Boost_INCLUDE_DIRS}")
	message(STATUS "Boost_LIBRARIES : ${Boost_LIBRARIES}")
	
	file(GLOB_RECURSE dependencies
		"../include/*.h"
		"*.cpp"
	)
	
	include_directories(${Boost_INCLUDE_DIRS} ${SFML_INCLUDE_DIR})	
	add_executable( ${EXECUTABLE_NAME} ${dependencies}	)	
	target_link_libraries(${EXECUTABLE_NAME} ${Boost_LIBRARIES})
endif()
########################################
########################################
########################################