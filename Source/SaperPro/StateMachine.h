#pragma once

#include <unordered_map>

class State;

class StateMachine
{
private:
	int			current_state_num;
	State*		current_state;
	
	std::unordered_map<int, State*>		states;
protected:
	void addState( State* new_state );
	void addInitState( State* new_state );		///<Ustawia pocz¹tkowy stan maszyny.
	
public:
	StateMachine();
	~StateMachine();

	inline int getCurrentState() { return current_state_num; }
	int input( const char* event_id, float value );
};


class State
{
private:
	int state_id;
public:
	State( int id ) { state_id = id; }
	~State() = default;
	
	virtual int transition( const char* event_id, float value ) = 0;
	virtual void stateBegin() = 0;
	virtual void stateEnd() = 0;
	inline int getId() { return state_id; }
};