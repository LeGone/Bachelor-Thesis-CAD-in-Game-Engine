#include "VUEActor.h"

AVUEActor::AVUEActor()
{
	Mesh = CreateDefaultSubobject<URuntimeMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = Mesh;
	Mesh->bCastDynamicShadow = false;
	Mesh->bUseAsyncCooking = true;
	SectionIndex = 0;
}

void AVUEActor::AddPrimitive(const TArray<FVector>& Vertices, const TArray<int32>& Triangles, const TArray<FColor>& VertexColors, UMaterialInstanceDynamic* Material, const bool bEnableCollision, const bool bCalculateNormalsAndTangents)
{
	if (Vertices.Num() > 0 && Triangles.Num() > 0)
	{
		TArray<FVector> Normals;
		TArray<FRuntimeMeshTangent> Tangents;
		TArray<FVector2D> TextureCoordinates;
		ESectionUpdateFlags SectionUpdateFlags = ESectionUpdateFlags::None;

		if (bCalculateNormalsAndTangents)
		{
			SectionUpdateFlags |= ESectionUpdateFlags::CalculateNormalTangent;
		}

		Mesh->CreateMeshSection(SectionIndex, Vertices, Triangles, Normals, TextureCoordinates, VertexColors, Tangents, bEnableCollision, EUpdateFrequency::Infrequent, SectionUpdateFlags);
		Mesh->SetMaterial(SectionIndex, Cast<UMaterialInterface>(Material));

		SectionIndex++;

		Mesh->ShouldGenerateAutoLOD();
	}
}