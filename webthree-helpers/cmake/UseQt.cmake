function(ele_apply TARGET REQUIRED SUBMODULE)

set(ELE_QT_VERSION 5.4)

if (${SUBMODULE} STREQUAL "Core")
	find_package(Qt5Core ${ELE_QT_VERSION} ${REQUIRED})
	if (NOT Qt5Core_FOUND)
		return()
	endif()

	set(Qt5Core_VERSION_MAJOR ${Qt5Core_VERSION_MAJOR} PARENT_SCOPE)
	set_target_properties(${EXECUTABLE} PROPERTIES AUTOMOC ON)

	if (APPLE)
		find_program(MACDEPLOYQT_APP macdeployqt)
		find_program(ELE_APP_DMG appdmg)
	endif()

	# we need to find path to windeployqt on windows
	if (WIN32)
		find_program(WINDEPLOYQT_APP windeployqt)
	endif()

	if (("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang") AND NOT (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "3.6") AND NOT APPLE)
		# Supress warnings for qt headers for clang+ccache
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-inconsistent-missing-override")
	endif ()
	target_link_libraries(${TARGET} Qt5::Core)
	if (APPLE AND (NOT "${Qt5Core_VERSION_STRING}" VERSION_LESS "5.5"))
		find_package(Qt5DBus ${ELE_QT_VERSION} ${REQUIRED})
		target_link_libraries(${EXECUTABLE} Qt5::DBus)
	endif()
endif()

if (${SUBMODULE} STREQUAL "Widgets")
	ele_use(${TARGET} ${REQUIRED} Qt::Core)
	find_package(Qt5Widgets ${ELE_QT_VERSION} ${REQUIRED})
	if (NOT Qt5Widgets_FOUND)
		return()
	endif()
	set(Qt5Core_VERSION_MAJOR ${Qt5Widgets_VERSION_MAJOR} PARENT_SCOPE)
	set_target_properties(${EXECUTABLE} PROPERTIES AUTOUIC ON)
	target_link_libraries(${TARGET} Qt5::Widgets)
	if (APPLE AND (NOT "${Qt5Core_VERSION_STRING}" VERSION_LESS "5.5"))
		find_package(Qt5PrintSupport ${ELE_QT_VERSION} ${REQUIRED})
		target_link_libraries(${EXECUTABLE} Qt5::PrintSupport)
	endif()
endif()

if (${SUBMODULE} STREQUAL "Network")
	ele_use(${TARGET} ${REQUIRED} Qt::Core)
	find_package(Qt5Network ${ELE_QT_VERSION} ${REQUIRED})
	if (NOT Qt5Network_FOUND)
		return()
	endif()
	target_link_libraries(${TARGET} Qt5::Network)
endif()

if (${SUBMODULE} STREQUAL "WebEngine")
	ele_use(${TARGET} ${REQUIRED} Qt::Core)
	find_package(Qt5WebEngine ${ELE_QT_VERSION} ${REQUIRED})
	if (NOT Qt5WebEngine_FOUND)
		return()
	endif()
	target_link_libraries(${TARGET} Qt5::WebEngine)
	if (APPLE AND (NOT "${Qt5Core_VERSION_STRING}" VERSION_LESS "5.5"))
		find_package(Qt5WebEngineCore ${ELE_QT_VERSION} ${REQUIRED})
		target_link_libraries(${EXECUTABLE} Qt5::WebEngineCore)
	endif()
endif()

if (${SUBMODULE} STREQUAL "WebEngineWidgets")
	ele_use(${TARGET} ${REQUIRED} Qt::WebEngine)
	ele_use(${TARGET} ${REQUIRED} Qt::Widgets)
	find_package(Qt5WebEngineWidgets ${ELE_QT_VERSION} ${REQUIRED})
	if (NOT Qt5WebEngineWidgets_FOUND)
		return()
	endif()
	target_link_libraries(${TARGET} Qt5::WebEngineWidgets)
endif()

endfunction()
