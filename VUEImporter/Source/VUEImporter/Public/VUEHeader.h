#pragma once

#include "VUEImporterPlugin.h"
#include "VUEHeader.generated.h"

UCLASS(config = Editor)
class AVUEHeader : public AActor
{
	GENERATED_BODY()

protected:
	
	virtual void BeginPlay();

public:

	UPROPERTY(VisibleAnywhere)
	FString TargetSoftware;

	UPROPERTY(VisibleAnywhere)
	FString SourceSoftware;

	UPROPERTY(VisibleAnywhere)
	FString VUEVersion;

	UPROPERTY(VisibleAnywhere)
	FString FileType;

	UPROPERTY(VisibleAnywhere)
	FString ModelID;

	UPROPERTY(VisibleAnywhere)
	FString NameOfDocument;

	UPROPERTY(VisibleAnywhere)
	FString RootFileName;

	UPROPERTY(VisibleAnywhere)
	FString SourcePath;

	UPROPERTY(VisibleAnywhere)
	FString Classname;

	UPROPERTY(VisibleAnywhere)
	int FileSize;

	UPROPERTY(VisibleAnywhere)
	int NumberOfUIDs;

	UPROPERTY(VisibleAnywhere)
	int NumberOfViews;

	UPROPERTY(VisibleAnywhere)
	int NumberOfFiles;

	UPROPERTY(VisibleAnywhere)
	int NumberOfParts;

	UPROPERTY(VisibleAnywhere)
	TArray<UMaterialInstanceDynamic*> Materials;

	FString FullFilePath;

	AVUEHeader();
};