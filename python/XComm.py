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

import os
import sys
import socket
import asyncore
import select
import time

class XComm( asyncore.dispatcher ) :
	
	def __init__ ( self, ExistingChannel=None ):
		self.Id = 1
		if ExistingChannel :
			self.Id = 2
		self._RequestId_ = 1
		self.ExpectRequest = 0 
		self.Request = "NONE"
		self.Host = None
		self.Port = None
		asyncore.dispatcher.__init__(self, ExistingChannel)

	def GetHost( self ) :
		return self.Host

	def GetPort( self ) :
		return self.Port

	def SetHost( self, Host ) :
		self.Host = Host

	def SetPort( self, Port ) :
		self.Port = Port

	def Serve( self, Port=None ) :
		port = Port
		if not port :
			port = self.Port
		print 'Starting Server on Port ' + str(port)
		self.create_socket( socket.AF_INET, socket.SOCK_STREAM )
		self.bind( ('', port) )
		self.listen(5)

	def Loop(self) :
		asyncore.loop()

	def Connect ( self, Host=None, Port=None ) :
		if Host == None :
			if self.Host != None :
				Host = self.Host
			else :
				Host = "localhost"
		if Port == None :
			if self.Port != None :
				Port = self.Port
			else :
				Port = XGuiServer.GetUniquePort() 
		self.create_socket( socket.AF_INET, socket.SOCK_STREAM )
		self.socket.setblocking(1)
		SleepTime = 0.5
		tries = 0
		while ( self.connected == 0  ) and ( tries < 20 ) :
			try :
				print 'Connection to (' + Host + ':%d) attempt #%d' % (Port, tries )
				self.connect( ( Host, Port ) )
				# while not self.connected :
				# 	print 'Before Connected = %d' % self.connected
				# 	asyncore.poll( 0.1  )
				# 	print 'After Connected = %d' % self.connected
				print 'Success' 
				self.Host = Host
				self.Port = Port
				self.socket.setblocking(0)
				return( 1 )
			except socket.error, why :
				# print 'Continue : Reason = ' + os.strerror( why[0] )
				self.close()
				time.sleep( SleepTime )
				self.create_socket( socket.AF_INET, socket.SOCK_STREAM )
				self.socket.setblocking(1)
				tries += 1
		return(0)

	def Send( self, Data ) :
		self.socket.setblocking(1)
		self.send('%10d' % len(Data))
		self.send( Data )
		self.socket.setblocking(0)
	
	def Disconnect( self ) :
		print 'Disconnecting ' + str(self.socket.getpeername()) + ' Id %d' % self.Id
		try :
			self.close()
		except :
			pass
		self.connected = 0
		print 'Done'

	def handle_connect( self ) :
		print 'Complete Client Connection'

	def handle_close( self ) :
		pass
#		self.Disconnect()

	def handle_write( self ) :
		pass

	def handle_accept( self ) :
		conn, addr = self.socket.accept()
		print 'Accepting Connection from ' + str(conn) + ' ' + str(addr)
		NewChannel = XComm( conn )

	def handle_read( self ) :
		LenString = self.recv(10)
		if not LenString :
			self.Disconnect()
			return
		self.socket.setblocking(1)
		BytesLeft = int(LenString)
		Data = ''
		while BytesLeft > 0 :
			data = self.recv(BytesLeft);
			if data == None  :
				self.Disconnect()
				return
			else :
#				print 'Read :' + data
				Data += data
				BytesLeft -= len(data)
		self.socket.setblocking(0)
		self.Service( Data )

	def CheckAll( self , Timeout ) :
		asyncore.poll( Timeout )

	def Service(self, Data) :
		# Override this Method
		print 'Service : ' + Data

if __name__ == '__main__' :
	argc = len(sys.argv)
	Comm = XComm()
	if argc == 3 :
		# Client
		Comm.Connect( sys.argv[1], int( sys.argv[2]))
		for i in range(100):
			Comm.Send("Hello World %d" % i )
			time.sleep( 0.5 )
	else :
		Comm.Serve(int( sys.argv[1])) 
		Comm.Loop()
