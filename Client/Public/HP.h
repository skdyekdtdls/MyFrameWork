#pragma once

BEGIN(Client)
class HP
{
protected:
	HP() = default;
	virtual ~HP() = default;
public:
	virtual void TakeDamage(_uint iAmount) = 0;
	virtual void Heal(_uint iAmount) = 0;
};
END