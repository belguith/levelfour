//!
//! \file "TableToGraphNode.h"
//! \brief Header file for TableToGraphNode class.
//!
//! \version    1.0
//! \date       18.05.2009 (last updated)
//!

#ifndef TableToGraphNODE_H
#define TableToGraphNODE_H

#include "Node.h"
#include "vtkGraph.h"
#include "vtkTable.h"
#include "vtkTableToGraph.h"

//!
//! Class for a Degree Centrality Measure
//!
class TableToGraphNode : public Node
{

    Q_OBJECT
    ADD_INSTANCE_COUNTER

public: // constructors and destructors

    //!
    //! Constructor of the TableToGraphNode class.
    //!
    //! \param name The name for the new node.
    //! \param parameterRoot A copy of the parameter tree specific for the type of the node.
    //!
	TableToGraphNode ( const QString &name, ParameterGroup *parameterRoot );

    //!
    //! Destructor of the TableToGraphNode class.
    //!
    //! Defined virtual to guarantee that the destructor of a derived class
    //! will be called if the instance of the derived class is saved in a
    //! variable of its parent class type.
    //!
    virtual ~TableToGraphNode ();

private slots: //

    //!
    //! Processes the node's input data to generate the node's output table.
    //!
    virtual void processOutputVTKGraph ();

    //!
    //! Update the graph (called for example when parameters change)
    //!
    virtual void updateGraph ();

    //!
    //! Update the input table 
    //! \return True if it succeeds, otherwise false
    //!
    bool updateTable ();

private: 
    //!
    //! The name of the input vtk table parameter.
    //!
    QString m_inputVTKTableParameterName;

    //!
    //! The name of the output vtk Graph parameter.
    //!
    QString m_outputVTKGraphName;

    //!
    //! The parameters that defines the edgeFrom/To and vertices for the graph.
    //!
	EnumerationParameter * edgesFromParameter, * edgesToParameter, * vertexIDParameter;
	
    //!
    //! The input table 
    //!
	vtkTable * m_inputTable;

	//!
    //! The resulting graph
    //!
	vtkGraph * m_graph;

	vtkTableToGraph * tableToGraph;
};


#endif
