#include "VUEHelper.h"

#include "Developer/DesktopPlatform/Public/IDesktopPlatform.h"
#include "Developer/DesktopPlatform/Public/DesktopPlatformModule.h"

FVector VUEHelper::VUEVector3DToFVector(const VUEParserW::Math::Vector3D* Vector)
{
	double X = Vector->X;
	double Y = Vector->Y;
	double Z = Vector->Z;

	if (X < 0.001 && X > -0.001)
	{
		X = 0;
	}

	if (Y < 0.001 && Y > -0.001)
	{
		Y = 0;
	}

	if (Z < 0.001 && Z > -0.001)
	{
		Z = 0;
	}

	return FVector(X, Y, Z);
}

FVector VUEHelper::VUEVector3DToFVector(const VUEParserW::Math::Vector3D Vector)
{
	double X = Vector.X;
	double Y = Vector.Y;
	double Z = Vector.Z;

	if (X < 0.001 && X > -0.001)
	{
		X = 0;
	}

	if (Y < 0.001 && Y > -0.001)
	{
		Y = 0;
	}

	if (Z < 0.001 && Z > -0.001)
	{
		Z = 0;
	}

	return FVector(X, Y, Z);
}

ULineBatchComponent* VUEHelper::GetLineBatcher()
{
	return GetWorld()->PersistentLineBatcher;
}

void VUEHelper::FlushLineBatcher()
{
	GetLineBatcher()->Flush();
}

UWorld* VUEHelper::GetWorld()
{
	return GEditor->GetEditorWorldContext().World();
}

void VUEHelper::DrawLine(FVector const& LineStart, FVector const& LineEnd, FLinearColor const& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority, float Thickness)
{
	// no debug line drawing on dedicated server
	if (GEngine->GetNetMode(GetWorld()) != NM_DedicatedServer)
	{
		// this means foreground lines can't be persistent 
		ULineBatchComponent* const LineBatcher = GetLineBatcher();
		if (LineBatcher != NULL)
		{
			float const LineLifeTime = (LifeTime > 0.f) ? LifeTime : LineBatcher->DefaultLifeTime;
			LineBatcher->DrawLine(LineStart, LineEnd, Color, DepthPriority, Thickness, LineLifeTime);
		}
	}
}

void VUEHelper::DrawLine(FVector const& LineStart, FVector const& LineEnd, FLinearColor const& Color)
{
	DrawLine(LineStart, LineEnd, Color, true, -1.f, 0, 0.f);
}

void VUEHelper::DrawPoint(FVector const& Postion, FLinearColor const& Color)
{
	GetLineBatcher()->DrawPoint(Postion, Color, 5.0f, 0, -1.f);
}

bool VUEHelper::OpenVUEFileDialog(TArray<FString>& OutFiles)
{
	void* ParentWindowPtr = FSlateApplication::Get().GetActiveTopLevelWindow()->GetNativeWindow()->GetOSWindowHandle();

	return (FDesktopPlatformModule::Get()->OpenFileDialog(
		ParentWindowPtr,
		TEXT("Import Smartplant file"),
		TEXT("C:/Users/LeGone/Desktop/NW_VUE_Reader"),
		TEXT("Test.vue"),
		TEXT("Smartplant 3D (*.vue *.zvf)|*.vue;*.zvf"),
		EFileDialogFlags::None,
		OutFiles
	));
}