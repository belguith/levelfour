######################################################################
# VTK Designer 2 - Viewport component
# 
# Autogenerated using VNFComponentGen
######################################################################

include(../Components.pri)

MOC_DIR         = $${PROJECT_BASE}/tmp/Viewport
OBJECTS_DIR     = $${PROJECT_BASE}/tmp/Viewport
UI_HEADERS_DIR  = $${PROJECT_BASE}/tmp/Viewport
RCC_DIR         = $${PROJECT_BASE}/tmp/Viewport
UI_DIR          = $${PROJECT_BASE}/tmp/Viewport

# Additional includes from the VTK system
INCLUDEPATH     += $${PROJECT_BASE}/Common

# Input
HEADERS += source/ViewportComponent.h \
           source/TableViewVisNode.h \
           source/TreeViewVisNode.h \
           source/ListViewVisNode.h \
 

SOURCES += source/ViewportComponent.cpp \
           source/TableViewVisNode.cpp \
           source/TreeViewVisNode.cpp \
           source/ListViewVisNode.cpp \


# VNFComponentGen has generated an empty resource file. If you add
# new objects to the resource file, then you can uncomment the line
# below
RESOURCES += ViewportComponent.qrc

# Nodes in VTK Designer 2 have to implement one or more node interfaces
# Implementing all of them from scratch would be a tough task. So, we make
# use of ready made implementations and then subclass from them.
HEADERS   +=  $${PROJECT_BASE}/Common/CGenericVisNodeDesc.h \
              $${PROJECT_BASE}/Common/CGenericVisNodeBase.h 

SOURCES   +=  $${PROJECT_BASE}/Common/CGenericVisNodeDesc.cpp \
              $${PROJECT_BASE}/Common/CGenericVisNodeBase.cpp 

