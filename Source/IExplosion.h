#pragma once

/**@brief Interfejs dla klas, kt�re mog� si� znale�� w polu ra�enia eksplozji
i musza na to zareagowa�.*/
class IExplosion
{
private:

public:
	virtual void takeDamage( float power ) = 0;

};