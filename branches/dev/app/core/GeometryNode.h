/*
-----------------------------------------------------------------------------
This source file is part of FRAPPER
research.animationsinstitut.de
sourceforge.net/projects/frapper

Copyright (c) 2008-2009 Filmakademie Baden-Wuerttemberg, Institute of Animation 

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; version 2.1 of the License.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html
-----------------------------------------------------------------------------
*/

//!
//! \file "GeometryNode.h"
//! \brief Header file for GeometryNode class.
//!
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \version    1.0
//! \date       25.03.2009 (last updated)
//!

#ifndef GEOMETRYNODE_H
#define GEOMETRYNODE_H

#include "FrapperPrerequisites.h"
#include "ViewNode.h"


//!
//! Base class for all nodes that produce 3D geometry.
//!
class FRAPPER_CORE_EXPORT GeometryNode : public ViewNode
{

    Q_OBJECT

public: // constructors and destructors

    //!
    //! Constructor of the GeometryNode class.
    //!
    //! \param name The name for the new node.
    //! \param parameterRoot A copy of the parameter tree specific for the type of the node.
    //! \param outputGeometryName The name of the geometry output parameter.
    //!
    GeometryNode ( const QString &name, ParameterGroup *parameterRoot, const QString &outputGeometryName = "Geometry" );

    //!
    //! Destructor of the GeometryNode class.
    //!
    //! Defined virtual to guarantee that the destructor of a derived class
    //! will be called if the instance of the derived class is saved in a
    //! variable of its parent class type.
    //!
    virtual ~GeometryNode ();

public: // functions

    //!
    //! Returns the scene node that contains scene objects created or modified
    //! by this node.
    //!
    //! \return The scene node containing objects created or modified by this node.
    //!
    virtual Ogre::SceneNode * getSceneNode ();

    //!
    //! Returns the radius of the bounding sphere surrounding this object.
    //!
    //! \return The radius of the bounding sphere surrounding this object.
    //!
    double getBoundingSphereRadius () const;

private: // functions

    //!
    //! Attaches a locator geometry to the geometry node's scene node.
    //!
    void createLocatorGeometry ();

protected: // data

    //!
    //! The name of the geometry output parameter.
    //!
    QString m_outputGeometryName;

};


#endif
