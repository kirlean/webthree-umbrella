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
/** @file OtherAccounts.h
 * 
 * 
 */

#pragma once

#include "AccountNamerPlugin.h"

namespace dev
{
namespace alele
{
namespace zero
{

class OtherAccounts: public QObject, public AccountNamerPlugin
{
	Q_OBJECT

public:
	OtherAccounts(ZeroFace* _m);

protected:
	std::string toName(Address const& _a) const override { if (m_toName.count(_a)) return m_toName.at(_a); return std::string(); }
	Address toAddress(std::string const& _n) const override { if (m_toAddress.count(_n)) return m_toAddress.at(_n); return Address(); }
	Addresses knownAddresses() const override { return keysOf(m_toName); }

private slots:
	void import();

private:
	void readSettings(QSettings const&) override;
	void writeSettings(QSettings&) override;

	std::unordered_map<std::string, Address> m_toAddress;
	std::unordered_map<Address, std::string> m_toName;
};

}
}
}
