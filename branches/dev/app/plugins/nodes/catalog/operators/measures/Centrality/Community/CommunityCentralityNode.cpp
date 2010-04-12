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

/*
-----------------------------------------------------------------------------
file "CommunityCentralityNode.cpp"
brief Implementation file for CommunityCentralityNode class.

version    1.0
date       18.05.2009 (last updated)

Description
-----------
 The larger the centrality value is, the more influence the node can give in forming community. 
 Nodes with high community centrality play central role in their local neighborhood.

 Closeness centrality at a node is 1/average distance to all 
 other nodes.

Parameters
----------
    G : graph
      A graph 

    vids:
	  Vector giving the vertices for which the closeness centrality scores will be computed. 
	
	weighted_edges : bool, optional
      Consider the edge weights in determining the shortest paths.
      If False, all edge weights are considered equal.

    modes:
      The type of shortest paths to be used for the calculation in directed graphs. Possible values:

		OUT: the lengths of the outgoing paths are calculated.

		IN: the lengths of the incoming paths are calculated.

		ALL: the directed graph is considered as an undirected one for the computation. 
      
Returns
-------
nodes : dictionary
 Dictionary of nodes with closeness centrality as the value.

 Community Centrality
    
Notes
-----
 The closeness centrality is normalized to to n-1 / size(G)-1 where 
 n is the number of nodes in the connected part of graph containing
 the node.  If the graph is not completely connected, this
 algorithm computes the closeness centrality for each connected
 part separately.  

Reference
---------
 
 Finding community structure in networks using the eigenvectors of matrices, M. E. J. Newman, Phys. Rev. E 74, 036104 (2006). 


-----------------------------------------------------------------------------
*/

#include "CommunityCentralityNode.h"


///
/// Constructors and Destructors
///


//!
//! Constructor of the CommunityCentralityNode class.
//!
//! \param name The name for the new node.
//! \param parameterRoot A copy of the parameter tree specific for the type of the node.
//!
CommunityCentralityNode::CommunityCentralityNode ( const QString &name, ParameterGroup *parameterRoot ) :
    Node(name, parameterRoot)
{
}


//!
//! Destructor of the CommunityCentralityNode class.
//!
//! Defined virtual to guarantee that the destructor of a derived class
//! will be called if the instance of the derived class is saved in a
//! variable of its parent class type.
//!
CommunityCentralityNode::~CommunityCentralityNode ()
{
}


