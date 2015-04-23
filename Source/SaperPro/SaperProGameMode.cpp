// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SaperPro.h"
#include "SaperProGameMode.h"
#include "SaperProCharacter.h"

ASaperProGameMode::ASaperProGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
