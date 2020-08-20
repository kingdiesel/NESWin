//
//

#ifndef NES_NOP_H
#define NES_NOP_H

#include "BaseInstruction.h"
#include "../Addressing/AddressingMode.h"

template<typename _addressing_mode, typename _execute, int _op_code>
class NOPBase : public BaseInstruction<_addressing_mode, _execute, _op_code>
{
public:
	NOPBase() : BaseInstruction<_addressing_mode, _execute, _op_code>(2, "NOP")
	{
	}

	void ExecuteImplementation()
	{
	}
};

class NOP : public NOPBase<ImpliedAddressingStrategy, NOP, 0xEA>
{
};

class NOP1A : public NOPBase<ImpliedAddressingStrategy, NOP1A, 0x1A>
{
};

class NOP3A : public NOPBase<ImpliedAddressingStrategy, NOP3A, 0x3A>
{
};

class NOP5A : public NOPBase<ImpliedAddressingStrategy, NOP5A, 0x5A>
{
};

class NOPDA : public NOPBase<ImpliedAddressingStrategy, NOPDA, 0xDA>
{
};

class NOPFA : public NOPBase<ImpliedAddressingStrategy, NOPFA, 0xFA>
{
};

#endif //NES_NOP_H
