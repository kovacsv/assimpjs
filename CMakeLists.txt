cmake_minimum_required (VERSION 3.6)

set_property (GLOBAL PROPERTY USE_FOLDERS ON)

set (CMAKE_SUPPRESS_REGENERATION 1)
set (CMAKE_CONFIGURATION_TYPES Debug;Release;RelWithDebInfo)

add_definitions (-DUNICODE -D_UNICODE)

project (AssimpJS)

function (SetCompilerOptions module)
	target_compile_options (${module} PUBLIC "$<$<CONFIG:Debug>:-DDEBUG>")
	if (WIN32)
		set (AdditionalWarnings /w44061 /w44062 /w44265 /w44266 /w44355 /w44596 /w44800)
		target_compile_options (${module} PUBLIC /W4 ${AdditionalWarnings})
	else ()
		target_compile_options (${module} PUBLIC -std=c++11 -Wall -Wextra)
	endif ()
endfunction ()

# Assimp

add_subdirectory (assimp)

# AssimpJS

set (AssimpJSSourcesFolder assimpjs/src)
file (GLOB
	AssimpJSSourceFiles
	${AssimpJSSourcesFolder}/*.hpp
	${AssimpJSSourcesFolder}/*.cpp
)
source_group ("Sources" FILES ${AssimpJSSourceFiles})
add_executable (AssimpJS ${AssimpJSSourceFiles})
SetCompilerOptions (AssimpJS)
target_link_libraries (AssimpJS assimp)
set_target_properties(AssimpJS PROPERTIES OUTPUT_NAME assimpjs)
set_target_properties (AssimpJS PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")

if (${EMSCRIPTEN})
	target_compile_options (AssimpJS PUBLIC "$<$<CONFIG:Debug>:-gsource-map>")

	target_link_options (AssimpJS PUBLIC -sMODULARIZE=1)
	target_link_options (AssimpJS PUBLIC -sEXPORT_NAME='assimpjs')
	target_link_options (AssimpJS PUBLIC -sALLOW_MEMORY_GROWTH=1 --no-heap-copy)
	target_link_options (AssimpJS PUBLIC -sNO_DISABLE_EXCEPTION_CATCHING)
	target_link_options (AssimpJS PUBLIC --bind)
	
	get_filename_component (AssimpJSTestFolderAbsolute "${CMAKE_CURRENT_LIST_DIR}/assimpjs/test" ABSOLUTE)
	add_custom_command (TARGET AssimpJS POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${AssimpJSTestFolderAbsolute} $<TARGET_FILE_DIR:AssimpJS>
	)
endif ()
