// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "VUEImporterCommands.h"

#define LOCTEXT_NAMESPACE "FVUEImporterModule"

void FVUEImporterCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "VUEImporter", "Bring up VUEImporter window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
