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
/** @file Bzz.h
 * @authors:
 *   
 *   
 * 
 */

#pragma once
#include "BzzFace.h"

namespace dev
{

namespace bzz
{
class Interface;
}

namespace rpc
{

class Bzz: public BzzFace
{
public:
	Bzz(dev::bzz::Interface& _bzz);
	virtual RPCModules implementedModules() const override
	{
		return RPCModules{RPCModule{"bzz", "1.0"}};
	}
	virtual std::string bzz_put(std::string const& _data) override;
	virtual std::string bzz_get(std::string const& _hash) override;

private:
	dev::bzz::Interface& m_bzz;
};

}
}
