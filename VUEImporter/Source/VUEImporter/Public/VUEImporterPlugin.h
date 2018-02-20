// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ModuleManager.h"
#include "VUEHeader.h"

#include "AllowWindowsPlatformTypes.h"
#define byte char
#include "../VUEParserCPPWrapper/VUEParserCPPWrapper.hpp"
#include "HideWindowsPlatformTypes.h"

class FToolBarBuilder;
class FMenuBuilder;

class FVUEImporterModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();
	
private:

	TSharedPtr<class FUICommandList> PluginCommands;

	FReply OnClickedImportButton();
	FReply OnClickedFlushLinesAndPointsButton();

	void AddToolbarExtension(FToolBarBuilder& Builder);
	void AddMenuExtension(FMenuBuilder& Builder);

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);
};