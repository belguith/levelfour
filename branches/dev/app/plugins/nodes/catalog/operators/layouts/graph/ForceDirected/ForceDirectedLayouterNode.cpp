/*
-----------------------------------------------------------------------------
file "ForceDirectedLayouterNode.cpp"
brief Implementation file for ForceDirectedLayouterNode class.
*/

#include "ForceDirectedLayouterNode.h"
#include "VTKTableParameter.h"

INIT_INSTANCE_COUNTER(ForceDirectedLayouterNode)

//!
//! Constructor of the ForceDirectedLayouterNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
ForceDirectedLayouterNode::ForceDirectedLayouterNode ( const QString &name, ParameterGroup *parameterRoot ) :
    VTKGraphLayoutNode(name, parameterRoot)
{
	setTypeName("ForceDirectedLayouterNode");

	m_layoutInstance = vtkForceDirectedLayoutStrategy::New();

	// These properties relate to graph layout
	// None

	// These properties relate specifically to the layout strategy
	setChangeFunction("Set Three Dimensional Layout", SLOT(setThreeDimensionalLayout()));
    setCommandFunction("Set Three Dimensional Layout", SLOT(setThreeDimensionalLayout()));

	setChangeFunction("Set Random Seed", SLOT(setRandomSeed()));
    setCommandFunction("Set Random Seed", SLOT(setRandomSeed()));

	setChangeFunction("Set Max Number Of Iterations", SLOT(setMaxNumberOfIterations()));
    setCommandFunction("Set Max Number Of Iterations", SLOT(setMaxNumberOfIterations()));

	setChangeFunction("Set Iterations Per Layout", SLOT(setIterationsPerLayout()));
    setCommandFunction("Set Iterations Per Layout", SLOT(setIterationsPerLayout()));

	setChangeFunction("Set Initial Temperature", SLOT(setInitialTemperature()));
    setCommandFunction("Set Initial Temperature", SLOT(setInitialTemperature()));

	setChangeFunction("Set Cool Down Rate", SLOT(setCoolDownRate()));
    setCommandFunction("Set Cool Down Rate", SLOT(setCoolDownRate()));

	setChangeFunction("Set Random Initial Points", SLOT(setRandomInitialPoints()));
    setCommandFunction("Set Random Initial Points", SLOT(setRandomInitialPoints()));

	INC_INSTANCE_COUNTER
}


//!
//! Destructor of the ForceDirectedLayouterNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
ForceDirectedLayouterNode::~ForceDirectedLayouterNode ()
{
	emit destroyed();
    DEC_INSTANCE_COUNTER
    Log::info(QString("ForceDirectedLayouterNode destroyed."), "ForceDirectedLayouterNode::~ForceDirectedLayouterNode");
}

//!
//! Set the layout properties
//!
void ForceDirectedLayouterNode::setThreeDimensionalLayout ()
{
	M_LAYOUT->SetThreeDimensionalLayout(m_threeD);
}

void ForceDirectedLayouterNode::setRandomSeed ()
{
	M_LAYOUT->SetRandomSeed(m_randomSeed);
}

void ForceDirectedLayouterNode::setMaxNumberOfIterations ()
{
	M_LAYOUT->SetMaxNumberOfIterations(m_maxIterations);
}

void ForceDirectedLayouterNode::setIterationsPerLayout ()
{
	M_LAYOUT->SetIterationsPerLayout(m_layoutIterations);
}

void ForceDirectedLayouterNode::setInitialTemperature ()
{
	M_LAYOUT->SetInitialTemperature(m_initialTemperature);
}

void ForceDirectedLayouterNode::setCoolDownRate ()
{
	M_LAYOUT->SetCoolDownRate(m_coolDownRate);
}

void ForceDirectedLayouterNode::setRandomInitialPoints ()
{
	M_LAYOUT->SetRandomInitialPoints(m_randomPoints);
}

void ForceDirectedLayouterNode::processParameters()
{
	if (!updateInputGraph())
		return;
	
	refreshOutput();
}