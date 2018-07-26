// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "PixelateGameMode.h"
#include "PixelatePawn.h"

APixelateGameMode::APixelateGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = APixelatePawn::StaticClass();
}

