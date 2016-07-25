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
/** @file TestHelper.h
 * @author Marko Simovic <markobarko@gmail.com>
 * 
 */

#pragma once

#include <functional>
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <boost/version.hpp>

namespace dev
{
namespace test
{

#if (BOOST_VERSION >= 105900)
#define ELE_BOOST_CHECK_IMPL(_message, _requireOrCheck) BOOST_TEST_TOOL_DIRECT_IMPL( \
		false,															\
		_requireOrCheck,												\
		_message														\
	)
#else
#define ELE_BOOST_CHECK_IMPL(_message, _requireOrCheck) BOOST_CHECK_IMPL( \
		false,															\
		_message,														\
		_requireOrCheck,												\
		CHECK_MSG														\
	)
#endif

/// Make sure that no Exception is thrown during testing. If one is thrown show its info and fail the test.
/// Our version of BOOST_REQUIRE_NO_THROW()
/// @param _statement     The statement for which to make sure no exceptions are thrown
/// @param _message       A message to act as a prefix to the expression's error information
#define ELE_TEST_REQUIRE_NO_THROW(_statement, _message)				\
	do																	\
	{																	\
		try															\
		{																\
			BOOST_TEST_PASSPOINT();										\
			_statement;												\
		}																\
		catch (boost::exception const& _e)								\
		{																\
			auto msg = std::string(_message " due to an exception thrown by " \
				BOOST_STRINGIZE(_statement) "\n") + boost::diagnostic_information(_e); \
			ELE_BOOST_CHECK_IMPL(msg, REQUIRE);							\
		}																\
		catch (...)														\
		{																\
			ELE_BOOST_CHECK_IMPL(										\
				"Unknown exception thrown by " BOOST_STRINGIZE(_statement),	\
				REQUIRE													\
			);															\
		}																\
	}																	\
	while (0)

/// Check if an Exception is thrown during testing. If one is thrown show its info and continue the test
/// Our version of BOOST_CHECK_NO_THROW()
/// @param _statement    The statement for which to make sure no exceptions are thrown
/// @param _message       A message to act as a prefix to the expression's error information
#define ELE_TEST_CHECK_NO_THROW(_statement, _message)					\
	do																	\
	{																	\
		try															\
		{																\
			BOOST_TEST_PASSPOINT();										\
			_statement;												\
		}																\
		catch (boost::exception const& _e)								\
		{																\
			auto msg = std::string(_message " due to an exception thrown by " \
				BOOST_STRINGIZE(_statement) "\n") + boost::diagnostic_information(_e); \
			ELE_BOOST_CHECK_IMPL(msg, CHECK);							\
		}																\
		catch (...)														\
		{																\
			ELE_BOOST_CHECK_IMPL(										\
				"Unknown exception thrown by " BOOST_STRINGIZE(_statement),	\
				CHECK													\
			);															\
		}																\
	}																	\
	while (0)

/// Allows observing test execution process.
/// This class also provides methods for registering and notifying the listener
class Listener
{
public:
	virtual ~Listener() = default;

	virtual void suiteStarted(std::string const&) {}
	virtual void testStarted(std::string const& _name) = 0;
	virtual void testFinished() = 0;

	static void registerListener(Listener& _listener);
	static void notifySuiteStarted(std::string const& _name);
	static void notifyTestStarted(std::string const& _name);
	static void notifyTestFinished();

	/// Test started/finished notification RAII helper
	class ExecTimeGuard
	{
	public:
		ExecTimeGuard(std::string const& _testName) { notifyTestStarted(_testName);	}
		~ExecTimeGuard() { notifyTestFinished(); }
		ExecTimeGuard(ExecTimeGuard const&) = delete;
		ExecTimeGuard& operator=(ExecTimeGuard) = delete;
	};
};

}
}
