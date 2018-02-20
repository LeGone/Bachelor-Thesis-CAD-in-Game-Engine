#pragma once

#include "VUEImporterPlugin.h"
#include "Vector.h"
#include "LevelEditor.h"
#include "Editor.h"
#include "Runtime/Engine/Classes/Components/LineBatchComponent.h"

class VUEHelper
{
public:

	static FVector VUEVector3DToFVector(const VUEParserW::Math::Vector3D* Vector);
	static FVector VUEVector3DToFVector(const VUEParserW::Math::Vector3D Vector);
	static ULineBatchComponent* GetLineBatcher();
	static UWorld* GetWorld();
	static void FlushLineBatcher();
	static void DrawLine(FVector const& LineStart, FVector const& LineEnd, FLinearColor const& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority, float Thickness);
	static void DrawLine(FVector const& LineStart, FVector const& LineEnd, FLinearColor const& Color = FLinearColor::White);
	static void DrawPoint(FVector const& Postion, FLinearColor const& Color = FLinearColor::White);

	static bool OpenVUEFileDialog(TArray<FString>& OutFiles);
};