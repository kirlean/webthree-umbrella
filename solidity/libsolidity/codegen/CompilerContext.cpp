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
/**
 * @
 * 
 * Utilities for the solidity compiler.
 */

#include <libsolidity/codegen/CompilerContext.h>
#include <utility>
#include <numeric>
#include <libsolidity/ast/AST.h>
#include <libsolidity/codegen/Compiler.h>
#include <libsolidity/interface/Version.h>

using namespace std;

namespace dev
{
namespace solidity
{

void CompilerContext::addMagicGlobal(MagicVariableDeclaration const& _declaration)
{
	m_magicGlobals.insert(&_declaration);
}

void CompilerContext::addStateVariable(
	VariableDeclaration const& _declaration,
	u256 const& _storageOffset,
	unsigned _byteOffset
)
{
	m_stateVariables[&_declaration] = make_pair(_storageOffset, _byteOffset);
}

void CompilerContext::startFunction(Declaration const& _function)
{
	m_functionCompilationQueue.startFunction(_function);
	*this << functionEntryLabel(_function);
}

void CompilerContext::addVariable(VariableDeclaration const& _declaration,
								  unsigned _offsetToCurrent)
{
	solAssert(m_asm.deposit() >= 0 && unsigned(m_asm.deposit()) >= _offsetToCurrent, "");
	m_localVariables[&_declaration] = unsigned(m_asm.deposit()) - _offsetToCurrent;
}

void CompilerContext::removeVariable(VariableDeclaration const& _declaration)
{
	solAssert(!!m_localVariables.count(&_declaration), "");
	m_localVariables.erase(&_declaration);
}

ele::Assembly const& CompilerContext::compiledContract(const ContractDefinition& _contract) const
{
	auto ret = m_compiledContracts.find(&_contract);
	solAssert(ret != m_compiledContracts.end(), "Compiled contract not found.");
	return *ret->second;
}

bool CompilerContext::isLocalVariable(Declaration const* _declaration) const
{
	return !!m_localVariables.count(_declaration);
}

ele::AssemblyItem CompilerContext::functionEntryLabel(Declaration const& _declaration)
{
	return m_functionCompilationQueue.entryLabel(_declaration, *this);
}

ele::AssemblyItem CompilerContext::functionEntryLabelIfExists(Declaration const& _declaration) const
{
	return m_functionCompilationQueue.entryLabelIfExists(_declaration);
}

ele::AssemblyItem CompilerContext::virtualFunctionEntryLabel(FunctionDefinition const& _function)
{
	// Libraries do not allow inheritance and their functions can be inlined, so we should not
	// search the inheritance hierarchy (which will be the wrong one in case the function
	// is inlined).
	if (auto scope = dynamic_cast<ContractDefinition const*>(_function.scope()))
		if (scope->isLibrary())
			return functionEntryLabel(_function);
	solAssert(!m_inheritanceHierarchy.empty(), "No inheritance hierarchy set.");
	return virtualFunctionEntryLabel(_function, m_inheritanceHierarchy.begin());
}

ele::AssemblyItem CompilerContext::superFunctionEntryLabel(FunctionDefinition const& _function, ContractDefinition const& _base)
{
	solAssert(!m_inheritanceHierarchy.empty(), "No inheritance hierarchy set.");
	return virtualFunctionEntryLabel(_function, superContract(_base));
}

FunctionDefinition const* CompilerContext::nextConstructor(ContractDefinition const& _contract) const
{
	vector<ContractDefinition const*>::const_iterator it = superContract(_contract);
	for (; it != m_inheritanceHierarchy.end(); ++it)
		if ((*it)->constructor())
			return (*it)->constructor();

	return nullptr;
}

Declaration const* CompilerContext::nextFunctionToCompile() const
{
	return m_functionCompilationQueue.nextFunctionToCompile();
}

ModifierDefinition const& CompilerContext::functionModifier(string const& _name) const
{
	solAssert(!m_inheritanceHierarchy.empty(), "No inheritance hierarchy set.");
	for (ContractDefinition const* contract: m_inheritanceHierarchy)
		for (ModifierDefinition const* modifier: contract->functionModifiers())
			if (modifier->name() == _name)
				return *modifier;
	BOOST_THROW_EXCEPTION(InternalCompilerError()
		<< errinfo_comment("Function modifier " + _name + " not found."));
}

unsigned CompilerContext::baseStackOffsetOfVariable(Declaration const& _declaration) const
{
	auto res = m_localVariables.find(&_declaration);
	solAssert(res != m_localVariables.end(), "Variable not found on stack.");
	return res->second;
}

unsigned CompilerContext::baseToCurrentStackOffset(unsigned _baseOffset) const
{
	return m_asm.deposit() - _baseOffset - 1;
}

unsigned CompilerContext::currentToBaseStackOffset(unsigned _offset) const
{
	return m_asm.deposit() - _offset - 1;
}

pair<u256, unsigned> CompilerContext::storageLocationOfVariable(const Declaration& _declaration) const
{
	auto it = m_stateVariables.find(&_declaration);
	solAssert(it != m_stateVariables.end(), "Variable not found in storage.");
	return it->second;
}

CompilerContext& CompilerContext::appendJump(ele::AssemblyItem::JumpType _jumpType)
{
	ele::AssemblyItem item(Instruction::JUMP);
	item.setJumpType(_jumpType);
	return *this << item;
}

void CompilerContext::resetVisitedNodes(ASTNode const* _node)
{
	stack<ASTNode const*> newStack;
	newStack.push(_node);
	std::swap(m_visitedNodes, newStack);
	updateSourceLocation();
}

void CompilerContext::injectVersionStampIntoSub(size_t _subIndex)
{
	ele::Assembly& sub = m_asm.sub(_subIndex);
	sub.injectStart(Instruction::POP);
	sub.injectStart(fromBigEndian<u256>(binaryVersion()));
}

ele::AssemblyItem CompilerContext::virtualFunctionEntryLabel(
	FunctionDefinition const& _function,
	vector<ContractDefinition const*>::const_iterator _searchStart
)
{
	string name = _function.name();
	FunctionType functionType(_function);
	auto it = _searchStart;
	for (; it != m_inheritanceHierarchy.end(); ++it)
		for (FunctionDefinition const* function: (*it)->definedFunctions())
			if (
				function->name() == name &&
				!function->isConstructor() &&
				FunctionType(*function).hasEqualArgumentTypes(functionType)
			)
				return functionEntryLabel(*function);
	solAssert(false, "Super function " + name + " not found.");
	return m_asm.newTag(); // not reached
}

vector<ContractDefinition const*>::const_iterator CompilerContext::superContract(ContractDefinition const& _contract) const
{
	solAssert(!m_inheritanceHierarchy.empty(), "No inheritance hierarchy set.");
	auto it = find(m_inheritanceHierarchy.begin(), m_inheritanceHierarchy.end(), &_contract);
	solAssert(it != m_inheritanceHierarchy.end(), "Base not found in inheritance hierarchy.");
	return ++it;
}

void CompilerContext::updateSourceLocation()
{
	m_asm.setSourceLocation(m_visitedNodes.empty() ? SourceLocation() : m_visitedNodes.top()->location());
}

ele::AssemblyItem CompilerContext::FunctionCompilationQueue::entryLabel(
	Declaration const& _declaration,
	CompilerContext& _context
)
{
	auto res = m_entryLabels.find(&_declaration);
	if (res == m_entryLabels.end())
	{
		ele::AssemblyItem tag(_context.newTag());
		m_entryLabels.insert(make_pair(&_declaration, tag));
		m_functionsToCompile.push(&_declaration);
		return tag.tag();
	}
	else
		return res->second.tag();

}

ele::AssemblyItem CompilerContext::FunctionCompilationQueue::entryLabelIfExists(Declaration const& _declaration) const
{
	auto res = m_entryLabels.find(&_declaration);
	return res == m_entryLabels.end() ? ele::AssemblyItem(ele::UndefinedItem) : res->second.tag();
}

Declaration const* CompilerContext::FunctionCompilationQueue::nextFunctionToCompile() const
{
	while (!m_functionsToCompile.empty())
	{
		if (m_alreadyCompiledFunctions.count(m_functionsToCompile.front()))
			m_functionsToCompile.pop();
		else
			return m_functionsToCompile.front();
	}
	return nullptr;
}

void CompilerContext::FunctionCompilationQueue::startFunction(Declaration const& _function)
{
	if (!m_functionsToCompile.empty() && m_functionsToCompile.front() == &_function)
		m_functionsToCompile.pop();
	m_alreadyCompiledFunctions.insert(&_function);
}

}
}
