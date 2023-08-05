#pragma once

namespace Client
{
	__interface IStrategy
	{
		virtual void Execute(void* pData) = 0;
	};
}
