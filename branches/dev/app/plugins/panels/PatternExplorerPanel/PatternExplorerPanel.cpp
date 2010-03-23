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
//! \file "PatternExplorerPanel.cpp"
//! \brief Implementation file for PatternExplorerPanel class.
//!
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \author     Sebastian Bach <sebastian.bach@filmakademie.de>
//! \version    1.0
//! \date       07.12.2009 (last updated)
//!

#include "PatternExplorerPanel.h"
#include <QFileSystemModel>

///
/// Constructors and Destructors
///

//!
//! Constructor of the PatternExplorerPanel class.
//!
//! \param parent The parent widget the created instance will be a child of.
//! \param flags Extra widget options.
//!
PatternExplorerPanel::PatternExplorerPanel ( QWidget *parent /* = 0 */, Qt::WindowFlags flags /* = 0 */ ) :
    ViewPanel(ViewPanel::T_PluginPanel, parent, flags)
{
	setupUi(this);

	QString templatesDirectory;
	QFileSystemModel* dirModel;
	// New Dir Browser
	dirModel = new QFileSystemModel;

	templatesDirectory = "patterns";

	dirModel->setRootPath(templatesDirectory);
	patternExplorerView->setModel(dirModel);
	patternExplorerView->setRootIndex(dirModel->setRootPath(templatesDirectory));

	// Show only the file name
	for(int i=1; i<dirModel->columnCount(); i++)
		   patternExplorerView->setColumnHidden(i, true);

}


//!
//! Destructor of the NetworkPanel class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
PatternExplorerPanel::~PatternExplorerPanel ()
{

}

///
/// Public Funcitons
///


//!
//! Fills the given tool bars with actions for the PatternExplorerPanel view.
//!
//! \param mainToolBar The main tool bar to fill with actions.
//! \param panelToolBar The panel tool bar to fill with actions.
//!
void PatternExplorerPanel::fillToolBars ( QToolBar *mainToolBar, QToolBar *panelToolBar )
{
	QAction *ui_descriptionAction;
	ui_descriptionAction = new QAction(this);
    ui_descriptionAction->setObjectName(QString::fromUtf8("ui_descriptionAction"));
    ui_descriptionAction->setCheckable(true);
    QIcon icon1;
    icon1.addFile(QString::fromUtf8(":/infoIcon"), QSize(), QIcon::Normal, QIcon::Off);
    ui_descriptionAction->setIcon(icon1);
	ui_descriptionAction->setToolTip("Show Description");

	connect(ui_descriptionAction, SIGNAL(toggled(bool)), this, SLOT(showDiscription(bool)));

	mainToolBar->addAction(ui_descriptionAction);
}

//!
//! Returns the tree view that is used to display the scene objects.
//!
//! \return The tree view that is used to display the scene objects.
//!
QTreeView * PatternExplorerPanel::getPatternExplorerViewPanel ()
{
    return patternExplorerView;
}

///
/// Private Slots
///

//!
//! Sets the description mode
//!
//! \param description boolean value of the action�s state
//!
void PatternExplorerPanel::showDescription(bool description)
{
	m_description = description;
}

