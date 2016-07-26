/*
This file is part of cpp-elementrem.

cpp-elementrem is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

cpp-elementrem is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with cpp-elementrem.  If not, see <http://www.gnu.org/licenses/>.
*/
/** @file UnixSocketServer.h
* @authors:
*   
* 
*/
#pragma once

#ifndef WIN32

#include <string>
#include <thread>
#include <sys/un.h>
#include "IpcServerBase.h"

namespace dev
{
class UnixDomainSocketServer: public IpcServerBase<int>
{
public:
	UnixDomainSocketServer(std::string const& _appId);
	~UnixDomainSocketServer();
	bool StartListening() override;
	bool StopListening() override;

protected:
	void Listen() override;
	void CloseConnection(int _socket) override;
	size_t Write(int _connection, std::string const& _data) override;
	size_t Read(int _connection, void* _data, size_t _size) override;

	sockaddr_un m_address;
	int m_socket = 0;
};

} // namespace dev

#endif