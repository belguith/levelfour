######################################################################
# VTK Designer 2 - OsgCore component
# 
# Autogenerated using VNFComponentGen
######################################################################

include(../Components.pri)

MOC_DIR         = $${PROJECT_BASE}/tmp/OsgCore
OBJECTS_DIR     = $${PROJECT_BASE}/tmp/OsgCore
UI_HEADERS_DIR  = $${PROJECT_BASE}/tmp/OsgCore
RCC_DIR         = $${PROJECT_BASE}/tmp/OsgCore
UI_DIR          = $${PROJECT_BASE}/tmp/OsgCore

DEPENDPATH      += . \
                   Interfaces 

# Additional includes from the VTK system
INCLUDEPATH     += $${PROJECT_BASE}/Common \
                   Interfaces 

# Input
HEADERS += source/OsgCoreComponent.h \
           source/OsgSimpleViewVisNode.h \
           source/OsgCompositeViewVisNode.h \
           source/OsgLightVisNode.h \
	     source/OsgLightVisNodeIOData.h \
           source/OsgGroupVisNode.h \
	     source/OsgGroupVisNodeIOData.h \
           source/OsgGeodeVisNode.h \
           source/OsgBoxVisNode.h \
           source/OsgSphereVisNode.h \
	     source/QOSGWidget.h \
		 source/AdapterWidget.h \
	     source/QOSGGraphics.h \
	     source/OsgNodeVisNodeIOData.h \
           source/OsgCylinderVisNode.h \
	     source/OsgConeVisNode.h \
	     source/OsgDrawableVisNodeIOData.h \
	     source/OsgSwitchVisNode.h \
	     source/OsgPATVisNode.h \
	     source/OsgPATVisNodeIOData.h \
	     source/OsgModelVisNode.h \
	     source/OsgBillboardVisNode.h \
           source/OsgCameraVisNode.h \
           source/OsgImageVisNode.h \
           source/OsgCapsuleVisNode.h \
           source/OsgInfinitePlaneVisNode.h \
           source/OsgTexture2DVisNode.h \

				
SOURCES += source/OsgCoreComponent.cpp \
           source/OsgSimpleViewVisNode.cpp \
           source/OsgCompositeViewVisNode.cpp \
           source/OsgLightVisNode.cpp \
           source/OsgGroupVisNode.cpp \
			source/QOSGWidget.cpp \
			source/QOSGGraphics.cpp \
		 source/AdapterWidget.cpp \
			source/OsgGroupVisNodeIOData.cpp \
			source/OsgLightVisNodeIOData.cpp \
           source/OsgGeodeVisNode.cpp \
           source/OsgBoxVisNode.cpp \
           source/OsgSphereVisNode.cpp \
           source/OsgCylinderVisNode.cpp \
		   source/OsgConeVisNode.cpp \
			source/OsgNodeVisNodeIOData.cpp \
		   source/OsgDrawableVisNodeIOData.cpp \
		   source/OsgSwitchVisNode.cpp \
		   source/OsgPATVisNode.cpp \
		   source/OsgPATVisNodeIOData.cpp \
		   source/OsgModelVisNode.cpp \
           source/OsgBillboardVisNode.cpp \
           source/OsgCameraVisNode.cpp \
           source/OsgImageVisNode.cpp \
           source/OsgCapsuleVisNode.cpp \
           source/OsgInfinitePlaneVisNode.cpp \
           source/OsgTexture2DVisNode.cpp \

	     

LIBS			+= -losgd \
				-losgGAd \
				-losgViewerd \
				-losgDBd \
				-lQtGuid4 \
				-lQtCored4 \
				-lQtOpenGLd4 \
				-lopenGL32 \
				-losgEarthUtild \
				-lopenThreadsd \
				-l$${PROJECT_BASE}/lib/MainWindow \

# VNFComponentGen has generated an empty resource file. If you add
# new objects to the resource file, then you can uncomment the line
# below
RESOURCES += OsgCore.qrc

# Nodes in VTK Designer 2 have to implement one or more node interfaces
# Implementing all of them from scratch would be a tough task. So, we make
# use of ready made implementations and then subclass from them.
HEADERS   +=  $${PROJECT_BASE}/Common/CGenericVisNodeDesc.h \
              $${PROJECT_BASE}/Common/CGenericVisNodeBase.h 

SOURCES   +=  $${PROJECT_BASE}/Common/CGenericVisNodeDesc.cpp \
              $${PROJECT_BASE}/Common/CGenericVisNodeBase.cpp 

