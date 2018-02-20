// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "VUEImporterStyle.h"

class FVUEImporterCommands : public TCommands<FVUEImporterCommands>
{
public:

	FVUEImporterCommands()
		: TCommands<FVUEImporterCommands>(TEXT("VUEImporter"), NSLOCTEXT("Contexts", "VUEImporter", "VUEImporter Plugin"), NAME_None, FVUEImporterStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};