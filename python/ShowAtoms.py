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

import sys
import string

# Load Xdmf
import Xdmf
# Load vtk
from vtk import *
from libvtkXdmfPython import *
import vtkRenderWidget

import Tkinter
import vtkPipeline.vtkPipeline

class ShowAtoms :

	def __init__(self, FileName ) :
		self.FileName = FileName
		self.ImageCntr = 0
		Reader = vtkXdmfReader()
		Reader.SetInputFileName( FileName )
		Reader.Initialize()
		Reader.SetAllAttributeStatusOn()
		Reader.Update()

		Sphere = vtkSphereSource()
		Sphere.SetPhiResolution( 10 )
		Sphere.SetThetaResolution( 10 )
		Sphere.SetRadius(0.25)
		SphereGlyph = vtkGlyph3D()
		SphereGlyph.SetSource( 0, Sphere.GetOutput() )
		SphereGlyph.SetInput( Reader.GetOutput())
		SphereGlyph.OrientOff()
		SphereGlyph.SetColorMode( 0 )
		SphereGlyph.SetVectorModeToUseVector()
		SphereGlyph.SetColorModeToColorByVector()
		SphereGlyph.ScalingOff()
		SphereGlyph.Update()

		Arrow = vtkArrowSource()
		Arrow.SetShaftResolution( 15 )
		Arrow.SetTipResolution( 15 )
		ArrowGlyph = vtkGlyph3D()
		ArrowGlyph.SetSource( 0, Arrow.GetOutput() )
		ArrowGlyph.SetInput( Reader.GetOutput())
		ArrowGlyph.OrientOn()
		ArrowGlyph.SetVectorModeToUseVector()
		ArrowGlyph.SetColorModeToColorByVector()
		ArrowGlyph.ScalingOff()
		ArrowGlyph.Update()

		ScalarRange = ArrowGlyph.GetOutput().GetScalarRange()
		print 'Displacement Range = ' + str(ScalarRange)

		Mapper = vtkPolyDataMapper()
		Mapper.SetInput( ArrowGlyph.GetOutput())
		Mapper.SetScalarRange( ScalarRange )
		Mapper.GetLookupTable().SetHueRange(.667, 0)

		Actor = vtkActor()
		Actor.SetMapper( Mapper )

		SphereMapper = vtkPolyDataMapper()
		SphereMapper.SetInput( SphereGlyph.GetOutput())
		SphereMapper.ScalarVisibilityOff()

		SphereActor = vtkActor()
		SphereActor.SetMapper( SphereMapper )
		SphereActor.GetProperty().SetColor(.3, .3, .3)

		Ren = vtkRenderer()
		self.Ren = Ren
		Ren.SetBackground(1, 1, 1)
		Ren.AddActor( Actor )
		Ren.AddActor( SphereActor )

		# self.AddLegend( Mapper.GetLookupTable(), .1, .1 )

		self.SetUpPipeline(Ren)

	def AddLegend( self, ColorTable, X, Y ) :
		Legend = vtkScalarBarActor()
		Legend.GetProperty().SetColor( 0, 0, 1 )
		Legend.SetLookupTable( ColorTable )
		Legend.SetTitle( """Displacement""")
		Legend.SetFontFamilyToTimes()
		Legend.SetHeight( .4 )
		Legend.SetWidth( .05 )
		Legend.SetMaximumNumberOfColors( 64 )
		Legend.SetPosition( X, Y )
		self.Ren.AddActor2D( Legend )
		
	def DumpImage( self ) :
		ImageFile = os.path.splitext( self.FileName )
		ImageFile = ImageFile[0] + '_%04d' % self.ImageCntr + '.tif'
		print 'Writing ' + ImageFile
		Image = vtkWindowToImageFilter()
		Image.SetInput( self.RenWin )
		Writer = vtkTIFFWriter()
		Writer.SetInput( Image.GetOutput())
		Writer.SetFileName( ImageFile )
		Writer.Write()
		self.ImageCntr += 1
		
		

	def Done(self) :
		self.exit = 1

	def SetUpPipeline( self, Ren ) :
		root = Tkinter.Tk()
		root.title("Atom Viewer")
		wid = vtkRenderWidget.vtkTkRenderWidget (root, width=500, height=500)
		wid.pack (expand='true', fill='both')
		wid.bind ("<KeyPress-q>", lambda e=None: self.Done() )
		wid.bind ("<KeyPress-i>", lambda e=None: self.DumpImage() )
		renWin = wid.GetRenderWindow()
		self.RenWin = renWin
		renWin.AddRenderer(Ren)
		renWin.SetSize(500,500)
		renWin.Render ()

		pipe = vtkPipeline.vtkPipeline.vtkPipelineBrowser (root, renWin)
		pipe.browse ()
		# root.mainloop ()
		self.exit = -1
		while self.exit < 0 :
			Tkinter.tkinter.dooneevent( 0 )

if __name__ == '__main__' :
	argc = len( sys.argv )
	viewer = ShowAtoms( sys.argv[ argc - 1 ] )
		
