//C++ header - fIVE|Analyse - Copyright (C) 2002-2003 Michael Gronager, UNI-C
//Distributed under the terms of the GNU Library General Public License (LGPL)
//as published by the Free Software Foundation.

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/ref_ptr>

#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkCellArray.h>
// vtkActorToOSG - translates vtkActor to osg::Geode. If geode is NULL, new one
//   will be created. Optional verbose parameter prints debugging and
//   performance information.
osg::ref_ptr< osg::Geode > vtkActorToOSG(vtkActor* actor, osg::ref_ptr< osg::Geode > geode = NULL, int verbose=0);

osg::ref_ptr< osg::Geometry > processPrimitive(vtkActor *a, vtkCellArray *prims, int pType, int v);
