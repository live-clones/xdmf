#
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
import socket
import Xdmf

argc = len( sys.argv )
Hostname = sys.argv[ argc - 3 ]
Port = sys.argv[ argc - 2 ]
FileName = sys.argv[ argc - 1 ]
DOM = Xdmf.XdmfDOM()
DOM.SetInputFileName( FileName )
DOM.Parse()

soc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
soc.connect(( Hostname, int(Port) ))
soc.send( DOM.Serialize() )
Error = 0
while Error == 0 :
	Data = soc.recv(1024)
	if len( Data ) < 3 :
		Host, Port = soc.getpeername()
		Name, Alias, Ips = socket.gethostbyaddr( Host )
		print 'Bad String From ' + Name
		Error = 1
	else :
		print 'Data:'+Data

