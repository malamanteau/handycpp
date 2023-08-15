cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(
	handycpp 
	VERSION 1.0.0.0 
	LANGUAGES C CXX)

# C++ VERSION: When generating build configurations, use the compiler specific flag to enable C++17
	set(CMAKE_CXX_STANDARD 17)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)

# PRIVATE FILES
	set(handycpp_SOURCES 
		src/HandyConsole.cpp
		src/HandyGuid.cpp
		src/HandyMMFile.cpp
		src/HandyLoader.cpp
		src/HandySystemInfo.cpp
		src/HandyThreadUtils.cpp
		src/stb_image.cpp
	)

# HEADERS
	set(handycpp_PRIVATE_HEADERS 
		src/cfgpath.h
		src/Common.hpp
	)

	set(handycpp_PUBLIC_HEADERS
		"Handy.hpp"
		"HandyBase.hpp"
		"HandyCompat.hpp"
		"HandyConsole.hpp"
		"HandyDeque.hpp"
		"HandyEncoding.hpp"
		"HandyExtended.hpp"
		"HandyFile.hpp"
		"HandyGuid.hpp"
		"HandyHash.hpp"
		"HandyLoader.hpp"
		"HandyMath.hpp"
		"HandyMemory.hpp"
		"HandyMMFile.hpp"
		"HandyPIMPL.hpp"
		"HandyRange.hpp"
		"HandyResult.hpp"
		"HandySerDe.hpp"
		"HandyString.hpp"
		"HandySystemInfo.hpp"
		"HandyThreadUtils.hpp"
		"HandyTryGet.hpp"
		"HandyUtils.hpp"
		"Extended/HandyArgs.hpp"
		"Extended/HandyCalamity.hpp"
		"Extended/HandyPacket.hpp"
		"Extended/HandySHA1.hpp"
		"Extended/HandySmallVector.hpp"
		"Extended/stb_image.h"
		"Extended/GSL/include/gsl/gsl_assert"
		"Extended/GSL/include/gsl/gsl_byte"
		"Extended/GSL/include/gsl/gsl_util"
		"Extended/GSL/include/gsl/span"
		"Math/Extended/sse_mathfun.h"
		"Math/Core.hpp"
		"Math/Extended.hpp"
		"Math/Core/Base.hpp"
		"Math/Core/BitContinuum.hpp"
		"Math/Core/Vector4.hpp"
		"Math/Core/Vector3.hpp"
		"Math/Core/Vector2.hpp"
		"Math/Core/Vector4i.hpp"
		"Math/Core/Vector3i.hpp"
		"Math/Core/Vector2i.hpp"
		"Math/Core/Vector4d.hpp"
		"Math/Core/Vector3d.hpp"
		"Math/Core/Vector2d.hpp"
		"Math/Core/Matrix3.hpp"
		"Math/Core/Matrix4.hpp"
		"Math/Core/Popcount.hpp"
		"Math/Core/RGB888.hpp"
		"Math/Core/RGBA8888.hpp"
		"Math/Core/Conversions.hpp"
		"Math/Extended/AABB2.hpp"
		"Math/Extended/AABB2d.hpp"
		"Math/Extended/AABB2i.hpp"
		"Math/Extended/AABB3.hpp"
		"Math/Extended/Functions.hpp"
		"Math/Extended/Maps.hpp"
		"Math/Extended/IProjection.hpp"
		"Math/Extended/Line2.hpp"
		"Math/Extended/LineClipping.hpp"
		"Math/Extended/LineSimplification.hpp"
		"Math/Extended/OrthoFrame3.hpp"
		"Math/Extended/OrthoProjection.hpp"
		"Math/Extended/PerspectiveProjection.hpp"
		"Math/Extended/Xform3.hpp"
	)

# ============================= NEW W/ CONFIG EXPORT ===================================

	add_library(${PROJECT_NAME} STATIC ${handycpp_SOURCES})
	add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

	target_include_directories(${PROJECT_NAME} PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<INSTALL_INTERFACE:include>)
	
	include(../../cmake/DEFINITIONS.cmake)
	target_compile_definitions(${PROJECT_NAME} PUBLIC ${GLOBALISH_COMPILE_DEFINITIONS})

	if(MSVC)
		target_compile_options(${PROJECT_NAME} PUBLIC /MP /bigobj)

		set_target_properties (${PROJECT_NAME} PROPERTIES COMPILE_PDB_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_INSTALL_PREFIX}/lib)
		set_target_properties (${PROJECT_NAME} PROPERTIES COMPILE_PDB_OUTPUT_DIRECTORY_RELEASE ${CMAKE_INSTALL_PREFIX}/lib)

		target_compile_options(${PROJECT_NAME} PUBLIC $<$<CXX_COMPILER_ID:MSVC>:/Zi> $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-g>)
	endif()

	install(TARGETS ${PROJECT_NAME}
		EXPORT "${PROJECT_NAME}-export"
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
	)

	install(EXPORT "${PROJECT_NAME}-export"
		FILE
		${PROJECT_NAME}Config.cmake
		DESTINATION
		lib/cmake/${PROJECT_NAME}
	)

	# Commented one doesn't keep directory structure in include/ directory.
	# install(FILES ${handycpp_PUBLIC_HEADERS} DESTINATION "include/${PROJECT_NAME}")
	foreach(file ${handycpp_PUBLIC_HEADERS})
		get_filename_component(dir ${file} DIRECTORY)
		install(FILES ${file}  PERMISSIONS OWNER_READ GROUP_READ WORLD_READ DESTINATION  DESTINATION include/${PROJECT_NAME}/${dir})
	endforeach()

	include(CMakePackageConfigHelpers)
	write_basic_package_version_file(
			${CMAKE_BINARY_SOURCE_DIR}/cmake/${PROJECT_NAME}ConfigVersion.cmake
			VERSION ${PROJECT_VERSION}
			COMPATIBILITY SameMajorVersion)

	install(FILES
		${CMAKE_BINARY_SOURCE_DIR}/cmake/${PROJECT_NAME}ConfigVersion.cmake
		DESTINATION "lib/cmake/${PROJECT_NAME}")
