// Copyright Epic Games, Inc. All Rights Reserved.

#include "PersonalProj01_GP4GameMode.h"
#include "PersonalProj01_GP4Character.h"
#include "UObject/ConstructorHelpers.h"

APersonalProj01_GP4GameMode::APersonalProj01_GP4GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
