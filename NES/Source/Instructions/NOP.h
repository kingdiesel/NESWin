#pragma once

#include "BaseInstruction.h"

// http://www.obelisk.me.uk/6502/reference.html#NOP
template<class _addressing_strategy>
class NOPBase : public OpCodeBase<_addressing_strategy>
{
public:
	NOPBase() : OpCodeBase<_addressing_strategy>("NOP")
	{
	}

	void ExecuteImplementation()
	{
	}
};
typedef BaseInstruction<NOPBase<ImpliedAddressingStrategy>, 0xEA, 2> NOP;
typedef BaseInstruction<NOPBase<ImpliedAddressingStrategy>, 0x1A, 2> NOP1A;
typedef BaseInstruction<NOPBase<ImpliedAddressingStrategy>, 0x3A, 2> NOP3A;
typedef BaseInstruction<NOPBase<ImpliedAddressingStrategy>, 0x5A, 2> NOP5A;
typedef BaseInstruction<NOPBase<ImpliedAddressingStrategy>, 0xDA, 2> NOPDA;
typedef BaseInstruction<NOPBase<ImpliedAddressingStrategy>, 0xFA, 2> NOPFA;
