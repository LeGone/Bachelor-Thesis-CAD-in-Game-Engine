#include "VUEHeader.h"
#include "VUEGlobals.h"
#include "VUEImporter.h"

AVUEHeader::AVUEHeader()
{
}

void AVUEHeader::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	/*
	VUEImporter* VUEImporterInstance = new VUEImporter();
	VUEImporterInstance->ImportVUEFile(FullFilePath);
	delete VUEImporterInstance;
	*/
}