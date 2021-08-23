cmake_minimum_required (VERSION 3.6)

set_property (GLOBAL PROPERTY USE_FOLDERS ON)

set (CMAKE_SUPPRESS_REGENERATION 1)
set (CMAKE_CONFIGURATION_TYPES Debug;Release;RelWithDebInfo;MinSizeRel)

add_definitions (-DUNICODE -D_UNICODE)

project (AssimpJS)

function (SetCompilerOptions module)
	target_compile_options (${module} PUBLIC "$<$<CONFIG:Debug>:-DDEBUG>")
	if (WIN32)
		set (AdditionalWarnings /w44061 /w44062 /w44265 /w44266 /w44355 /w44596 /w44800)
		target_compile_options (${module} PUBLIC /W3 ${AdditionalWarnings})
	else ()
		target_compile_options (${module} PUBLIC -std=c++11)
	endif ()
endfunction ()

# Assimp

set (BUILD_SHARED_LIBS OFF CACHE BOOL "")
set (ASSIMP_BUILD_TESTS OFF CACHE BOOL "")
set (ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "")
set (ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "")
set (ASSIMP_BUILD_ASSJSON_EXPORTER ON CACHE BOOL "")

add_subdirectory (assimp)
if (${EMSCRIPTEN})
	target_compile_options (assimp PUBLIC -fexceptions)
	target_link_options (assimp PUBLIC -sNO_DISABLE_EXCEPTION_CATCHING)	
endif ()

# AssimpJS

set (AssimpJSSourcesFolder assimpjs/src)
file (GLOB
	AssimpJSSourceFiles
	${AssimpJSSourcesFolder}/*.hpp
	${AssimpJSSourcesFolder}/*.cpp
)
source_group ("Sources" FILES ${AssimpJSSourceFiles})

if (${EMSCRIPTEN})
	add_executable (AssimpJS ${AssimpJSSourceFiles})
	target_compile_options (AssimpJS PUBLIC "$<$<CONFIG:Debug>:-gsource-map>")
	target_compile_options (AssimpJS PUBLIC -fexceptions)

	target_link_options (AssimpJS PUBLIC -sMODULARIZE=1)
	target_link_options (AssimpJS PUBLIC -sEXPORT_NAME='assimpjs')
	target_link_options (AssimpJS PUBLIC -sALLOW_MEMORY_GROWTH=1 --no-heap-copy)
	target_link_options (AssimpJS PUBLIC -sNO_DISABLE_EXCEPTION_CATCHING)
	
	target_link_options (AssimpJS PUBLIC --bind)
	
	get_filename_component (AssimpJSExamplesFolderAbsolute "${CMAKE_CURRENT_LIST_DIR}/examples" ABSOLUTE)
	add_custom_command (TARGET AssimpJS POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${AssimpJSExamplesFolderAbsolute} $<TARGET_FILE_DIR:AssimpJS>
	)
else ()
	add_library (AssimpJS ${AssimpJSSourceFiles})
endif ()

SetCompilerOptions (AssimpJS)
target_link_libraries (AssimpJS assimp)
set_target_properties(AssimpJS PROPERTIES OUTPUT_NAME assimpjs)
set_target_properties (AssimpJS PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")

# AssimpJSTest

get_filename_component (AssimpJSTestFilesFolderAbsolute "${CMAKE_CURRENT_LIST_DIR}/assimpjs/testfiles" ABSOLUTE)
add_custom_command (TARGET AssimpJS POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${AssimpJSTestFilesFolderAbsolute} $<TARGET_FILE_DIR:AssimpJS>/testfiles
)

if (${EMSCRIPTEN})
else ()
	set (AssimpJSExampleSourcesFolder assimpjs/example)
	file (GLOB
		AssimpJSExampleSourceFiles
		${AssimpJSExampleSourcesFolder}/*.hpp
		${AssimpJSExampleSourcesFolder}/*.cpp
	)
	source_group ("Sources" FILES ${AssimpJSExampleSourceFiles})
	add_executable (AssimpJSExample ${AssimpJSExampleSourceFiles})
	SetCompilerOptions (AssimpJSExample)
	target_include_directories (AssimpJSExample PUBLIC ${AssimpJSSourcesFolder} ${TinyCSGEmscriptenSourcesFolder})
	target_link_libraries (AssimpJSExample AssimpJS)
	set_target_properties (AssimpJSExample PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
endif ()

