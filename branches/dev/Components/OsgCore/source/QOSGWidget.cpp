/* OpenSceneGraph example, osganimate.
*
*  Permission is hereby granted, free of charge, to any person obtaining a copy
*  of this software and associated documentation files (the "Software"), to deal
*  in the Software without restriction, including without limitation the rights
*  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*  copies of the Software, and to permit persons to whom the Software is
*  furnished to do so, subject to the following conditions:
*
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
*  THE SOFTWARE.
*/


#include <QOSGWidget.h>

QOSGWidget::QOSGWidget( QWidget * parent, const char * name, WindowFlags f, bool overrideTraits):
#if USE_QT4
    QWidget(parent, f), _overrideTraits (overrideTraits)
#else
    QWidget(parent, name, f), _overrideTraits (overrideTraits)
#endif
{
    createContext();
    

#if USE_QT4
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);
    setFocusPolicy(Qt::ClickFocus);
#else
    setBackgroundMode(Qt::NoBackground);
#endif    
}

void QOSGWidget::createContext()
{
    osg::DisplaySettings* ds = osg::DisplaySettings::instance();

    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;

    traits->readDISPLAY();
    if (traits->displayNum<0) traits->displayNum = 0;

    traits->windowName = "osgViewerQt";
    traits->screenNum = 0;
    traits->x = x();
    traits->y = y();
    traits->width = width();
    traits->height = height();
    traits->alpha = ds->getMinimumNumAlphaBits();
    traits->stencil = ds->getMinimumNumStencilBits();
    traits->windowDecoration = false;
    traits->doubleBuffer = false; // if set to true -> crash
    traits->sharedContext = 0;
    traits->sampleBuffers = ds->getMultiSamples();
    traits->samples = ds->getNumMultiSamples();

#if defined(__APPLE__) 
    // Extract a WindowPtr from the HIViewRef that QWidget::winId() returns.
    // Without this change, the peer tries to call GetWindowPort on the HIViewRef
    // which returns 0 and we only render white.
    traits->inheritedWindowData = new WindowData(HIViewGetWindow((HIViewRef)winId()));

#else // all others
    traits->inheritedWindowData = new WindowData(winId());
#endif


    if (ds->getStereo())
    {
        switch(ds->getStereoMode())
        {
            case(osg::DisplaySettings::QUAD_BUFFER): traits->quadBufferStereo = true; break;
            case(osg::DisplaySettings::VERTICAL_INTERLACE):
            case(osg::DisplaySettings::CHECKERBOARD):
            case(osg::DisplaySettings::HORIZONTAL_INTERLACE): traits->stencil = 8; break;
            default: break;
        }
    }

    osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
    _gw = dynamic_cast<osgViewer::GraphicsWindow*>(gc.get());

    // get around dearanged traits on X11 (MTCompositeViewer only)
    if (_overrideTraits)
    {
        traits->x = x();
        traits->y = y();
        traits->width = width();
        traits->height = height();
    }

}

QPaintEngine* QWidget::paintEngine () const
{
    return 0;
}

#ifndef WIN32 
void QOSGWidget::destroyEvent(bool destroyWindow, bool destroySubWindows)
{   
    _gw->getEventQueue()->closeWindow();
}


void QOSGWidget::closeEvent( QCloseEvent * event )
{
#ifndef USE_QT4
    event->accept();
#endif

    _gw->getEventQueue()->closeWindow();
}


void QOSGWidget::resizeEvent( QResizeEvent * event )
{
    const QSize & size = event->size();
    _gw->getEventQueue()->windowResize(0, 0, size.width(), size.height() );
    _gw->resized(0, 0, size.width(), size.height());
}

void QOSGWidget::keyPressEvent( QKeyEvent* event )
{
#if USE_QT4
    _gw->getEventQueue()->keyPress( (osgGA::GUIEventAdapter::KeySymbol) *(event->text().toAscii().data() ) );
#else
    _gw->getEventQueue()->keyPress( (osgGA::GUIEventAdapter::KeySymbol) event->ascii() );
#endif
}

void QOSGWidget::keyReleaseEvent( QKeyEvent* event )
{
#if USE_QT4
    int c = *event->text().toAscii().data();
#else
    int c = event->ascii();
#endif

    _gw->getEventQueue()->keyRelease( (osgGA::GUIEventAdapter::KeySymbol) (c) );
}

void QOSGWidget::mousePressEvent( QMouseEvent* event )
{
    int button = 0;
    switch(event->button())
    {
        case(Qt::LeftButton): button = 1; break;
        case(Qt::MidButton): button = 2; break;
        case(Qt::RightButton): button = 3; break;
        case(Qt::NoButton): button = 0; break;
        default: button = 0; break;
    }
    _gw->getEventQueue()->mouseButtonPress(event->x(), event->y(), button);
}
void QOSGWidget::mouseDoubleClickEvent ( QMouseEvent * event )
{
    int button = 0;
    switch(event->button())
    {
        case(Qt::LeftButton): button = 1; break;
        case(Qt::MidButton): button = 2; break;
        case(Qt::RightButton): button = 3; break;
        case(Qt::NoButton): button = 0; break;
        default: button = 0; break;
    }
    _gw->getEventQueue()->mouseDoubleButtonPress(event->x(), event->y(), button);
}
void QOSGWidget::mouseReleaseEvent( QMouseEvent* event )
{
    int button = 0;
    switch(event->button())
    {
        case(Qt::LeftButton): button = 1; break;
        case(Qt::MidButton): button = 2; break;
        case(Qt::RightButton): button = 3; break;
        case(Qt::NoButton): button = 0; break;
        default: button = 0; break;
    }
    _gw->getEventQueue()->mouseButtonRelease(event->x(), event->y(), button);
}

void QOSGWidget::mouseMoveEvent( QMouseEvent* event )
{
    _gw->getEventQueue()->mouseMotion(event->x(), event->y());
}
#endif

void CompositeViewerQOSG::Tile()
{
  int n = getNumViews() - 1; // -1 to account for dummy view

  for ( int i = 0; i < n; ++i )
  {
    osgViewer::View * view = getView(i+1);  // +1 to account for dummy view
    view->getCamera()->setViewport( new osg::Viewport( 0, i*height()/n , width(), height()/n ) );
    view->getCamera()->setProjectionMatrixAsPerspective( 30.0f, double( width() ) / double( height()/n ), 1.0f, 10000.0f );
  }
}


void CompositeViewerQOSG::AddView( osg::Node * scene )
{
  osgViewer::View* view = new osgViewer::View;
  addView(view);

  // scene->addParent(mpXYGridTransform);
  // viewport interfaces
 
  view->setSceneData( scene );
  view->setCameraManipulator(new osgGA::TrackballManipulator);

  // add the state manipulator
  osg::ref_ptr<osgGA::StateSetManipulator> statesetManipulator = new osgGA::StateSetManipulator;
  statesetManipulator->setStateSet(view->getCamera()->getOrCreateStateSet());

  //prepare scene
  view->getCamera()->setGraphicsContext( getGraphicsWindow() );
  view->getCamera()->setClearColor( osg::Vec4( 0.08, 0.08, 0.5, 1.0 ) );
  Tile();
}

void CompositeViewerQOSG::RemoveView()
{
  if ( getNumViews() > 1 )
  {
    removeView( getView( getNumViews() - 1 ) );
  }
  Tile();
}

// Set up viewport interfaces
////////////////////////////////////////////////////////////////////////////////
void ViewerQOSG::ToggleXYGrid(bool enabled)
{
   if(enabled)
   {
      mpXYGridTransform->setNodeMask(0xFFFFFFFF);
   }
   else
   {
      mpXYGridTransform->setNodeMask(0x0);
   }
}

////////////////////////////////////////////////////////////////////////////////
void ViewerQOSG::ToggleXZGrid(bool enabled)
{
   if(enabled)
   {
      mpXZGridTransform->setNodeMask(0xFFFFFFFF);
   }
   else
   {
      mpXZGridTransform->setNodeMask(0x0);
   }
}

////////////////////////////////////////////////////////////////////////////////
void ViewerQOSG::ToggleYZGrid(bool enabled)
{
   if(enabled)
   {
      mpYZGridTransform->setNodeMask(0xFFFFFFFF);
   }
   else
   {
      mpYZGridTransform->setNodeMask(0x0);
   }
}


////////////////////////////////////////////////////////////////////////////////
void ViewerQOSG::MakeGrids()
{
   const int numVertices = 2 * 2 * GRID_LINE_COUNT;
   osg::Vec3 vertices[numVertices];
   float length = (GRID_LINE_COUNT - 1) * GRID_LINE_SPACING;
   int ptr = 0;

   for(int i = 0; i < GRID_LINE_COUNT; ++i)
   {
      vertices[ptr++].set(-length / 2 + i * GRID_LINE_SPACING, length / 2, 0.0f);
      vertices[ptr++].set(-length / 2 + i * GRID_LINE_SPACING, -length / 2, 0.0f);
   }

   for (int i = 0; i < GRID_LINE_COUNT; ++i)
   {
      vertices[ptr++].set(length / 2, -length / 2 + i * GRID_LINE_SPACING, 0.0f);
      vertices[ptr++].set(-length / 2, -length / 2 + i * GRID_LINE_SPACING, 0.0f);
   }

   osg::Geometry* geometry = new osg::Geometry;
   geometry->setVertexArray(new osg::Vec3Array(numVertices, vertices));
   geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, numVertices));

   osg::Geode* geode = new osg::Geode;
   geode->addDrawable(geometry);
   geode->getOrCreateStateSet()->setMode(GL_LIGHTING, 0);

   mpXYGridTransform = new osg::MatrixTransform;
   mpXYGridTransform->addChild(geode);

   mpXZGridTransform = new osg::MatrixTransform;
   mpXZGridTransform->setMatrix(osg::Matrix::rotate(osg::PI_2, 1, 0, 0));

   mpXZGridTransform->addChild(geode);
   mpXZGridTransform->setNodeMask(0x0);

   mpYZGridTransform = new osg::MatrixTransform;
   mpYZGridTransform->setMatrix(osg::Matrix::rotate(osg::PI_2, 0, 1, 0));

   mpYZGridTransform->addChild(geode);
   mpYZGridTransform->setNodeMask(0x0);
}

void setupManipulatorAndHandler(osgViewer::View & viewer, osg::ArgumentParser & arguments)
{
    // set up the camera manipulators.
    {
        osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> keyswitchManipulator = new osgGA::KeySwitchMatrixManipulator;

        keyswitchManipulator->addMatrixManipulator( '1', "Trackball", new osgGA::TrackballManipulator() );
        keyswitchManipulator->addMatrixManipulator( '2', "Flight", new osgGA::FlightManipulator() );
        keyswitchManipulator->addMatrixManipulator( '3', "Drive", new osgGA::DriveManipulator() );
        keyswitchManipulator->addMatrixManipulator( '4', "Terrain", new osgGA::TerrainManipulator() );

        viewer.setCameraManipulator( keyswitchManipulator.get() );
    }

    // add the state manipulator
    viewer.addEventHandler( new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()) );

    // add the thread model handler
    viewer.addEventHandler(new osgViewer::ThreadingHandler);

    // add the window size toggle handler
    viewer.addEventHandler(new osgViewer::WindowSizeHandler);

    // add the stats handler
    viewer.addEventHandler(new osgViewer::StatsHandler);

    // add the help handler
    viewer.addEventHandler(new osgViewer::HelpHandler(arguments.getApplicationUsage()));
}
int mainQOSGWidgetTest(QApplication& a, osg::ArgumentParser& arguments)
{
    // load the scene.
     osg::ref_ptr<ViewerQOSG> viewerWindow(new ViewerQOSG);

	 osg::ref_ptr<osg::Geode> loadedModel = new osg::Geode();
	osg::ref_ptr<osg::ShapeDrawable> sphere = new osg::ShapeDrawable(new osg::Box());
	loadedModel->addDrawable(sphere.get());

    // Open the ViewerQOSG window at 30/30 instead of 0/0.  In some instances,
    // the window may otherwise lack any window decoration.
    // viewerWindow->setGeometry(0,0,640,480);
    viewerWindow->setGeometry(30,30,640,480);

    // Setup the camera only after ViewerQOSG's Qt base class has been
    // initialized. Without this change the view doesn't cover the whole
    // window.
    viewerWindow->updateCamera();
    viewerWindow->setCameraManipulator(new osgGA::TrackballManipulator);
    viewerWindow->setSceneData(loadedModel.get());

    viewerWindow->show();

    setupManipulatorAndHandler(*viewerWindow.get(), arguments);

    a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );

    return a.exec();
}

int mainQOSGWidget(QApplication& a, osg::ArgumentParser& arguments)
{
	osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFiles(arguments);

    if (!loadedModel)
    {
        std::cout << arguments[0] <<": No data loaded." << std::endl;
        return 1;
    }

    std::cout<<"Using QOSGWidget - QWidget + osgViewer creating the graphics context."<<std::endl;

    if (arguments.read("--CompositeViewer"))
    {
        osg::ref_ptr<CompositeViewerQOSG> viewerWindow(new CompositeViewerQOSG);

        viewerWindow->setGeometry(50,50,640,480);
        // Open the ViewerQOSG window at 30/30 instead of 0/0.  In some instances,
        // the window may otherwise lack any window decoration.
        // viewerWindow->setGeometry(30,30,640,480);

        unsigned int width = viewerWindow->width();
        unsigned int height = viewerWindow->height();

        {
            osgViewer::View* view1 = new osgViewer::View;
            view1->getCamera()->setGraphicsContext(viewerWindow->getGraphicsWindow());
            view1->getCamera()->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(width)/static_cast<double>(height/2), 1.0, 1000.0);
            view1->getCamera()->setViewport(new osg::Viewport(0,0,width,height/2));
            view1->setSceneData(loadedModel.get());
            view1->getCamera()->setClearColor( osg::Vec4( 0.08, 0.08, 0.5, 1.0 ) );

            setupManipulatorAndHandler(*view1, arguments);

            viewerWindow->addView(view1);
        }

        {
            osgViewer::View* view2 = new osgViewer::View;
            view2->getCamera()->setGraphicsContext(viewerWindow->getGraphicsWindow());
            view2->getCamera()->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(width)/static_cast<double>(height/2), 1.0, 1000.0);
            view2->getCamera()->setViewport(new osg::Viewport(0,height/2,width,height/2));
            view2->setSceneData(loadedModel.get());
            view2->getCamera()->setClearColor( osg::Vec4( 0.08, 0.08, 0.5, 1.0 ) );

            setupManipulatorAndHandler(*view2, arguments);
		
            viewerWindow->addView(view2);
        }

        viewerWindow->_scene = loadedModel.get();
        viewerWindow->Tile();
        viewerWindow->show();

        a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );

        return a.exec();
    }
#if USE_QT4
    else if (arguments.read("--MTCompositeViewer"))
    {

        std::cout <<" + using standard CompositeViewer with seperate contexts (Multithreaded mode / EXPERIMENTAL !)" <<std::endl;

        int nbCowsX = 3;
        int nbCowsY = 2;
        int size = 300;
        unsigned int width = nbCowsX * size;
        unsigned int height = nbCowsY * size;

        QGridLayout *uiLayout = new QGridLayout ();
        QWidget *wy = new QWidget ();

        // the timer holds an instance of osgViewer::CompositeViewer
        // NOTE: this is a workaround since we're not using QT's moc precompiler here..
        QViewerTimer *ctimer = new QViewerTimer ();

        for (int x=0;x<nbCowsX; x++)
            for (int y=0;y<nbCowsY; y++)
            {

                // embed the QOSGWidget into QGroupBox to demonstrate that we
                // really use QT's Widgets
                //
                std::stringstream widgetname; widgetname << "View (" << x << "," << y << ")";
                QGroupBox *w= new QGroupBox (QString (widgetname.str ().c_str ()), wy);
                QGridLayout *tmpl = new QGridLayout ();
                QOSGWidget *gw = new QOSGWidget (w, 0, 0, true);
                tmpl->addWidget (gw);
                w->setLayout(tmpl);
                uiLayout->addWidget (w, y, x);

                // setup views as usual
                osgViewer::View* view = new osgViewer::View;
                view->getCamera()->setGraphicsContext(gw->getGraphicsWindow ());
                view->getCamera()->setProjectionMatrixAsPerspective
                    (30.0f, static_cast<double>(width*2)/static_cast<double>(height), 1.0, 1000.0);
                view->getCamera()->setViewport(new osg::Viewport(0,0,size,size));
                view->addEventHandler(new osgViewer::StatsHandler);
                view->setCameraManipulator(new osgGA::TrackballManipulator);
                view->setSceneData(loadedModel.get ());
                ctimer->_viewer->addView(view);

            }

        //uiLayout->addWidget (ctimer);
        wy->setLayout (uiLayout);
        wy->resize (width, height);
        wy->show ();

        // we need the timer to be visible for repaints
        // NOTE: this is a workaround since we're not using QT's moc precompiler here..
        ctimer->resize (1,1);
        ctimer->show ();

        a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );

        return a.exec();
    }
#endif
    else
    {
        osg::ref_ptr<ViewerQOSG> viewerWindow(new ViewerQOSG);

        // Open the ViewerQOSG window at 30/30 instead of 0/0.  In some instances,
        // the window may otherwise lack any window decoration.
        // viewerWindow->setGeometry(0,0,640,480);
        viewerWindow->setGeometry(30,30,640,480);

        // Setup the camera only after ViewerQOSG's Qt base class has been
        // initialized. Without this change the view doesn't cover the whole
        // window.
        viewerWindow->updateCamera();
        viewerWindow->setCameraManipulator(new osgGA::TrackballManipulator);
        viewerWindow->setSceneData(loadedModel.get());

        viewerWindow->show();

        setupManipulatorAndHandler(*viewerWindow.get(), arguments);

        a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );

        return a.exec();
    }
}