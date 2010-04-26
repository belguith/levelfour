//!
//! \file "VTKGraphLayoutNode.h"
//! \brief Header file for VTKGraphLayoutNode class.
//!
//! \version    1.0
//! \date       18.05.2009 (last updated)
//!

#ifndef VTKGraphLayoutNode_H
#define VTKGraphLayoutNode_H

#include "Node.h"
#include "vtkGraph.h"
#include "vtkGraphLayoutStrategy.h"
#include "vtkTree.h"
#include "vtkTable.h"
#include <QString>


//!
//! Class for a VTKGraphLayoutNode
//!
class FRAPPER_CORE_EXPORT VTKGraphLayoutNode : public Node
{

    Q_OBJECT
    ADD_INSTANCE_COUNTER

public: // constructors and destructors

    //!
    //! Constructor of the VTKGraphLayoutNode class.
    //!
    //! \param name The name for the new node.
    //! \param parameterRoot A copy of the parameter tree specific for the type of the node.
    //!
    VTKGraphLayoutNode ( const QString &name, ParameterGroup *parameterRoot );

    //!
    //! Destructor of the VTKGraphLayoutNode class.
    //!
    //! Defined virtual to guarantee that the destructor of a derived class
    //! will be called if the instance of the derived class is saved in a
    //! variable of its parent class type.
    //!
    virtual ~VTKGraphLayoutNode ();

protected slots: //

    //!
    //! Processes the node's input data to generate the node's output table.
    //!
    void processOutputVTKTable();

    //!
    //! Update the input graph (called for example when input parameters have been dirtied)
    //! return It returns 0 if it succeeds, otherwise 1
    //!
    int updateInputGraph ();

protected: // functions

    //!
    //! Create a vtkTable with ID, x, y, z from an vtk graph 
    //!
	vtkTable * createTableFromGraph(vtkGraph *graph);


protected: // data 
	//!
    //! The graph layout algorithm to be used (circular, radial, etc)
    //!
	vtkGraphLayoutStrategy * m_layoutInstance;

protected: // data

    //!
    //! The name of the input vtk graph parameter.
    //!
    QString m_inputVTKGraphName;

    //!
    //! The name of the output vtk table parameter.
    //!
    QString m_ouputVTKTableParameterName;
	
	//!
    //! The output Table
    //!
    vtkTable * m_outputTable;

	//!
    //! The input graph
    //!
    vtkGraph * m_inGraph;
};


#endif