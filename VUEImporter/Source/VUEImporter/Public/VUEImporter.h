// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ModuleManager.h"
#include "VUEHeader.h"

// The parser needs things from windows api?
#include "AllowWindowsPlatformTypes.h"
#define byte char
#include "../VUEParserCPPWrapper/VUEParserCPPWrapper.hpp"
#include "HideWindowsPlatformTypes.h"

class VUEImporter
{
public:

	void ImportVUEFile(FString Filename);

private:


	AVUEHeader* CreateHeader(VUEParserW::Header* Header, FString FullFilePath);
	void CreateMaterials(VUEParserW::VUEParser& VUEParserInstance, AVUEHeader* VUEHeaderInstance);
};