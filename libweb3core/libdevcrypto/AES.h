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
/** @file AES.h
 * @author Alex Leverington <nessence@gmail.com>
 * 
 *
 * AES
 * todo: use openssl
 */

#pragma once

#include "Common.h"

namespace dev
{

bytes aesDecrypt(bytesConstRef _cipher, std::string const& _password, unsigned _rounds = 2000, bytesConstRef _salt = bytesConstRef());

}
