#!/usr/bin/env python
#/*******************************************************************/
#/*                               XDMF                              */
#/*                   eXtensible Data Model and Format              */
#/*                                                                 */
#/*  Id : Id  */
#/*  Date : $Date$ */
#/*  Version : $Revision$ */
#/*                                                                 */
#/*  Author:                                                        */
#/*     Jerry A. Clarke                                             */
#/*     clarke@arl.army.mil                                         */
#/*     US Army Research Laboratory                                 */
#/*     Aberdeen Proving Ground, MD                                 */
#/*                                                                 */
#/*     Copyright @ 2002 US Army Research Laboratory                */
#/*     All Rights Reserved                                         */
#/*     See Copyright.txt or http://www.arl.hpc.mil/ice for details */
#/*                                                                 */
#/*     This software is distributed WITHOUT ANY WARRANTY; without  */
#/*     even the implied warranty of MERCHANTABILITY or FITNESS     */
#/*     FOR A PARTICULAR PURPOSE.  See the above copyright notice   */
#/*     for more information.                                       */
#/*                                                                 */
#/*******************************************************************/
# import mpi, sys
import mpi
import sys
import string

# print 'Loading Xdmf'
import Xdmf

from XdmfGridPart import *
# print 'Loading vtk'
from vtk import *
from libvtkXdmfPython import *


class XdmfIsoViewer :

	def __init__( self, controller,  DOM, node ):
		self.controller = controller
		# print 'Reading Heavy Data'
		Grid = Xdmf.XdmfGrid()
		Grid.SetDOM( DOM )
		Grid.SetGridFromElement( node )
		Grid.AssignAttributeByName('Volume Fraction for air')
		VtkGrid = vtkDataObject( Grid.GetBaseGrid() )


		NodeFilter  = vtkCellDataToPointData()
		NodeFilter.SetInput( VtkGrid )
		# print 'Update NodeFilter'
		NodeFilter.Update()

		# Take Ghost Cells into Account
		# If it's a Rectilinear Grid Change
		# it to a Structured Grid since Extract Grid doesn't
		# handle Rectilinear Grids.
		WholeGrid = NodeFilter.GetOutput()
		# print 'Before: Node #' + str( mpi.rank ) + ' ' + str( WholeGrid.GetExtent() )
		i1, i2, j1, j2, k1, k2 = WholeGrid.GetExtent()
		if mpi.rank == 0 :
			k2 -= 1
		elif mpi.rank == ( mpi.procs - 1 ) :
			k1 += 1
		else :
			k1 += 1
			k2 -= 1
		OuptutType = WholeGrid.GetClassName()
		if OuptutType == 'vtkRectilinearGrid' :
			RGExtract = vtkRectilinearGridGeometryFilter()
			RGExtract.SetInput( NodeFilter.GetOutput() )
			RGExtract.SetExtent( i1, i2, j1, j2, k1, k2 )
			RGExtract.Update()
			Points 	= RGExtract.GetOutput().GetPoints()

			SG = vtkStructuredGrid()
			SG.SetDimensions( 1 + i2 - i1, 1 + j2 - j1, 1 + k2 - k1 )
			SG.SetPoints( Points )
			SG.GetPointData().SetScalars( RGExtract.GetOutput().GetPointData().GetScalars() )
			SG.Update()
		else :
			Extract = vtkExtractGrid()
			Extract.SetInput( WholeGrid )
			Extract.SetExtent( i1, i2, j1, j2, k1, k2 )
			Extract.Update()
			SG = Extract.GetOutput()	


		
		CoutourFilter = vtkContourFilter()
		CoutourFilter.SetInput( SG )
		# print 'After : Node #' + str( mpi.rank ) + ' ' + str( SG.GetExtent() )
		CoutourFilter.SetValue( 0, 0.51 )
		# print 'Update Iso'
		CoutourFilter.Update()
		self.Output = CoutourFilter.GetOutput()
			

	def DisconnectOutput( self ) :
		controller = self.controller
		if mpi.rank == 0 :
			for Id in range(1, mpi.procs) :
				BREAK_RMI_TAG = 239954
				controller.TriggerRMI( Id, BREAK_RMI_TAG )

	def ConnectOutput( self ) :
		controller = self.controller
		myId = controller.GetLocalProcessId()
		if mpi.rank == 0 :
			self.PolyData = vtkAppendPolyData()
			self.PolyData.AddInput( self.Output )
			for Id in range(1, mpi.procs) :
				print 'Connect to Proc #' + str( Id )
				downStreamPort = vtkInputPort()
				downStreamPort.SetController(controller)
				downStreamPort.SetRemoteProcessId( Id )
				downStreamPort.SetTag( 10 )
				downStreamPort.Update()
				data  = downStreamPort.GetPolyDataOutput()
				self.PolyData.AddInput( data )
		else :
			upStreamPort = vtkOutputPort()
			upStreamPort.SetController(controller)
			upStreamPort.SetInput(self.Output)
			upStreamPort.SetTag( 10 )
			upStreamPort.WaitForUpdate()


	def Show( self ) :
		Ren = vtkRenderer()
		self.Ren = Ren
		Mapper = vtkPolyDataMapper()
		Mapper.SetInput( self.PolyData.GetOutput() )

		Actor = vtkActor()
		Actor.SetMapper( Mapper )
		# Actor.GetProperty().SetRepresentationToWireframe()

		Ren.AddActor( Actor )
		RenWin = vtkRenderWindow()
		RenWin.AddRenderer(self.Ren)

		Style = vtkInteractorStyleTrackball()
		Style.SetTrackballModeToTrackball()
		iRen = vtkRenderWindowInteractor()
		iRen.SetInteractorStyle( Style )
		iRen.SetRenderWindow(RenWin)

		RenWin.SetSize(500,500)
		self.Ren.SetBackground(1, 1, 1)

		iRen.Initialize()
		iRen.Start()

if __name__ == '__main__' :
	controller = vtkXdmfMPIController()
	controller.FakeInitialize()

	argc = len( sys.argv )
	FileName = sys.argv[ argc - 1 ]
	print 'Parsing ' + FileName
	DOM = Xdmf.XdmfDOM()
	DOM.SetInputFileName( FileName )
	DOM.Parse()
	GridNode = DOM.FindElement("Grid")
	Partioner = XdmfGridPart( DOM )
	kdim, jdim, idim = Partioner.Setup( GridNode )
	# Slice Along K
	istart = 0
	iend = idim - 1
	jstart = 0
	jend = jdim - 1
	nplanes = kdim / mpi.procs
	kstart = mpi.rank * nplanes
	kend = kstart + nplanes
	if mpi.rank == 0 :
		kend += 1
	elif mpi.rank == ( mpi.procs - 1 ) :
		kstart -= 1
		kend = kdim - 1
	else :
		kstart -= 1
		kend += 1
	print 'Node ' + str(mpi.rank) + ' %d %d' % ( kstart, kend )
	Partioner.SetExtent( kstart, kend, jstart, jend, istart, iend )
	Partioner.Transform( GridNode )
	fd = open('Try.%d' % mpi.rank, 'w')
	XML = DOM.Serialize( GridNode )
	fd.write( XML )
	fd.close()
	viewer = XdmfIsoViewer( controller, DOM, GridNode )
	viewer.ConnectOutput()
	if mpi.rank == 0 :
		viewer.Show()
		viewer.DisconnectOutput()
	print 'Node #' + str( mpi.rank ) +  ' is back'
	controller.Finalize()
