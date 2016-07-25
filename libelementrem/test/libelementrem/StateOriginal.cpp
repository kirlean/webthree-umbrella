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
/** @file stateOriginal.cpp
 * 
 * 
 * State test functions.
 */

#include <test/TestHelper.h>
#include <boost/test/unit_test.hpp>
#include <boost/filesystem/operations.hpp>
#include <libelementrem/BlockChain.h>
#include <libelementrem/Block.h>
#include <libelementrem/GenericFarm.h>
#include <libelecore/BasicAuthority.h>
#include <libelementrem/Defaults.h>

using namespace std;
using namespace dev;
using namespace dev::ele;

namespace dev
{
namespace test
{

int stateTest();

BOOST_FIXTURE_TEST_SUITE(StateIntegration, TestOutputHelper)

BOOST_AUTO_TEST_CASE(Basic)
{
	Block s(Block::Null);
}

BOOST_AUTO_TEST_CASE(Complex)
{
	//Mining Changed on block branch
	//Review this test

	/*if (test::Options::get().nodag)
		return;

	cnote << "Testing State...";

	KeyPair me = Secret(sha3("Gav Wood"));
	KeyPair myMiner = Secret(sha3("Gav's Miner"));
//	KeyPair you = sha3("123");

	Defaults::setDBPath(boost::filesystem::temp_directory_path().string() + "/" + toString(chrono::system_clock::now().time_since_epoch().count()));

	OverlayDB stateDB = State::openDB(h256());
	BlockChain bc(new BasicAuthority, ChainParams(Network::Olympic);
	cout << bc;

	Block s = bc.genesisBlock(stateDB);
	s.setAuthor(myMiner.address());
	cout << s;

	// Sync up - this won't do much until we use the last state.
	s.sync(bc);

	cout << s;

	// Mine to get some element!
	mine(s, bc);

	bc.attemptImport(s.blockData(), stateDB);

	cout << bc;

	s.sync(bc);

	cout << s;

	// Inject a transaction to transfer funds from miner to me.
	Transaction t(1000, 10000, 100000, me.address(), bytes(), s.transactionsFrom(myMiner.address()), myMiner.secret());
	assert(t.sender() == myMiner.address());
	s.execute(bc.lastHashes(), t);

	cout << s;

	// Mine to get some element and set in stone.
	s.commitToSeal(bc);
	s.commitToSeal(bc);
	mine(s, bc);
	bc.attemptImport(s.blockData(), stateDB);

	cout << bc;

	s.sync(bc);

	cout << s;*/
}

BOOST_AUTO_TEST_SUITE_END()

}
}
