######################################################################
# VTK Designer 2 - VisSystemCanvas component
# 
# Standard visualization network editor + network manager component
######################################################################

include(../Components.pri)

MOC_DIR         = $${PROJECT_BASE}/tmp/VisSystemCanvas
OBJECTS_DIR     = $${PROJECT_BASE}/tmp/VisSystemCanvas
UI_HEADERS_DIR  = $${PROJECT_BASE}/tmp/VisSystemCanvas
RCC_DIR         = $${PROJECT_BASE}/tmp/VisSystemCanvas
UI_DIR          = $${PROJECT_BASE}/tmp/VisSystemCanvas

# Input
HEADERS += source/CVisNetworkConnection.h \
           source/CVisSystemCanvas.h \
           source/CVisSystemCanvasCmds.h \
           source/CVisSystemCanvasComponent.h \
           source/CVisSystemCanvasConnectionItem.h \
           source/CVisSystemCanvasNodeItem.h \
           source/CVisSystemCanvasNodeLayout.h \
           source/CVisSystemCanvasWidget.h \
           source/CVisSystemCanvasNote.h \
           source/CVisSystemCanvasThumbnailView.h

SOURCES += source/CVisNetworkConnection.cpp \
           source/CVisSystemCanvas.cpp \
           source/CVisSystemCanvasCmds.cpp \
           source/CVisSystemCanvasComponent.cpp \
           source/CVisSystemCanvasConnectionItem.cpp \
           source/CVisSystemCanvasNodeItem.cpp \
           source/CVisSystemCanvasNodeLayout.cpp \
           source/CVisSystemCanvasWidget.cpp \
           source/CVisSystemCanvasNote.cpp \
           source/CVisSystemCanvasThumbnailView.cpp 
           
RESOURCES += VisSystemCanvas.qrc
QT        += opengl svg

# Common stuff
HEADERS += $${PROJECT_BASE}/Common/CUndoRedo.h 
SOURCES += $${PROJECT_BASE}/Common/CUndoRedo.cpp 


