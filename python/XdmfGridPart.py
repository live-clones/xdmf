#!/bin/env python
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
import Xdmf

# from vtk import *

class XdmfGridPart :

	def __init__( self, DOM ) :
		self.DOM = DOM 

	def SetExtent( self, KStart, KEnd, JStart, JEnd, IStart, IEnd ) :
		self.KStart = KStart
		self.KEnd = KEnd
		self.JStart = JStart
		self.JEnd = JEnd
		self.IStart = IStart
		self.IEnd = IEnd

	def Setup( self, GridNode ) :
		DOM = self.DOM
		Topology = DOM.FindElement( 'Topology', 0, GridNode )
		DimString = DOM.Get( Topology, 'Dimensions')
		kdim, jdim, idim = string.split( DimString )
		self.KDim = int( kdim )
		self.JDim = int( jdim )
		self.IDim = int( idim )
		return( self.KDim, self.JDim, self.IDim )
		
	def TransformAttributes( self, GridNode ) :
		DOM = self.DOM
		kdim = self.KEnd - self.KStart + 1
		jdim = self.JEnd - self.JStart + 1
		idim = self.IEnd - self.IStart + 1
		Index = 0
		Attribute = DOM.FindElement( 'Attribute', Index, GridNode )
		while Attribute != None :
			Name = string.upper(DOM.Get( Attribute, 'Name'))
			Center = string.upper(DOM.Get( Attribute, 'Center'))
			data = DOM.FindElement( 'DataStructure', 0, Attribute )
			DimList = string.split( DOM.Get( data, 'Dimensions' ) )
			Ndims = len( DimList )
			if Center == 'NODE' :
				DimString = "%d %d %d" % ( kdim, jdim, idim )
				if Ndims > 3 :
					DimString += ' ' + string.join( DimList[3:] )
				# Start
				HyperSlab = "%d %d %d " % ( self.KStart, self.JStart, self.IStart )
				if Ndims > 3 :
					for i in range( Ndims - 3 ) :
						HyperSlab += " 0 "
				# Stride
				for i in range( Ndims ) :
					HyperSlab += " 1 "
				# Count
				HyperSlab += "%d %d %d " % ( kdim, jdim, idim )
				if Ndims > 3 :
					for i in range( Ndims - 3 ) :
						HyperSlab += " %d " % DimList[i]
				HyperSlab += '\n'
			else :
				DimString = "%d %d %d" % ( kdim - 1, jdim - 1, idim - 1 )
				if Ndims > 3 :
					DimString += ' ' + string.join( DimList[3:] )
				# Start
				HyperSlab = "%d %d %d " % ( self.KStart, self.JStart, self.IStart )
				if Ndims > 3 :
					for i in range( Ndims - 3 ) :
						HyperSlab += " 0 "
				# Stride
				for i in range( Ndims ) :
					HyperSlab += " 1 "
				# Count
				HyperSlab += "%d %d %d " % ( kdim - 1, jdim - 1, idim - 1 )
				if Ndims > 3 :
					for i in range( Ndims - 3 ) :
						HyperSlab += " %d " % DimList[i]
				HyperSlab += '\n'
			# Patch Data

			XML = '<DataTransform Type="HyperSlab" Dimensions="' + DimString + '">\n'
			XML += '<DataStructure Dimensions="3 ' + str(Ndims)+ '" Format="XML">\n'
			XML += HyperSlab
			XML += """</DataStructure>"""
			XML += DOM.Serialize( data )
			XML += """</DataTransform>\n"""
			DOM.InsertFromString( Attribute, XML )
			DOM.DeleteNode( data )
			Index += 1
			Attribute = DOM.FindElement( 'Attribute', Index, GridNode )
	
	def TransformGeometry( self, GridNode ) :
		DOM = self.DOM
		kdim = self.KEnd - self.KStart + 1
		jdim = self.JEnd - self.JStart + 1
		idim = self.IEnd - self.IStart + 1
		Topology = DOM.FindElement( 'Topology', 0, GridNode )
		DOM.Set( Topology, "Dimensions", "%d %d %d" % ( kdim, jdim, idim ) )
		Geometry = DOM.FindElement( 'Geometry', 0, GridNode )
		Type = string.upper(DOM.Get( Geometry, 'Type'))
		print 'Patching ' + Type + ' Geometry'
		if Type == 'ORIGIN_DXDYDZ' :
			OriginNode = DOM.FindElement( 'DataStructure', 0, Geometry )
			Origin = string.split( DOM.Get( OriginNode, 'CData' ) )
			OriginX = float( Origin[0] )
			OriginY = float( Origin[1] )
			OriginZ = float( Origin[2] )
			# print 'OldOrigin = %f %f %f' % ( OriginX, OriginY, OriginZ )
			DxdydzNode = DOM.FindElement( 'DataStructure', 1, Geometry )
			Dxdydz = string.split( DOM.Get( DxdydzNode, 'CData' ) )
			DxdydzX = float( Dxdydz[0] )
			DxdydzY = float( Dxdydz[1] )
			DxdydzZ = float( Dxdydz[2] )
			# print 'OldDxdydz = %f %f %f' % ( DxdydzX, DxdydzY, DxdydzZ )
			OriginX = ( DxdydzX * self.IStart ) + OriginX
			OriginY = ( DxdydzY * self.JStart ) + OriginY
			OriginZ = ( DxdydzZ * self.KStart ) + OriginZ
			# print 'NewOrigin = %f %f %f' % ( OriginX, OriginY, OriginZ )
			DOM.Set( OriginNode, 'CData', '%f %f %f' % ( OriginX, OriginY, OriginZ ))
		if Type == 'VXVYVZ' :
			vx = DOM.FindElement( 'DataStructure', 0, Geometry )
			vy = DOM.FindElement( 'DataStructure', 1, Geometry )
			vz = DOM.FindElement( 'DataStructure', 2, Geometry )
			# Patch X 
			XML = """<DataTransform Type="HyperSlab" Dimensions="%d">\n""" % idim
			XML += """<DataStructure Dimensions="3 1" Format="XML">\n"""
			XML += '%d 1 %d\n' % ( self.IStart, idim )
			XML += """</DataStructure>"""
			XML += DOM.Serialize( vx )
			XML += """</DataTransform>\n"""
			DOM.InsertFromString( Geometry, XML )
			# Patch Y 
			XML = """<DataTransform Type="HyperSlab" Dimensions="%d">\n""" % jdim
			XML += """<DataStructure Dimensions="3 1" Format="XML">\n"""
			XML += '%d 1 %d\n' % ( self.JStart, jdim )
			XML += """</DataStructure>"""
			XML += DOM.Serialize( vy )
			XML += """</DataTransform>\n"""
			DOM.InsertFromString( Geometry, XML )
			# Patch Z 
			XML = """<DataTransform Type="HyperSlab" Dimensions="%d">\n""" % kdim
			XML += """<DataStructure Dimensions="3 1" Format="XML">\n"""
			XML += '%d 1 %d\n' % ( self.KStart, kdim )
			XML += """</DataStructure>"""
			XML += DOM.Serialize( vz )
			XML += """</DataTransform>\n"""
			DOM.InsertFromString( Geometry, XML )
			DOM.DeleteNode( vx )
			DOM.DeleteNode( vy )
			DOM.DeleteNode( vz )

	def Transform( self, GridNode ) :
		self.TransformGeometry( GridNode )
		self.TransformAttributes( GridNode )

if __name__ == '__main__' :
	argc = len( sys.argv )
	FileName = sys.argv[ argc - 7 ]
	kstart = int( sys.argv[ argc - 6 ] )
	kend = int( sys.argv[ argc - 5 ] )
	jstart = int( sys.argv[ argc - 4 ] )
	jend = int( sys.argv[ argc - 3 ] )
	istart = int( sys.argv[ argc - 2 ] )
	iend = int( sys.argv[ argc - 1 ] )
	fd = open( FileName )
	XML = fd.read()
	fd.close;

	DOM = Xdmf.XdmfDOM()
	DOM.Parse( XML )
	GridNode = DOM.FindElement( 'Grid' )
	Partioner = XdmfGridPart( DOM )
	kdim, jdim, idim = Partioner.Setup( GridNode )
	print "Dims = %d %d %d" % ( idim, jdim, kdim )
	Partioner.SetExtent( kstart, kend, jstart, jend, istart, iend )
	Partioner.Transform( GridNode )
	print DOM.Serialize( None )
	
