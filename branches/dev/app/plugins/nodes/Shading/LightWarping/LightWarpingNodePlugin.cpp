/*
-----------------------------------------------------------------------------
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
//! \file "LightWarpingNodePlugin.cpp"
//! \brief Implementation file for LightWarpingNode class.
//!
//! \author     Felix Bucella <felix.bucella@filmakademie.de>
//! \author     Simon Spielmann <sspielma@filmakademie.de>
//! \version    1.1
//! \date       08.12.2009 (last updated)
//!

#include "LightWarpingNodePlugin.h"
#include "LightWarpingNode.h"
#include <QtCore/QtPlugin>


///
/// Public Functions
///


//!
//! Creates a node of this node type.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//! \return A pointer to the new node.
//!
Node * LightWarpingNodePlugin::createNode ( const QString &name, ParameterGroup *parameterRoot )
{
    return new LightWarpingNode(name, parameterRoot);
}


Q_EXPORT_PLUGIN2(LightWarpingNodeplugin, LightWarpingNodePlugin)
