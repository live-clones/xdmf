/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfDSMCommMPI.cpp                                                  */
/*                                                                           */
/*  Author:                                                                  */
/*     Andrew Burns                                                          */
/*     andrew.j.burns2@us.army.mil                                           */
/*     US Army Research Laboratory                                           */
/*     Aberdeen Proving Ground, MD                                           */
/*                                                                           */
/*     Copyright @ 2013 US Army Research Laboratory                          */
/*     All Rights Reserved                                                   */
/*     See Copyright.txt for details                                         */
/*                                                                           */
/*     This software is distributed WITHOUT ANY WARRANTY; without            */
/*     even the implied warranty of MERCHANTABILITY or FITNESS               */
/*     FOR A PARTICULAR PURPOSE.  See the above copyright notice             */
/*     for more information.                                                 */
/*                                                                           */
/*****************************************************************************/

/*=========================================================================
  This code is derived from an earlier work and is distributed
  with permission from, and thanks to ...
=========================================================================*/

/*============================================================================

  Project                 : H5FDdsm
  Module                  : H5FDdsmCommMpi.cxx

  Authors:
     John Biddiscombe     Jerome Soumagne
     biddisco@cscs.ch     soumagne@cscs.ch

  Copyright (C) CSCS - Swiss National Supercomputing Centre.
  You may use modify and and distribute this code freely providing
  1) This copyright notice appears on all copies of source code
  2) An acknowledgment appears with any substantial usage of the code
  3) If this code is contributed to any other open source project, it
  must not be reformatted such that the indentation, bracketing or
  overall style is modified significantly.

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

  This work has received funding from the European Community's Seventh
  Framework Programme (FP7/2007-2013) under grant agreement 225967 âxtMuSEâOC

============================================================================*/

#include <XdmfDSMCommMPI.hpp>
#include <XdmfError.hpp>
#include <mpi.h>
#include <string.h>

XdmfDSMCommMPI::XdmfDSMCommMPI()
{
  IntraComm = MPI_COMM_NULL;
  Id = -1;
  IntraSize = -1;
  InterComm = MPI_COMM_NULL;
  InterSize = -1;
  SetDsmMasterHostName("");
  InterCommType = XDMF_DSM_COMM_MPI;
}

XdmfDSMCommMPI::~XdmfDSMCommMPI()
{
  if (InterComm != MPI_COMM_NULL) {
    int status = MPI_Comm_free(&InterComm);
    if (status != MPI_SUCCESS) {
      try {
                          XdmfError::message(XdmfError::FATAL, "Failed to free intercomm Comm");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  if (IntraComm != MPI_COMM_NULL) {
    int status = MPI_Comm_free(&IntraComm);
    if (status != MPI_SUCCESS) {
      try {
        XdmfError::message(XdmfError::FATAL, "Failed to free intercomm Comm");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
}

void
XdmfDSMCommMPI::SetDsmMasterHostName(const char *hostName)
{
  strcpy(DsmMasterHostName, hostName);
}

char *
XdmfDSMCommMPI::GetDsmMasterHostName()
{
  return DsmMasterHostName;
}

int
XdmfDSMCommMPI::GetId()
{
  return this->Id;
}

int
XdmfDSMCommMPI::GetIntraSize()
{
  return this->IntraSize;
}

int
XdmfDSMCommMPI::GetInterSize()
{
  return this->InterSize;
}

int
XdmfDSMCommMPI::GetInterCommType()
{
  return this->InterCommType;
}

void
XdmfDSMCommMPI::Init()
{
  int size, rank;
  if (MPI_Comm_size(this->IntraComm, &size) != MPI_SUCCESS) {
    try {
      XdmfError::message(XdmfError::FATAL, "Failed to initialize size");
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  if (MPI_Comm_rank(this->IntraComm, &rank) != MPI_SUCCESS) {
    try {
      XdmfError::message(XdmfError::FATAL, "Failed to initialize rank");
    }
    catch (XdmfError e) {
      throw e;
    }
  }

  this->Id = rank;
  this->IntraSize = size;
}

void
XdmfDSMCommMPI::OpenPort()
{
  if (Id == 0) {
    int status = MPI_Open_port(MPI_INFO_NULL, DsmMasterHostName);
    if (status != MPI_SUCCESS) {
      try {
        std::string message = "Failed to open port ";
        message = message + DsmMasterHostName;
        XdmfError::message(XdmfError::FATAL, message);
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
}

void
XdmfDSMCommMPI::ClosePort()
{
  if (Id == 0) {
    int status;
    status = MPI_Open_port(MPI_INFO_NULL, DsmMasterHostName);
    if (status != MPI_SUCCESS) {
      try {
        std::string message = "Failed to close port ";
        message = message + DsmMasterHostName;
        XdmfError::message(XdmfError::FATAL, message);
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
}

void
XdmfDSMCommMPI::Accept()
{
  int status = MPI_Comm_accept(DsmMasterHostName, MPI_INFO_NULL, 0, IntraComm, &InterComm);
  if (status != MPI_SUCCESS) {
    try {
      std::string message = "Failed to accept port ";
      message = message + DsmMasterHostName;
      XdmfError::message(XdmfError::FATAL, message);
    }
    catch (XdmfError e) {
      throw e;
    }
  }
  else {
    MPI_Comm_remote_size(InterComm, &InterSize);
  }
}

int
XdmfDSMCommMPI::Connect()
{
  if (InterComm != MPI_COMM_NULL) {
    // If the intercomm already exists, no need to connect
    // If you want to reset the intercomm, set it to MPI_COMM_NULL before calling this
    // using either SetInterComm or Disconnect
    return MPI_SUCCESS;
  }
  else {
    MPI_Errhandler_set(IntraComm, MPI_ERRORS_RETURN);
    int status = MPI_Comm_connect(DsmMasterHostName, MPI_INFO_NULL, 0, IntraComm, &InterComm);
    MPI_Errhandler_set(IntraComm, MPI_ERRORS_ARE_FATAL);
    if (status != MPI_SUCCESS) {
      try {
        std::string message = "Failed to connect to port ";
        message = message + DsmMasterHostName;
        XdmfError::message(XdmfError::FATAL, message);
      }
      catch (XdmfError e) {
        throw e;
      }
    }
    else {
      status = MPI_Comm_remote_size(InterComm, &InterSize);
      return MPI_SUCCESS;
    }
  }
  return MPI_SUCCESS;
}

void
XdmfDSMCommMPI::Disconnect()
{
  if (InterComm != MPI_COMM_NULL) {
    int status = MPI_Comm_free(&InterComm);
    if (status != MPI_SUCCESS) {
      try {
        XdmfError::message(XdmfError::FATAL, "Failed to disconnect Comm");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  InterComm = MPI_COMM_NULL;
}

void
XdmfDSMCommMPI::DupComm(MPI_Comm comm)
{
  if (IntraComm != comm) {
    int status;
    if (IntraComm != MPI_COMM_NULL) {
      status = MPI_Comm_free(&IntraComm);
      if (status != MPI_SUCCESS) {
        try {
                            XdmfError::message(XdmfError::FATAL, "Failed to disconnect Comm");
        }
        catch (XdmfError e) {
          throw e;
        }
      }
    }
    if (comm != MPI_COMM_NULL) {
      status = MPI_Comm_dup(comm, &IntraComm);
      if (status != MPI_SUCCESS) {
        try {
          XdmfError::message(XdmfError::FATAL, "Failed to duplicate Comm");
        }
        catch (XdmfError e) {
          throw e;
        }
      }
      else {
        status = MPI_Comm_size(IntraComm, &IntraSize);
        status = MPI_Comm_rank(IntraComm, &Id);
      }
    }
  }
}

void
XdmfDSMCommMPI::DupInterComm(MPI_Comm comm)
{
  if (InterComm != comm) {
    int status;
    if (InterComm != MPI_COMM_NULL) {
      status = MPI_Comm_free(&InterComm);
      if (status != MPI_SUCCESS) {
        try {
          XdmfError::message(XdmfError::FATAL, "Failed to disconnect Comm");
        }
        catch (XdmfError e) {
          throw e;
        }
      }
    }
    if (comm != MPI_COMM_NULL) {
      status = MPI_Comm_dup(comm, &InterComm);
      if (status != MPI_SUCCESS) {
        try {
          XdmfError::message(XdmfError::FATAL, "Failed to duplicate Comm");
        }
        catch (XdmfError e) {
          throw e;
        }
      }
      else {
        status = MPI_Comm_size(InterComm, &InterSize);
        if (status != MPI_SUCCESS) {
          MPI_Comm_remote_size(InterComm, &InterSize);
        }
      }
    }
    else {
      InterSize = -1;
    }
  }
}

MPI_Comm
XdmfDSMCommMPI::GetInterComm()
{
  return InterComm;
}

MPI_Comm
XdmfDSMCommMPI::GetIntraComm()
{
  return IntraComm;
}
