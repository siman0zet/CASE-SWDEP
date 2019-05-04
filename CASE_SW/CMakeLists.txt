project(CASE_SW)
cmake_minimum_required(VERSION 2.8.7)

message("*** CASE_SWDEP ***")

set(CMAKE_AUTOMOC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Укажите путь к Qt5Widgets_cmake, если не обнаружится Qt5Widgets
#set (Qt5Widgets_DIR /home/syler/Qt5.4.1/5.4/gcc/lib/cmake/Qt5Widgets)

#set (Qt5Widgets_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(Qt5Widgets QUIET)

if(NOT CMAKE_CXX_COMPILER)
message(SEND_ERROR "Failed to find C++")
endif()

if(NOT CMAKE_C_COMPILER)
message(SEND_ERROR "Failed to find C")
endif()

if(NOT Qt5Widgets_FOUND)
    message(SEND_ERROR "Failed to find Qt5Widgets. Try to set 'Qt5Widgets_DIR' variable.
	In order to do this use: 
	cmake -D Qt5Widgets_DIR='<path to Qt5WidgetsConfig.cmake>' <path to CASE_SW cmake>")
    return()
else()
    include_directories(${Qt5Widgets_INCLUDE_DIRS} )
endif()

add_definitions(${Qt5Widgets_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

set (CASE_SW_HDRS CConnection.h CConnectionView.h CConnectionViewWidget.h CDataModel.h CDataModelException.h CDetailedEntityView.h CDetailedEntityWidget.h CEntity.h CEntityView.h CForeignRow.h CLangWidget.h CLoadAndSaveWidget.h CPluginInterface.h CRow.h CScriptGenerator.h CScrollArea.h CViewModel.h MainWindow.h ui_MainWindow.h CUniquesEditorWidget.h)

set (CASE_SW_SRCS main.cpp CConnection.cpp CConnectionView.cpp CConnectionViewWidget.cpp CDataModel.cpp CDataModelException.cpp CDetailedEntityView.cpp CDetailedEntityWidget.cpp CEntity.cpp CEntityView.cpp CForeignRow.cpp CLangWidget.cpp CLoadAndSaveWidget.cpp CRow.cpp CScriptGenerator.cpp CScrollArea.cpp CViewModel.cpp MainWindow.cpp CUniquesEditorWidget.cpp)

set (CASE_SW_UIS MainWindow.ui)

set (CASE_SW_RSCS langs img MainWindow.ui)

install ( DIRECTORY langs DESTINATION /${CMAKE_INSTALL_PREFIX}/)

install ( DIRECTORY img DESTINATION /${CMAKE_INSTALL_PREFIX}/)

#install_files ( /${CMAKE_INSTALL_PREFIX}/langs FILES English.qm Russian.qm)

#install_files ( /${CMAKE_INSTALL_PREFIX}/img FILES img/agregate.bmp img/agregate_act.bmp img/create_entity.bmp img/create_entity_act.bmp img/curs.bmp img/curs_act.bmp img/many_many.bmp img/many_many_act.bmp img/one_many.bmp img/one_many_act.bmp img/one_one.bmp img/one_one_act.bmp img/remove_entity.bmp img/remove_entity_act.bmp)

install ( DIRECTORY libs DESTINATION /${CMAKE_INSTALL_PREFIX}/)

add_executable (CASE_SW ${CASE_SW_SRCS} ${CASE_SW_UIS} ${CASE_SW_RSCS} ${CASE_SW_MOCS} ${CASE_SW_HDRS} )
target_link_libraries ( CASE_SW ${Qt5Widgets_LIBRARIES})
