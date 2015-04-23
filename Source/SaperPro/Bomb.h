// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Bomb.generated.h"

/**Klasa bazowa dla wszystkich bomb.*/
UCLASS()
class SAPERPRO_API ABomb : public AActor
{
	GENERATED_BODY()

	float planting_time;			// Klasa pochdna musi sama sobie ustawiæ
public:	
	// Sets default values for this actor's properties
	ABomb();

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
	UCapsuleComponent* bomb_trigger;
	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
	USphereComponent* bomb_range;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	inline float getPlantingTime() { return planting_time; }
	
};
