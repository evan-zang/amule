// This file is part of the aMule Project
//
// Copyright (c) 2003-2004 aMule Project ( http://www.amule-project.net )
// Copyright (C) 2002 Merkur ( merkur-@users.sourceforge.net / http://www.emule-project.net )
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#ifdef __WXMSW__
	#include <winsock.h>
	#include <wx/defs.h>
	#include <wx/msw/winundef.h>
#else
	#include <netinet/in.h>	// Needed for struct sockaddr_in
#endif
#include <wx/defs.h>		// Needed before any other wx/*.h
#include <wx/socket.h>		// Needed for wxDatagramSocket

#include "types.h"		// Needed for uint16 and uint32
#include "amuleIPV4Address.h"	// Needed for wxIPV4address
#include "CTypedPtrList.h"	// Needed for CTypedPtrList

class Packet;
class CServer;
class CSafeMemFile;

#define WM_DNSLOOKUPDONE WM_USER+280

struct ServerUDPPacket {
	Packet*	packet;
	CServer*	server;
};

// Client to Server communication

class CUDPSocket : public wxDatagramSocket 
#ifdef AMULE_DAEMON
, wxThread
#endif
{
	friend class CServerConnect;
	DECLARE_DYNAMIC_CLASS(CUDPSocket);

	CUDPSocket():wxDatagramSocket(useless) {};
public:
	CUDPSocket(CServerConnect* in_serverconnect, amuleIPV4Address& addr);
	~CUDPSocket();

	void	SendPacket(Packet* packet,CServer* host);
	void	DnsLookupDone(uint32 ip);

	virtual void OnReceive(int nErrorCode);
 	void	ReceiveAndDiscard();

private:

	void	SendBuffer();
	void	ProcessPacket(CSafeMemFile& packet, int16 size, int8 opcode, const wxString& host, uint16 port);

	amuleIPV4Address m_SaveAddr;

	CServerConnect*	serverconnect;
	char*	sendbuffer;
	uint32	sendblen;
	CServer* cur_server;
	CTypedPtrList<CPtrList, ServerUDPPacket*> server_packet_queue;
	wxIPV4address useless;
#ifdef AMULE_DAEMON
	void *Entry();
#endif
};

#endif // UDPSOCKET_H
