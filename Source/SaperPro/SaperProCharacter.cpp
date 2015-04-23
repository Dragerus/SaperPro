// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SaperPro.h"
#include "SaperProCharacter.h"


const char* MOVE_FORWARD = "MoveForward";
const char* MOVE_RIGHT = "MoveRight";
const char* TURN = "Turn";
const char* LOOK_UP = "LookUp";
const char* PLANT_BOMB = "PlantBomb";
const char* JUMP_PRESSED = "Jump";
const char* JUMP_RELEASED = "Jump";
const char* JUMP_STOP = "JumpStop";


//////////////////////////////////////////////////////////////////////////
// ASaperProCharacter

ASaperProCharacter::ASaperProCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//state_machine = new SaperStateMachine( this );

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	want_plant_bomb = false;
	bomb_planting = false;
	isDead = false;
	life = 100.f;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASaperProCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);
	InputComponent->BindAction( "Jump", IE_Pressed, this, &ASaperProCharacter::JumpKey );
	InputComponent->BindAction( "Jump", IE_Released, this, &ASaperProCharacter::StopJumpingKey );

	InputComponent->BindAxis( "MoveForward", this, &ASaperProCharacter::MoveForward );
	InputComponent->BindAxis( "MoveRight", this, &ASaperProCharacter::MoveRight );

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis( "Turn", this, &ASaperProCharacter::AddControllerYawInput );
	InputComponent->BindAxis("TurnRate", this, &ASaperProCharacter::TurnAtRate);
	InputComponent->BindAxis( "LookUp", this, &ASaperProCharacter::AddControllerPitchInput );
	InputComponent->BindAxis("LookUpRate", this, &ASaperProCharacter::LookUpAtRate);

	// handle touch devices
	InputComponent->BindTouch(IE_Pressed, this, &ASaperProCharacter::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &ASaperProCharacter::TouchStopped);
	InputComponent->BindAction( PLANT_BOMB, IE_Pressed, this, &ASaperProCharacter::Plant );
	InputComponent->BindAction( PLANT_BOMB, IE_Released, this, &ASaperProCharacter::PlantEnd );
}

//void ASaperProCharacter::Tick( float DeltaSeconds )
//{
//	ACharacter::Tick( DeltaSeconds );
//
//	//if ( GetMovementComponent()->IsFalling() )
//	//	state_machine->input( JUMP_STOP, 0.0 );
//}

void ASaperProCharacter::Plant()
{
	want_plant_bomb = true;

    //FVector position = GetTransform ();
}

void ASaperProCharacter::PlantEnd()
{
	// To jest tymczasowe rozwi¹zania
	want_end_planting_bomb = true;


}

bool ASaperProCharacter::shouldnt_move()
{
	if ( bomb_planting )
		return true;
	if ( GetMovementComponent()->IsFalling() )
		return true;
    if (isDead)
        return true;
	return false;
}

void ASaperProCharacter::TurnCamera( float AxisValue )
{
	AddControllerYawInput( AxisValue );
}

void ASaperProCharacter::LookUp( float AxisValue )
{
	AddControllerPitchInput( AxisValue );
}

void ASaperProCharacter::JumpKey()
{
	if ( shouldnt_move() )
		return;
	this->Jump();
}

void ASaperProCharacter::StopJumpingKey()
{
	this->StopJumping();
}

void ASaperProCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	if ( shouldnt_move() )
		return;
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		Jump();
	}
}

void ASaperProCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1)
	{
		StopJumping();
	}
}

void ASaperProCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASaperProCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ASaperProCharacter::MoveForward(float Value)
{
	if ( shouldnt_move() )
		return;

	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ASaperProCharacter::MoveRight(float Value)
{
	if ( shouldnt_move() )
		return;

	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

//==================================================================//

void ASaperProCharacter::takeDamage( float power )
{
	life -= power;

	if ( life <= 0.0 )
		isDead = true;
}
