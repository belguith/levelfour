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
//! \file "ViewportPanel.cpp"
//! \brief Implementation file for ViewportPanel class.
//!
//! \author     Stefan Habel <stefan.habel@filmakademie.de>
//! \author     Nils Zweiling <nils.zweiling@filmakademie.de>
//! \version    1.0
//! \date       02.07.2009 (last updated)
//!

#include "ViewportPanel.h"
#include "PanelFrame.h"
#include "Window.h"
#include "OgreManager.h"
#include <QtGui/QDesktopWidget.h>
#include <QtGui/QMenu>
#include <QtGui/QToolButton>
#include <QtGui/QComboBox>
#include <QtGui/QPushButton>
#include <QtGui/QColorDialog>



///
/// Constructors and Destructors
///


//!
//! Constructor of the ViewportPanel class.
//!
//! \param parent The parent widget the created instance will be a child of.
//! \param flags Extra widget options.
//!
ViewportPanel::ViewportPanel ( QWidget *parent /* = 0 */, Qt::WindowFlags flags /* = 0 */ ) :
    ViewPanel(ViewPanel::T_Viewport, parent, flags),
    m_viewportWidget(0),
    m_fullscreen(false),
    m_stageActionGroup(0),
    m_cameraComboBox(0),
    m_cameraCreator(false),
	m_fogSpinBox(new QDoubleSpinBox(this)),
	m_fogCheckBox(new QCheckBox(this)),
    m_fogColorButton(new QPushButton(this)),
    m_backgroundColorButton(new QPushButton(this))
{
    setupUi(this);

    m_viewportWidget = new ViewportWidget(this);
    //m_viewportWidget = new ViewportWidget(this, 1, 0, 1);
    unsigned int selectedStageIndex = m_viewportWidget->getStageIndex();
    ui_vboxLayout->addWidget(m_viewportWidget);

    connect(m_viewportWidget, SIGNAL(viewChanged(ViewingParameters *)), SLOT(updateCamera(ViewingParameters *)));

    // set up connections for main tool bar actions
    connect(ui_homeAction, SIGNAL(triggered()), m_viewportWidget, SLOT(homeView()));
    connect(ui_wireframeAction, SIGNAL(toggled(bool)), m_viewportWidget, SLOT(setWireframe(bool)));
    connect(ui_backfaceCullingAction, SIGNAL(toggled(bool)), m_viewportWidget, SLOT(setBackfaceCulling(bool)));
    connect(ui_originAction, SIGNAL(toggled(bool)), m_viewportWidget, SLOT(toggleOrigin()));
    connect(ui_centerPointAction, SIGNAL(toggled(bool)), m_viewportWidget, SLOT(toggleCenterPoint()));
    connect(ui_orientationIndicatorAction, SIGNAL(toggled(bool)), m_viewportWidget, SLOT(toggleOrientationIndicator()));
    connect(ui_gridAction, SIGNAL(toggled(bool)), m_viewportWidget, SLOT(toggleGrid()));
#ifdef USE_STEREO    
    connect(ui_stereoAction, SIGNAL(toggled(bool)), this, SLOT(toggleFullscreen(bool)));
    connect(ui_stereoDialogAction, SIGNAL(triggered()), this, SLOT(showStereoDialog()));
#endif
	connect(m_fogSpinBox, SIGNAL(valueChanged(double)), this, SLOT(changeFog(double)));
	connect(m_fogCheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeFog(int)));
    connect(m_fogColorButton, SIGNAL(pressed()), this, SLOT(changeFogColor()));
    connect(m_backgroundColorButton, SIGNAL(pressed()), this, SLOT(changeBackgroundColor()));

    // initialize main tool bar actions
    ui_wireframeAction->setChecked(false);
    ui_backfaceCullingAction->setChecked(false);
    ui_gridAction->setChecked(true);
    ui_orientationIndicatorAction->setChecked(true);
    ui_stereoAction->setChecked(false);
    ui_overlayOnlyAction->setChecked(false);
    m_parameterMap["overlayOnly"] = "0";


    // create an action group for the stage actions
    m_stageActionGroup = new QActionGroup(this);
    for (unsigned int stageIndex = 1; stageIndex <= NUMBER_OF_STAGES; ++stageIndex) {
        QString text;
        if (stageIndex < 10)
            text = QString("Stage &%1").arg(stageIndex);
        else if (stageIndex == 10)
            text = "Stage 1&0";
        else
            text = QString("Stage %1").arg(stageIndex);
        QIcon icon (QString(":/stage%1Icon").arg(stageIndex));
        QAction *stageAction = new QAction(icon, text, m_stageActionGroup);
        QVariant indexVariant(stageIndex);
        stageAction->setData(indexVariant);
        connect(stageAction, SIGNAL(toggled(bool)), SLOT(selectStage()));
        stageAction->setCheckable(true);
        if (stageIndex == selectedStageIndex) {
            stageAction->setChecked(true);
            m_parameterMap["stageIndex"] = QString::number(stageIndex);
        }
    }
    //connect(m_stageActionGroup, SIGNAL(triggered(QAction *)), SLOT(selectStage(QAction *)));

    // create the stages drop-down menu
    QMenu *stagesMenu = new QMenu("Stages Menu", this);
    stagesMenu->addActions(m_stageActionGroup->actions());
    ui_stagesMenuAction->setMenu(stagesMenu);
}


//!
//! Destructor of the ViewportPanel class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
ViewportPanel::~ViewportPanel ()
{
}


///
/// Public Functions
///


//!
//! Fills the given tool bars with actions for the viewport view.
//!
//! \param mainToolBar The main tool bar to fill with actions.
//! \param panelToolBar The panel tool bar to fill with actions.
//!
void ViewportPanel::fillToolBars ( QToolBar *mainToolBar, QToolBar *panelToolBar )
{
    //mainToolBar->addAction(ui_previousViewAction);
    //mainToolBar->addAction(ui_nextViewAction);
    //mainToolBar->addSeparator();
    mainToolBar->addAction(ui_homeAction);
    mainToolBar->addSeparator();
    mainToolBar->addAction(ui_wireframeAction);
    mainToolBar->addAction(ui_backfaceCullingAction);
    mainToolBar->addSeparator();
    mainToolBar->addAction(ui_originAction);
    mainToolBar->addAction(ui_centerPointAction);
    mainToolBar->addAction(ui_orientationIndicatorAction);
    mainToolBar->addAction(ui_gridAction);
#ifdef USE_STEREO
    mainToolBar->addSeparator();
    mainToolBar->addAction(ui_stereoDialogAction);
    mainToolBar->addAction(ui_stereoAction);
#endif
    mainToolBar->addSeparator();
    mainToolBar->addAction(ui_overlayOnlyAction);
	mainToolBar->addSeparator();
	QLabel *fogLabel = new QLabel(tr("Fog - on/off: "), this);
    QLabel *fogDensityLabel = new QLabel(tr("Density: "), this);
    QLabel *fogColorLabel = new QLabel(tr("Color: "), this);
    QLabel *fogSpacerLabel = new QLabel(tr(" "), this);
    fogLabel->setFixedWidth(70);
    fogDensityLabel->setFixedWidth(40);
    fogColorLabel->setFixedWidth(30);
    fogSpacerLabel->setFixedWidth(10);
    m_fogSpinBox->setDecimals(3);
    m_fogSpinBox->setSingleStep(0.001);
	mainToolBar->addWidget(fogLabel);
    mainToolBar->addWidget(m_fogCheckBox);
    mainToolBar->addWidget(fogDensityLabel);
	mainToolBar->addWidget(m_fogSpinBox);
    if (m_fogColorButton) {
        const QColor &fogColor = m_viewportWidget->getFogColor();    
        m_fogColorButton->setStyleSheet(QString("background: rgb(%1, %2, %3);").arg(fogColor.red()).arg(fogColor.green()).arg(fogColor.blue()));
    }
    mainToolBar->addWidget(fogSpacerLabel);
    mainToolBar->addWidget(fogColorLabel);
    mainToolBar->addWidget(m_fogColorButton);
    mainToolBar->addSeparator();
    if (m_backgroundColorButton) {
        const QColor &backgroundColor = m_viewportWidget->getBackgroundColor();    
        m_backgroundColorButton->setStyleSheet(QString("background: rgb(%1, %2, %3);").arg(backgroundColor.red()).arg(backgroundColor.green()).arg(backgroundColor.blue()));
    }
    QLabel *backgroundColorLabel = new QLabel(tr("Background color: "), this);
    mainToolBar->addWidget(backgroundColorLabel);
    mainToolBar->addWidget(m_backgroundColorButton);

    // create the camera selection combo box
    m_cameraComboBox = new QComboBox(panelToolBar);
    m_cameraComboBox->setMinimumHeight(22);
    connect(m_cameraComboBox, SIGNAL(currentIndexChanged(const QString &)), SLOT(selectCamera(const QString &)));
    //m_cameraComboBox->addItem(tr("Free Camera"));
    ui_saveViewAction->setEnabled(false);

    // fill the panel tool bar with camera actions and the camera selection combo box
    panelToolBar->addAction(ui_addCameraAction);
    panelToolBar->addAction(ui_saveViewAction);
    panelToolBar->addWidget(m_cameraComboBox);
    panelToolBar->addAction(ui_linkViewAction);
    panelToolBar->addAction(ui_selectCameraAction);
    panelToolBar->addAction(ui_removeCameraAction);
    panelToolBar->addSeparator();
    panelToolBar->addAction(ui_stagesMenuAction);

    // set the stages menu's tool button to instantly display the menu when clicked
    QToolButton *stagesToolButton = qobject_cast<QToolButton *>(panelToolBar->widgetForAction(ui_stagesMenuAction));
    if (stagesToolButton)
        stagesToolButton->setPopupMode(QToolButton::InstantPopup);

    emit requestCameraList();
}

//!
//! Sets the panel parameters. For load and save functionality.
//! 
void ViewportPanel::setPanelParameters ( const QMap<QString, QString> &parameterMap )
{
    QMap<QString, QString>::const_iterator iter;
    for (iter = parameterMap.begin(); iter != parameterMap.end(); iter++) {
        const QString &parameterName = iter.key();
        const QString &parameterValue = iter.value();
        
        // set stage index
        if (parameterName == "stageIndex") {
            QList<QAction *> stageActions = m_stageActionGroup->actions();
            for (unsigned int i = 0; i < NUMBER_OF_STAGES; ++i) {
                QAction *stageAction = stageActions[i];
                if (i+1 == parameterValue.toUInt()) {
                    stageAction->setChecked(true);
                }
                //else
                //  stageAction->setChecked(false);
            }
        }
        // set view only overlay
        else if (parameterName == "overlayOnly") {
            bool checked = (bool) parameterValue.toUInt();
            ui_overlayOnlyAction->setChecked((bool) parameterValue.toUInt());
        }
        // set camera link status
        else if (parameterName == "cameraLinked") {
            bool checked = (bool) parameterValue.toUInt();
            ui_linkViewAction->setChecked((bool) parameterValue.toUInt());
        }
        // set current camera
        else if (parameterName == "cameraName") {
            const QString &cameraName = parameterValue;
            int index = m_cameraComboBox->findText(cameraName);
            if (index > -1)
                // select the free camera
                m_cameraComboBox->setCurrentIndex(index);
        }
    }
}


//!
//! Returns the viewport widget that is used for displaying the scene.
//!
//! \return the viewport widget that is used for displaying the scene.
//!
ViewportWidget * ViewportPanel::getViewportWidget () const
{
    return m_viewportWidget;
}


///
/// Public Slots
///


////!
////! Adds a new entry for the camera with the given name to the list of
////! available cameras.
////!
////! \param cameraName The name of the camera to add to the list of available cameras.
////!
//void ViewportPanel::addCamera ( const QString &cameraName )
//{
//    if (!m_cameraComboBox)
//        return;
//
//    m_cameraComboBox->addItem(cameraName);
//
//    // check if the camera has been created because the user has triggered this viewport panel's ui_addCameraAction
//    if (m_cameraCreator) {
//        int index = m_cameraComboBox->findText(cameraName);
//        m_cameraComboBox->setCurrentIndex(index);
//    }
//
//    ui_saveViewAction->setEnabled(true);
//}
//
//
////!
////! Removes the entry for the camera with the given name from the list of
////! available cameras.
////!
////! \param cameraName The name of the camera to remove from the list of available cameras.
////!
//void ViewportPanel::removeCamera ( const QString &cameraName )
//{
//    if (!m_cameraComboBox)
//        return;
//
//    int index = m_cameraComboBox->findText(cameraName);
//    if (index > -1) {
//        // select the free camera
//        m_cameraComboBox->setCurrentIndex(0);
//        // remove the camera to remove
//        m_cameraComboBox->removeItem(index);
//
//        ui_saveViewAction->setEnabled(m_cameraComboBox->count() > 1);
//    }
//}

//!
//! Updates the camera drop down menu.
//!
void ViewportPanel::updateCameras ( const QList<Node *> &cameraNodes, const QString &cameraName )
{
    if (!m_cameraComboBox)
        return;
    
    int currentIndex = m_cameraComboBox->currentIndex(); 
    
    m_cameraComboBox->clear();
    
    for (int i = 0; i < cameraNodes.size(); ++i) {
        if (cameraNodes[i]) {
            const QString &nodeName = cameraNodes[i]->getName(); 
            m_cameraComboBox->addItem(nodeName);
        }
    }

    int index = -1;
    if (cameraName == "") {
        if (currentIndex > m_cameraComboBox->count()-1)
            index = m_cameraComboBox->count()-1;
        else 
            index = currentIndex;
    }
    else
        index = m_cameraComboBox->findText(cameraName);

    if (index > -1) {
        // select the free camera
        m_cameraComboBox->setCurrentIndex(index);
        ui_saveViewAction->setEnabled(m_cameraComboBox->count() > 1);
        if (ui_linkViewAction->isChecked()) {
            emit applyCameraRequested(m_cameraComboBox->currentText(), m_viewportWidget->getViewingParameters());
            m_viewportWidget->updateView();
            m_viewportWidget->update();
        }
        // set parameters for load/save
        m_parameterMap["cameraName"] = m_cameraComboBox->currentText();
    }
    else {
        ui_linkViewAction->setChecked(false);
        // set parameters for load/save
        m_parameterMap["cameraName"] = "";
    }
}

///
/// Protected Events
///


//!
//! Handles key press events for the widget.
//!
//! \param event The description of the key event.
//!
void ViewportPanel::keyPressEvent ( QKeyEvent *event )
{
    //Log::debug(QString("Key %1 has been pressed or is still pressed (auto repeating: %2).").arg(event->key()).arg(event->isAutoRepeat()), "ViewportPanel::keyPressEvent");

    if (event->isAutoRepeat() && event->key() != Qt::Key_Left && event->key() != Qt::Key_Right) {
        event->ignore();
        return;
    }

    switch (event->key()) {
        case Qt::Key_B:
            ui_backfaceCullingAction->toggle();
            break;
        case Qt::Key_C:
            ui_centerPointAction->toggle();
            break;
        case Qt::Key_D:
            ui_stereoDialogAction->trigger();
            break;
        case Qt::Key_F:
            m_viewportWidget->frameSelectedObjects();
            break;
        case Qt::Key_G:
            ui_gridAction->toggle();
            break;
        case Qt::Key_H:
            ui_homeAction->trigger();
            break;
        case Qt::Key_I:
            ui_orientationIndicatorAction->toggle();
            break;
        case Qt::Key_L:
            ui_linkViewAction->toggle();
            break;
        case Qt::Key_O:
            ui_originAction->toggle();
            break;
        case Qt::Key_S:
            ui_stereoAction->toggle();
            break;
        case Qt::Key_W:
            ui_wireframeAction->toggle();
            break;
        case Qt::Key_Alt:
            m_viewportWidget->setViewMode(true);
            break;
        default:
            ViewPanel::keyPressEvent(event);
    }
}


//!
//! Handles key release events for the widget.
//!
//! \param event The description of the key event.
//!
void ViewportPanel::keyReleaseEvent ( QKeyEvent *event )
{
    if (event->isAutoRepeat()) {
        event->ignore();
        return;
    }

    if (event->key() == Qt::Key_Alt)
        m_viewportWidget->setViewMode(false);

    ViewPanel::keyReleaseEvent(event);
}


///
/// Protected Slots
///


//!
//! Toggles displaying the viewport widget in fullscreen mode.
//!
//! \param enable Flag to enable or disable the fullscreen mode for the viewport widget.
//!
void ViewportPanel::toggleFullscreen ( bool enable )
{
    PanelFrame *parentPanelFrame = dynamic_cast<PanelFrame *>(parentWidget());
    Window *parentWindow = dynamic_cast<Window *>(parentPanelFrame->parentWidget());

    if (!parentPanelFrame || !parentWindow)
        return;

    parentPanelFrame->toggleToolBars(!enable);
    parentWindow->setFullscreen(enable);
    m_viewportWidget->setStereo(enable);
}


//!
//! Show stereo config dialog.
//!
void ViewportPanel::showStereoDialog ()
{
    // create stereo parameter spin box map
    m_stereoParameterSBMap.clear();
    double rttSizeX = 0.0;
    double rttSizeY = 0.0;
    double eyeDist = 0.0;
    double fOffsetMultiplier = 0;
    double nearClip = 0.0;
    double farClip = 0.0;
    double fov = 0.0;
    double focalLength = 0.0;

    m_viewportWidget->getStereoParameters(rttSizeX, rttSizeY, eyeDist, fOffsetMultiplier, nearClip, farClip, fov, focalLength);

    m_stereoParameterSBMap["Rtt Size X"] = new QDoubleSpinBox();
    m_stereoParameterSBMap["Rtt Size X"]->setMaximum(2048.0);
    m_stereoParameterSBMap["Rtt Size X"]->setValue(rttSizeX);
    m_stereoParameterSBMap["Rtt Size Y"] = new QDoubleSpinBox();
    m_stereoParameterSBMap["Rtt Size Y"]->setMaximum(2048.0);
    m_stereoParameterSBMap["Rtt Size Y"]->setValue(rttSizeY);
    m_stereoParameterSBMap["Eye Dist"] = new QDoubleSpinBox();
    m_stereoParameterSBMap["Eye Dist"]->setMaximum(1.0);
    m_stereoParameterSBMap["Eye Dist"]->setMinimum(-1.0);
    m_stereoParameterSBMap["Eye Dist"]->setDecimals(5);
    m_stereoParameterSBMap["Eye Dist"]->setValue(eyeDist);
    m_stereoParameterSBMap["Frustum Multiplier"] = new QDoubleSpinBox();
    m_stereoParameterSBMap["Frustum Multiplier"]->setMaximum(5.0);
    m_stereoParameterSBMap["Frustum Multiplier"]->setMinimum(-5.0);
    m_stereoParameterSBMap["Frustum Multiplier"]->setValue(fOffsetMultiplier);
    m_stereoParameterSBMap["Near Clip"] = new QDoubleSpinBox();
    m_stereoParameterSBMap["Near Clip"]->setMaximum(100.0);
    m_stereoParameterSBMap["Near Clip"]->setValue(nearClip);
    m_stereoParameterSBMap["Far Clip"] = new QDoubleSpinBox();
    m_stereoParameterSBMap["Far Clip"]->setMaximum(10000.0);
    m_stereoParameterSBMap["Far Clip"]->setValue(farClip);
    m_stereoParameterSBMap["Fov"] = new QDoubleSpinBox();
    m_stereoParameterSBMap["Fov"]->setMaximum(180.0);
    m_stereoParameterSBMap["Fov"]->setValue(fov);
    m_stereoParameterSBMap["Focal Length"] = new QDoubleSpinBox();
    m_stereoParameterSBMap["Focal Length"]->setMaximum(1000.0);
    m_stereoParameterSBMap["Focal Length"]->setValue(focalLength);
    m_stereoParameterSBMap["Width Correction X"] = new QDoubleSpinBox();
    m_stereoParameterSBMap["Height Correction Y"] = new QDoubleSpinBox();
    m_stereoParameterSBMap["Offset X"] = new QDoubleSpinBox();
    m_stereoParameterSBMap["Offset Y"] = new QDoubleSpinBox();

    QMap<QString, QDoubleSpinBox *>::iterator iter = m_stereoParameterSBMap.begin();
    QVBoxLayout *layout = new QVBoxLayout();
    QHBoxLayout *hLayout;
    QLabel *label;
    while (iter != m_stereoParameterSBMap.end()) {
        hLayout = new QHBoxLayout();
        label = new QLabel(iter.key());
        hLayout->addWidget(label);
        hLayout->addWidget(iter.value());
        layout->addLayout(hLayout);
        ++iter;
    }

    QPushButton *button = new QPushButton("Apply");
    layout->addWidget(button);
    connect(button, SIGNAL(clicked()), SLOT(setStereoParameters()));

    QDialog *stereoDialog = new QDialog(this);
    stereoDialog->setModal(false);
    stereoDialog->resize(300, 300);
    stereoDialog->setWindowTitle(tr("Stereo Configuration"));
    stereoDialog->setLayout(layout);
    stereoDialog->show();
}


//!
//! Show stereo config dialog.
//!
void ViewportPanel::setStereoParameters ()
{
    if ( m_stereoParameterSBMap["Rtt Size X"]
         && m_stereoParameterSBMap["Rtt Size Y"]
         && m_stereoParameterSBMap["Eye Dist"]
         && m_stereoParameterSBMap["Frustum Multiplier"]
         && m_stereoParameterSBMap["Near Clip"]
         && m_stereoParameterSBMap["Far Clip"]
         && m_stereoParameterSBMap["Fov"]
         && m_stereoParameterSBMap["Focal Length"]
         && m_stereoParameterSBMap["Width Correction X"]
         && m_stereoParameterSBMap["Height Correction Y"]
         && m_stereoParameterSBMap["Offset X"]
         && m_stereoParameterSBMap["Offset Y"]
       )
       {
           m_viewportWidget->setStereoParameters((unsigned int) m_stereoParameterSBMap["Rtt Size X"]->value(), (unsigned int) m_stereoParameterSBMap["Rtt Size Y"]->value(),
                                                 m_stereoParameterSBMap["Eye Dist"]->value(), m_stereoParameterSBMap["Frustum Multiplier"]->value(),
                                                 m_stereoParameterSBMap["Near Clip"]->value(), m_stereoParameterSBMap["Far Clip"]->value(),
                                                 m_stereoParameterSBMap["Fov"]->value(), m_stereoParameterSBMap["Focal Length"]->value());
       }
}
///
/// Private Slots
///


//!
//! Requests the creation of a new camera node with the current viewing
//! parameters.
//!
//! \param checked The action's checked state.
//!
void ViewportPanel::on_ui_addCameraAction_triggered ( bool checked /* = false */ )
{
    m_cameraCreator = true;
    emit newCameraRequested(m_viewportWidget->getViewingParameters());
    m_cameraCreator = false;
}


//!
//! Requests an update of the currently selected camera if the action is
//! currently checked.
//!
//! \param checked The action's checked state.
//!
void ViewportPanel::on_ui_linkViewAction_toggled ( bool checked )
{
    if (m_cameraComboBox /* && m_cameraComboBox->currentIndex() > 0 */ && checked) {
        emit applyCameraRequested(m_cameraComboBox->currentText(), m_viewportWidget->getViewingParameters());
        //emit updateCameraRequested(m_cameraComboBox->currentText(), m_viewportWidget->getViewingParameters());
        m_viewportWidget->updateView();
        m_viewportWidget->update();
    }

    // set parameters for load/save
    if (checked)
        m_parameterMap["cameraLinked"] = "1";
    else
        m_parameterMap["cameraLinked"] = "0";
}


//!
//! Requests the selection of an existing camera node.
//!
//! \param checked The action's checked state.
//!
void ViewportPanel::on_ui_selectCameraAction_triggered ( bool checked /* = false */ )
{
    if (m_cameraComboBox /* && m_cameraComboBox->currentIndex() > 0 */) {
        emit selectCameraRequested(m_cameraComboBox->currentText(), false);
    }
}


//!
//! Requests the deletion of an existing camera node.
//!
//! \param checked The action's checked state.
//!
void ViewportPanel::on_ui_removeCameraAction_triggered ( bool checked /* = false */ )
{
    if (m_cameraComboBox /* && m_cameraComboBox->currentIndex() > 0 */)
        emit deleteCameraRequested(m_cameraComboBox->currentText());
}

//!
//! Sets whether only overlays (no geometry) should be displayed in viewport.
//!
//! \param checked The action's checked state.
//!
void ViewportPanel::on_ui_overlayOnlyAction_toggled ( bool checked /* = false */ )
{
    m_viewportWidget->setOverlayOnly(checked);
    m_viewportWidget->update();
    
    // set parameters for load/save
    if (checked)
        m_parameterMap["overlayOnly"] = "1";
    else
        m_parameterMap["overlayOnly"] = "0";
}


//!
//! Selects the camera to use for the viewport.
//!
//! \param cameraName The name of the camera to use.
//!
void ViewportPanel::selectCamera ( const QString &cameraName )
{
    bool freeCameraSelected = cameraName == tr("Free Camera");

    ui_linkViewAction->setEnabled(!freeCameraSelected);
    ui_selectCameraAction->setEnabled(!freeCameraSelected);
    ui_removeCameraAction->setEnabled(!freeCameraSelected);

    if (ui_linkViewAction->isChecked()) {
        emit applyCameraRequested(m_cameraComboBox->currentText(), m_viewportWidget->getViewingParameters());
        m_viewportWidget->updateView();
        m_viewportWidget->update();
    }

    //if (!freeCameraSelected)
    //    emit applyCameraRequested(cameraName, m_viewportWidget->getViewingParameters());
}


//!
//! Requests an update of the currently selected camera if the
//! ui_linkViewAction is currently checked.
//!
//! \param viewingParameters The viewing parameters currently set in the viewport
//!
void ViewportPanel::updateCamera ( ViewingParameters *viewingParameters )
{
    if (m_cameraComboBox && /* m_cameraComboBox->currentIndex() > 0 && */ ui_linkViewAction->isChecked())
        emit updateCameraRequested(m_cameraComboBox->currentText(), viewingParameters);
}


//!
//! Selects the stage to display in the viewport corresponding to the
//! triggered stage action.
//!
//! \param triggeredAction The action that was triggered.
//!
void ViewportPanel::selectStage ( /*QAction *triggeredAction*/ )
{
    QAction *sender = dynamic_cast<QAction *>(this->sender());
    if (sender) {
        QVariant data = sender->data();
        if (!data.isNull() && data.isValid() && data.canConvert(QVariant::UInt)) {
            unsigned int index = data.toUInt();
            m_viewportWidget->setStageIndex(index);
            ui_stagesMenuAction->setIcon(sender->icon());
            m_parameterMap["stageIndex"] = QString::number(index);
            emit triggerRebuild();
        }
    }
    //int index = m_stageActionGroup->actions().indexOf(triggeredAction);
    //if (index >= 0) {
    //    if (triggeredAction->isChecked()) {
    //        m_viewportWidget->setStageIndex(index + 1);
    //        ui_stagesMenuAction->setIcon(triggeredAction->icon());
    //        m_parameterMap["stageIndex"] = QString::number(index + 1);
    //    }
    //} else
    //    Log::error("The triggered action could not be found in the list of actions contained in the stage action group.", "ViewportPanel::selectStage");
}

//!
//! Sets new fog configuration.
//!
void ViewportPanel::changeFog ( double value )
{
	m_viewportWidget->setFog(m_fogCheckBox->isChecked(), m_fogSpinBox->value());	
}

//!
//! Sets new fog configuration.
//!
void ViewportPanel::changeFog ( int value )
{
	m_viewportWidget->setFog(m_fogCheckBox->isChecked(), m_fogSpinBox->value());	
}

//!
//! Changes fog color.
//!
void ViewportPanel::changeFogColor ()
{
    const QColor &currentFogColor = m_viewportWidget->getFogColor();
    const QColor &fogColor = QColorDialog::getColor(currentFogColor);
    if (fogColor.isValid()) {
        m_fogColorButton->setStyleSheet(QString("background: rgb(%1, %2, %3);").arg(fogColor.red()).arg(fogColor.green()).arg(fogColor.blue()));
        m_viewportWidget->setFogColor(fogColor);
        m_viewportWidget->setFog(m_fogCheckBox->isChecked(), m_fogSpinBox->value());
    }
}

//!
//! Changes background color.
//!
void ViewportPanel::changeBackgroundColor ( )
{
    const QColor &currentBackgroundColor = m_viewportWidget->getBackgroundColor();
    const QColor &backgroundColor = QColorDialog::getColor(currentBackgroundColor);    
    if (backgroundColor.isValid()) {
        m_backgroundColorButton->setStyleSheet(QString("background: rgb(%1, %2, %3);").arg(backgroundColor.red()).arg(backgroundColor.green()).arg(backgroundColor.blue()));
        m_viewportWidget->setBackgroundColor(backgroundColor);
    }
}

