// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "StateMachine.h"
#include "IExplosion.h"
#include <vector>
#include "SaperProCharacter.generated.h"


class ASaperProCharacter;

extern const char* MOVE_FORWARD;
extern const char* MOVE_RIGHT;
extern const char* TURN;
extern const char* LOOK_UP;
extern const char* PLANT_BOMB;
extern const char* JUMP_PRESSED;
extern const char* JUMP_RELEASED;


class ABomb;

class SaperStateMachine : public StateMachine
{
private:
public:
	SaperStateMachine( ASaperProCharacter* character );
	~SaperStateMachine() = default;
};

UENUM( Blueprintable )
enum SAPER_STATE
{
	StateIdleRun,
	StateJumpStart,
	StateJumpLoop,
	StateJumpEnd
};



UCLASS(config=Game)
class ASaperProCharacter : public ACharacter, public IExplosion
{
private:

	GENERATED_BODY()

	//SaperStateMachine*		state_machine;
	std::vector<ABomb*>			pack;
	float						life;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	ASaperProCharacter(const FObjectInitializer& ObjectInitializer);

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	//==================================================================//

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite, Category = "Saper" )
	bool		bomb_planting;		// Ustawia animation blueprint
	// Wszystkie zmienne want_... s¹ zerowane przez blueprint w odpowiednim momencie,
	// my tylko ustawiamy wartoœæ na true w celu powiadomienia.
	UPROPERTY( VisibleAnywhere, BlueprintReadWrite, Category = "Saper" )
	bool		want_plant_bomb;	// Ustawiamy gdy chcemy powiadomiæ animation blueprint
	UPROPERTY( VisibleAnywhere, BlueprintReadWrite, Category = "Saper" )
	bool		want_end_planting_bomb;	// Ustawiamy, gdy chcemy powiadomiæ blueprint
	UPROPERTY( VisibleAnywhere, BlueprintReadWrite, Category = "Saper" )
	bool		isDead;

	//==================================================================//
	// Funkcje
	UFUNCTION( BlueprintCallable, Category = "Saper" )
	virtual void takeDamage( float power );

	//==================================================================//
public:
	bool shouldnt_move();

	//inline void event_MOVE_FORWARD( float value ){ state_machine->input( MOVE_FORWARD, value ); }
	//inline void event_MOVE_RIGHT( float value ){ state_machine->input( MOVE_RIGHT, value ); }
	//inline void event_TURN( float value ){ state_machine->input( TURN, value ); }
	//inline void event_LOOK_UP( float value ){ state_machine->input( LOOK_UP, value ); }
	//inline void event_PLANT_BOMB(){ state_machine->input( PLANT_BOMB, 1.0f ); }
	//inline void event_JUMP_PRESSED(){ state_machine->input( JUMP_PRESSED, 1.0f ); }
	//inline void event_JUMP_RELEASED(){ state_machine->input( JUMP_RELEASED, 1.0f ); }


	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	void TurnCamera( float AxisValue );
	void LookUp( float AxisValue );
	void JumpKey();
	void StopJumpingKey();
	void Plant();
	void PlantEnd();


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};



class SaperState : public State
{
protected:
	ASaperProCharacter* character;
public:
	SaperState( ASaperProCharacter* owner, int state_id ) : State( state_id ) { character = owner; }
	~SaperState() = default;
	void stateBegin() {}
	virtual void stateEnd() {}

	void standard_movement( const char* event_id, float value );
	void standard_view( const char* event_id, float value );
	void standard_jump( const char* event_id );
};



class StateIdleRun : public SaperState
{
public:
	StateIdleRun( ASaperProCharacter* owner ) : SaperState( owner, SAPER_STATE::StateIdleRun ){}
	virtual int transition( const char* event_id, float value );
};

class StateJumpStart : public SaperState
{
public:
	StateJumpStart( ASaperProCharacter* owner ) : SaperState( owner, SAPER_STATE::StateJumpStart ){}
	virtual int transition( const char* event_id, float value );
};


class StateJumpLoop : public SaperState
{
public:
	StateJumpLoop( ASaperProCharacter* owner ) : SaperState( owner, SAPER_STATE::StateJumpLoop ){}
	virtual int transition( const char* event_id, float value );
};

class StateJumpEnd : public SaperState
{
public:
	StateJumpEnd( ASaperProCharacter* owner ) : SaperState( owner, SAPER_STATE::StateJumpEnd ){}
	virtual int transition( const char* event_id, float value );
};

