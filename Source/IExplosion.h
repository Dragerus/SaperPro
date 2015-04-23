#pragma once

/**@brief Interfejs dla klas, które mog¹ siê znaleŸæ w polu ra¿enia eksplozji
i musza na to zareagowaæ.*/
class IExplosion
{
private:

public:
	virtual void takeDamage( float power ) = 0;

};