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

"""PLY Format Converter"""


import string
import time
import colorsys
from IceParser import *
from Xdmf import *

class PlyParser( IceParser ) :

	def __init__( self, Filename='Data.ply' ) :
		self.DoConvert = 0
		IceParser.__init__( self, Filename )

	def ReadElements( self ) :
		self.GetNextLine()
		self.NodesPerElement = int(self.CurrentWords[0])
		self.ElementArray = XdmfArray()
		self.ElementArray.SetNumberType( XDMF_INT32_TYPE )
		self.ElementArray.SetShapeFromString( self.Elements + ' ' + str(self.NodesPerElement) )
		self.ElementArray.SetValues( 0, string.join( self.CurrentWords[1:] ))	
		Index = self.NodesPerElement
		Cntr = 0
		# One has already been read
		NumberOfElements = int( self.Elements ) - 1
		while NumberOfElements > 0 :
			self.GetNextLine()
			self.ElementArray.SetValues( Index, string.join( self.CurrentWords[1:] ))	
			if Cntr >= 5000 :
				print 'Read ' + str( Index / self.NodesPerElement ) + ' Elements'
				Cntr = 0
			Index += self.NodesPerElement
			Cntr += 1
			NumberOfElements -= 1

	def ReadNodes ( self ) :
		NumberOfNodes = int( self.Nodes )
		NumberOfElements = int( self.Elements )
		self.NodeArray = XdmfArray()
		self.NodeArray.SetNumberType( XDMF_FLOAT32_TYPE )
		self.NodeArray.SetShapeFromString( self.Nodes + ' 3' )
		self.ExtraArray = None
		self.FakeColorArray = None
		Extra = len( self.Props[3:] )
		if Extra > 0 :
			# self.ExtraArray = XdmfArray()
			# self.ExtraArray.SetNumberType( XDMF_FLOAT32_TYPE )
			# self.ExtraArray.SetShapeFromString( self.Nodes + ' ' + str( Extra ) )
			if self.DoConvert == 1 :
				self.FakeColorArray = XdmfArray()
				self.FakeColorArray.SetNumberType( XDMF_FLOAT32_TYPE )
				self.FakeColorArray.SetShapeFromString( self.Nodes )
		NodeIndex = 0
		ExtraIndex = 0
		FakeColorIndex = 0
		Cntr = 0
		Length = len( self.Props ) - 3
		while NumberOfNodes > 0 :
			self.GetNextLine()
			self.NodeArray.SetValues( NodeIndex, string.join( self.CurrentWords[:3] ))	
			if Extra > 0 :
				# self.ExtraArray.SetValues( ExtraIndex, string.join( self.CurrentWords[3:]))
				if self.DoConvert == 1 :
					r, g, b = self.CurrentWords[-3:]
					# RRRR GGGG BBBB
					# red = ( int( r ) & 0xE0 ) 
					# green = ( int( g ) & 0xE0 ) >> 3
					# blue  = ( int( b ) & 0xC0 )  >> 6
					red = ( int( r ) & 0xF0 )  << 4
					green = ( int( g ) & 0xF0 )
					blue  = ( int( b ) & 0xF0 )  >> 4
					FakeColor = red | green | blue
					self.FakeColorArray.SetValueFromInt64( FakeColorIndex, FakeColor )
					FakeColorIndex += 1
			if Cntr >= 5000 :
				print 'Read ' + str( NodeIndex / 3 ) + ' Nodes'
				Cntr = 0
			ExtraIndex += Length
			NodeIndex += 3
			Cntr += 1
			NumberOfNodes -= 1;

	def ReadHeader( self ) :
		Control = ''
		self.Props = []
		while ( Control != 'end_header' ) and ( self.GetNextLine() > 0 ):
			Control = self.CurrentWords[0]
			print 'Control = ' + Control
			if Control == 'format' :
				self.Format = self.CurrentWords[ 1 ]
				print 'Format = ' + self.Format
			elif Control == 'element' :
				Type = self.CurrentWords[ 1 ]
				if Type == 'vertex' :
					self.Nodes = self.CurrentWords[ 2 ]
				elif Type == 'face' :
					self.Elements = self.CurrentWords[ 2 ]
				else :
					pass
			elif Control == 'property' :
				Prop = self.CurrentWords[ 1 ]
				if Prop == 'list' :
					pass
				else :
					if self.CurrentWords[ 2 ] == 'red' :
						self.DoConvert = 1
					self.Props.append( self.CurrentWords[ 2 ] )
			else :
				pass

	def Parse ( self ) :
		print 'Reading Header'
		self.ReadHeader()
		print 'Format ' + self.Format
		print 'Nodes ' + self.Nodes
		print 'Elements ' + self.Elements
		print 'Scalars ' + str(self.Props)
		self.ReadNodes()
		self.ReadElements()
		h5 = XdmfHDF();
		DataSet = self.BaseName + '.h5:/Geometry/XYZ'
		print 'Writing ' + DataSet
		if h5.Open( DataSet, "rw" ) == XDMF_FAIL :	
			h5.CopyShape( self.NodeArray )
			h5.CopyType( self.NodeArray )
			h5.CreateDataset( DataSet )
		h5.Write( self.NodeArray )
		h5.Close()
		if self.FakeColorArray != None :
			h5 = XdmfHDF();
			DataSet = self.BaseName + '.h5:/FakeColor'
			print 'Writing ' + DataSet
			if h5.Open( DataSet, "rw" ) == XDMF_FAIL :	
				h5.CopyShape( self.FakeColorArray )
				h5.CopyType( self.FakeColorArray )
				h5.CreateDataset( DataSet )
			h5.Write( self.FakeColorArray )
			h5.Close()
		if self.ExtraArray != None :
			h5 = XdmfHDF();
			DataSet = self.BaseName + '.h5:/Scalars'
			print 'Writing ' + DataSet
			if h5.Open( DataSet, "rw" ) == XDMF_FAIL :	
				h5.CopyShape( self.ExtraArray )
				h5.CopyType( self.ExtraArray )
				h5.CreateDataset( DataSet )
			h5.Write( self.ExtraArray )
			h5.Close()
		DataSet = self.BaseName + '.h5:/Connections'
		print 'Writing ' + DataSet
		if h5.Open( DataSet, "rw" ) == XDMF_FAIL :	
			h5.CopyShape( self.ElementArray )
			h5.CopyType( self.ElementArray )
			h5.CreateDataset( DataSet )
		h5.Write( self.ElementArray )
		h5.Close()
		XMLFile = open( self.BaseName + '.xml', 'w' )
		XMLFile.write('<?xml version="1.0" ?>\n')
		XMLFile.write('<!DOCTYPE Xdmf SYSTEM "Xdmf.dtd" [\n')
		XMLFile.write('<!ENTITY HeavyData "' + self.BaseName + '.h5">\n')
		XMLFile.write(']>\n')
		XMLFile.write('\n\n\n')
		XMLFile.write('\t<!--\n')
		XMLFile.write('\tGenerated from ' + self.InputFileName + '\n' )
		XMLFile.write('\t' + time.strftime("%a, %d, %b %Y %H:%M", time.localtime()) + '\n')
		XMLFile.write('\t-->\n')
		XMLFile.write('\n')
		XMLFile.write('<Xdmf>\n')
		XMLFile.write('<Domain>\n')
		XMLFile.write('<Grid>\n')
		XMLFile.write('\t<Topology\n')
		if self.NodesPerElement == 3 :
			XMLFile.write('\tType="Triangle" NodesPerElement="3"\n')
		elif self.NodesPerElement == 4 :
			XMLFile.write('\tType="Quadrilateral" NodesPerElement="4"\n')
		else :
			XMLFile.write('\tType="Polygon" NodesPerElement="' + str( self.NodesPerElement ) + '"')
		XMLFile.write('\tNumberOfElements="' + self.Elements + '" >\n' )
		XMLFile.write('\t\t\t<DataStructure Format="HDF" DataType="Int"\n')
		XMLFile.write('\t\t\tDimensions="' + self.Elements + ' ' + str( self.NodesPerElement) + '">\n')
		XMLFile.write('\t\t\t&HeavyData;:/Connections\n')
		XMLFile.write('\t\t\t</DataStructure>\n')
		XMLFile.write('\t</Topology>\n')
		XMLFile.write('\t<Geometry Type="XYZ">\n')
		XMLFile.write('\t\t\t<DataStructure Format="HDF" DataType="Float"\n')
		XMLFile.write('\t\t\tDimensions="' + self.Nodes + ' 3">\n')
		XMLFile.write('\t\t\t&HeavyData;:/Geometry/XYZ\n')
		XMLFile.write('\t\t\t</DataStructure>\n')
		XMLFile.write('\t</Geometry>\n')
		if self.FakeColorArray != None :
			XMLFile.write('\t<Attribute Name="FakeColor" Center="Node" Type="Scalar">\n')
			XMLFile.write('\t\t\t<DataStructure Format="HDF" DataType="Float"\n')
			XMLFile.write('\t\t\tDimensions="' + self.Nodes + '">\n')
			XMLFile.write('\t\t\t&HeavyData;:/FakeColor\n')
			XMLFile.write('\t\t\t</DataStructure>\n')
			XMLFile.write('\t</Attribute>\n')
		XMLFile.write('</Grid>\n')
		XMLFile.write('</Domain>\n')
		XMLFile.write('</Xdmf>\n')
		XMLFile.close()
		print 'Done'
		return

if __name__ == '__main__' :
	argc = len( sys.argv )
	c = PlyParser( sys.argv[ argc - 1 ] )
	c.Parse()
