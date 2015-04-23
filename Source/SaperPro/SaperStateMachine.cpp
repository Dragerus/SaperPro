#include "SaperPro.h"
#include "SaperProCharacter.h"


extern const char* MOVE_FORWARD;
extern const char* MOVE_RIGHT;
extern const char* TURN;
extern const char* LOOK_UP;
extern const char* PLANT_BOMB;
extern const char* JUMP_PRESSED;
extern const char* JUMP_RELEASED;
extern const char* JUMP_STOP;

SaperStateMachine::SaperStateMachine( ASaperProCharacter* character )
{
	addInitState( new class StateIdleRun( character ) );
	addState( new class StateJumpEnd( character ) );
	//addState( new class StateJumpLoop( character ) );
	addState( new class StateJumpStart( character ) );
}


//==============================================//
// Funkcje obs³ugujace standardowe poruszanie postaci¹
//==============================================//
void SaperState::standard_movement( const char* event_id, float value )
{
	if ( event_id == MOVE_FORWARD )
		character->MoveForward( value );
	else if ( event_id == MOVE_RIGHT )
		character->MoveRight( value );
}

void SaperState::standard_view( const char* event_id, float value )
{
	if ( event_id == TURN )
		character->TurnCamera( value );
	else if ( event_id == LOOK_UP )
		character->LookUp( value );
}

void SaperState::standard_jump( const char* event_id )
{
	if ( event_id == JUMP_PRESSED )
		character->JumpKey();
	if ( event_id == JUMP_RELEASED )
		character->StopJumpingKey();
}

//==============================================//

/**Przejœcia:
StateIdleRun -> StateJumpStart

@attention Tu siê dzieje straszne z³oooo. Bo ja nie porównujê stringów w parametrze
event_id tylko wskaŸniki, co oznacza, ¿e je¿eli nie podamy jednej z tych sta³ych podanych na górze
to wszystko le¿y...*/
int StateIdleRun::transition( const char* event_id, float value )
{
	standard_view( event_id, value );
	standard_movement( event_id, value );

	if ( event_id == JUMP_PRESSED )
	{
		character->JumpKey();
		return StateJumpStart;
	}
	else if ( event_id == JUMP_STOP )
	{
		character->StopJumping();
		return StateJumpEnd;
	}

	return getId();
}

int StateJumpEnd::transition( const char* event_id, float value )
{

	return getId();
}

//int StateJumpLoop::transition( const char* event_id, float value )
//{
//
//
//	return getId();
//}

int StateJumpStart::transition( const char* event_id, float value )
{
	standard_view( event_id, value );

	return getId();
}
