// Fill out your copyright notice in the Description page of Project Settings.

#include "SaperPro.h"
#include "Bomb.h"


// Sets default values
ABomb::ABomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>( TEXT( "Root" ) );
	bomb_trigger = CreateDefaultSubobject<UCapsuleComponent>( TEXT( "Trigger" ) );
	bomb_range = CreateDefaultSubobject<USphereComponent>( TEXT( "Range" ) );

	bomb_trigger->AttachTo(RootComponent);
	bomb_range->AttachTo(RootComponent);

	planting_time = 0.0;		// Klasa pochdna musi sama sobie ustawiæ
}

// Called when the game starts or when spawned
void ABomb::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABomb::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

