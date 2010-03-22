/*
-----------------------------------------------------------------------------
This source file is part of FRAPPER
research.animationsinstitut.de
sourceforge.net/projects/frapper

Copyright (c) 2008-2009 Filmakademie Baden-Wuerttemberg, Institute of Animation 

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
*/

//!
//! \file "NodeTreePanelPlugin.cpp"
//! \brief Implementation file for NodeTreePanelPlugin class.
//!
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \version    0.1
//! \date       11.02.2009 (last updated)
//!

#include "NodeTreePanelPlugin.h"
#include "NodeTreePanel.h"
#include <QtCore/QtPlugin>


///
/// Public Functions
///


//!
//! Creates a panel of this panel type.
//!
//! \param parent The parent widget of the panel.
//! \return A pointer to the new panel.
//!
Panel * NodeTreePanelPlugin::createPanel( QWidget *parent)
{
    return new NodeTreePanel(parent);
}


Q_EXPORT_PLUGIN2(NodeTreePanelplugin, NodeTreePanelPlugin)
