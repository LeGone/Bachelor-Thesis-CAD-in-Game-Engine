// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "VUEImporter.h"
#include "VUEActor.h"
#include "VUETriangulation.h"
#include "VUEGlobals.h"
#include "VUEHelper.h"

#include "IPluginManager.h"
#include "DrawDebugHelpers.h"


/*
* Macro to reset the geometry arrays
*/
#define RESETSECTIONARRAYS \
		Vertices.Empty(); \
		Triangles.Empty(); \
		VertexColors.Empty(); \
		VUETriangulation::ResetIndexOfVertices();

void VUEImporter::ImportVUEFile(FString Filename)
{
	UWorld* World = VUEHelper::GetWorld();
	VUEParserW::VUEParser VUEParser;

	// Start the parse process
	VUEParser.Parse(*Filename);

	// We need the folder path later
	FString FolderPath = VUEParser.AHeader->NameOfDocument;

	AVUEHeader* VUEHeader = CreateHeader(VUEParser.AHeader, Filename);
	CreateMaterials(VUEParser, VUEHeader);

	// UE4 needs a few values to spawn an actor
	FVector Location(0, 0, 0);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Start the ui that displays the current progress as a progressbar
	FScopedSlowTask SlowTask(VUEParser.ListOfGeometriesCount, FText::FromString("Importing geometry"));
	SlowTask.MakeDialog(true, true);

	bool bEnableCollision = false;

	for (int ContainerIndex = 0; ContainerIndex < VUEParser.ListOfGeometriesCount; ContainerIndex++)
	{
		VUEParserW::GeometryObject* GeometryObject = VUEParser.ListOfGeometries[ContainerIndex];

		SlowTask.EnterProgressFrame(1.0F, FText::FromString(GeometryObject->UID));

		TArray<FVector> Vertices;
		TArray<int32> Triangles;
		TArray<FColor> VertexColors;

		// Spawn the Container-Actor. Every container contains the geometry and the meta data, that comes from the vueparser
		AVUEActor* SpawnedActor = (AVUEActor*)World->SpawnActor<AVUEActor>(AVUEActor::StaticClass(), Location, Rotation, SpawnInfo);

		SpawnedActor->Rename(GeometryObject->UID);
		SpawnedActor->SetActorLabel(GeometryObject->UID);
		SpawnedActor->SetFolderPath(*FolderPath);

		SpawnedActor->VUEHeader = VUEHeader;
		SpawnedActor->DefinitionType = (EDefinitionType)GeometryObject->Definition;
		SpawnedActor->SuperClass = (EDefinitionSuperClasses)GeometryObject->SuperClass;
		SpawnedActor->ContainerType = (EContainerType)GeometryObject->ContainerType;
		SpawnedActor->File = GeometryObject->File->Name;

		VUEParserW::Material* VUEMaterial = GeometryObject->Material;
		UMaterialInstanceDynamic* Material = VUEHeader->Materials[GeometryObject->Material->SimpleIndex];

		for (unsigned int GroupIndex = 0; GroupIndex < GeometryObject->ListOfGroupsCount; GroupIndex++)
		{
			VUEParserW::GeometryGroup* GeometryGroup = GeometryObject->ListOfGroups[GroupIndex];

			// Whenever there is a display aspect, store it into the container-actor
			if (GeometryGroup->DisplayAspect != VUEParserW::DisplayAspectType::None)
			{
				SpawnedActor->ContainedDisplayAspect.Add((EDisplayAspectType)GeometryGroup->DisplayAspect);
			}
				
			if (GeometryGroup->DisplayAspect == VUEParserW::DisplayAspectType::None
				|| GeometryGroup->DisplayAspect == VUEParserW::DisplayAspectType::SimplePhysical
				|| GeometryGroup->DisplayAspect == VUEParserW::DisplayAspectType::DetailedPhysical)
			{
				bEnableCollision = true;
			}
			else
			{
				bEnableCollision = false;
			}

			for (unsigned int PrimitiveIndex = 0; PrimitiveIndex < GeometryGroup->PrimitiveCount; PrimitiveIndex++)
			{
				VUEParserW::GeometryPrimitive* GeometryPrimitive = GeometryGroup->ListOfPrimitives[PrimitiveIndex];

				int bError = false;

				// Only possible if we are on primitive depth generation of geometries. We need material one unreal material per vue material.
				if (GeometryPrimitive->Material && VUEGlobals::CreateGeometryDepthLayer == VUEGlobals::ECreateGeometryDepthLayer::Primitive)
				{
					VUEMaterial = GeometryPrimitive->Material;
					Material = VUEHeader->Materials[GeometryPrimitive->Material->SimpleIndex];
				}
				else
				{
					VUEMaterial = GeometryObject->Material;
					Material = VUEHeader->Materials[GeometryObject->Material->SimpleIndex];
				}

				switch (GeometryPrimitive->Type)
				{
					// Todo: Line3D->StartLength ignored!
					case VUEParserW::ContainerType::Line3D:
					{
						VUEParserW::Primitive::Line3D* Line3D = static_cast<VUEParserW::Primitive::Line3D*>(GeometryPrimitive);

						VUEParserW::Math::Vector3D InLocation = Line3D->Position;
						VUEParserW::Math::Vector3D InDirection = Line3D->Direction;

						float EndLength = Line3D->EndLength * VUEGlobals::ScaleFactor;
						VUEParserW::Color InColor = VUEMaterial->DiffuseColor;

						FVector StartLocation = VUEHelper::VUEVector3DToFVector(InLocation) * VUEGlobals::ScaleFactor;
						FVector Direction = VUEHelper::VUEVector3DToFVector(InDirection) * EndLength;
						FVector EndLocation = StartLocation + Direction;

						VUEHelper::GetLineBatcher()->DrawLine(StartLocation, EndLocation, FLinearColor(InColor.R, InColor.G, InColor.B, InColor.A), 0, 0.f, -1.f);
						break;
					}

					case VUEParserW::ContainerType::LineString3D:
					{
						VUEParserW::Primitive::LineString3D* LineString3D = static_cast<VUEParserW::Primitive::LineString3D*>(GeometryPrimitive);

						if (LineString3D->ListOfVerticesCount > 1)
						{
							VUEParserW::Color InColor = VUEMaterial->DiffuseColor;
							FLinearColor Color = FLinearColor(InColor.R, InColor.G, InColor.B, InColor.A);

							TArray<FBatchedLine> Lines;
							for (int Index = 1; Index < LineString3D->ListOfVerticesCount; Index++)
							{
								FBatchedLine BatchedLine;
								BatchedLine.Color = Color;
								BatchedLine.DepthPriority = 0;
								BatchedLine.End = VUEHelper::VUEVector3DToFVector(LineString3D->ListOfVertices[Index-1]) * VUEGlobals::ScaleFactor;
								BatchedLine.Start = VUEHelper::VUEVector3DToFVector(LineString3D->ListOfVertices[Index]) * VUEGlobals::ScaleFactor;
								Lines.Add(BatchedLine);
							}

							VUEHelper::GetLineBatcher()->DrawLines(Lines);
						}

						break;
					}

					case VUEParserW::ContainerType::Point3D:
					{
						VUEParserW::Primitive::Point3D* Point3D = static_cast<VUEParserW::Primitive::Point3D*>(GeometryPrimitive);

						VUEParserW::Math::Vector3D InLocation = Point3D->Position;
						VUEParserW::Color InColor = VUEMaterial->DiffuseColor;

						FVector StartLocation = VUEHelper::VUEVector3DToFVector(InLocation) * VUEGlobals::ScaleFactor;

						VUEHelper::GetLineBatcher()->DrawPoint(StartLocation, FLinearColor(InColor.R, InColor.G, InColor.B, InColor.A), 5.0f, 0, -1.f);

						break;
					}

					case VUEParserW::ContainerType::Plane3D:
					{
						VUEParserW::Primitive::Plane3D* Plane3D = static_cast<VUEParserW::Primitive::Plane3D*>(GeometryPrimitive);

						FVector Normal = VUEHelper::VUEVector3DToFVector(Plane3D->Direction2);

						for (unsigned int WireIndex = 0; WireIndex < Plane3D->WireCount; WireIndex++)
						{
							VUEParserW::Primitive::PrimitiveContainer* Wire = Plane3D->Wires[WireIndex];

							TArray<FVector> Plane3DVertices;
							TArray<int32> Plane3DIndices;

							for (unsigned int WirePrimitiveIndex = 0; WirePrimitiveIndex < Wire->PrimitivesCount; WirePrimitiveIndex++)
							{
								VUEParserW::GeometryPrimitive* SubPrimitive = Wire->Primitives[WirePrimitiveIndex];
									
								if (SubPrimitive->Type == VUEParserW::ContainerType::Line3D)
								{
									VUEParserW::Primitive::Line3D* Line3D = static_cast<VUEParserW::Primitive::Line3D*>(SubPrimitive);
									FVector Position = VUEHelper::VUEVector3DToFVector(Line3D->Position);
									Plane3DVertices.Add(Position * VUEGlobals::ScaleFactor);
								}
								else
								{
									bError = true;
									break;
								}
							}

							if (!bError)
							{
								if (Plane3DVertices.Num() >= 3) // We need at least 3 Points to triangulate
								{
									VUETriangulation::TriangulatePolygon(Plane3DVertices, Plane3DIndices, Normal);

									if (Plane3DIndices.Num()) // Num() > 0 if triangulation was successful
									{
										// Copy vertices
										Vertices.Append(Plane3DVertices);
										Triangles.Append(Plane3DIndices);
									}
									else
									{
										bError = true;
										break;
									}
								}
								else
								{
									bError = true;
									break;
								}
							}
						}
						break;
					}

					case VUEParserW::ContainerType::Sphere3D:
					{
						VUEParserW::Primitive::Sphere3D* Sphere3D = static_cast<VUEParserW::Primitive::Sphere3D*>(GeometryPrimitive);

						VUETriangulation::Sphere(Vertices, Triangles, VertexColors, VUEHelper::VUEVector3DToFVector(Sphere3D->Position), Sphere3D->Radius);

						break;
					}
					
					case VUEParserW::ContainerType::Torus3D:
					{
						VUEParserW::Primitive::Torus3D* Torus3D = static_cast<VUEParserW::Primitive::Torus3D*>(GeometryPrimitive);

						if (!VUETriangulation::Torus3D(Torus3D, Vertices, Triangles, VertexColors))
						{
							bError = true;
						}

						break;
					}
					
					case VUEParserW::ContainerType::Projection3D:
					{
						VUEParserW::Primitive::Projection3D* Projection3D = static_cast<VUEParserW::Primitive::Projection3D*>(GeometryPrimitive);

						if (!VUETriangulation::Projection3D(Projection3D, Vertices, Triangles))
						{
							bError = true;
						}

						break;
					}

					case VUEParserW::ContainerType::Revolution3D:
					{
						VUEParserW::Primitive::Revolution3D* Revolution3D = static_cast<VUEParserW::Primitive::Revolution3D*>(GeometryPrimitive);

						if (!VUETriangulation::Revolution3D(Revolution3D, Vertices, Triangles))
						{
							bError = true;
						}

						break;
					}

					default:
						bError = true;
						break;
				}

				// Draw boundingbox on error?
				if (bError && VUEGlobals::bDrawBoundingBoxOnError)
				{
					VUETriangulation::Box(Vertices, Triangles, VertexColors, FBox(FVector(GeometryPrimitive->BoundingBox.M11, GeometryPrimitive->BoundingBox.M12, GeometryPrimitive->BoundingBox.M13), FVector(GeometryPrimitive->BoundingBox.M21, GeometryPrimitive->BoundingBox.M22, GeometryPrimitive->BoundingBox.M23)));
				}

				if (VUEGlobals::CreateGeometryDepthLayer == VUEGlobals::ECreateGeometryDepthLayer::Primitive)
				{
					if (Vertices.Num())
					{
						// Override material to use the error material? Does only work on primitive depth!
						if (bError && VUEGlobals::bUseTransparency)
						{
							Material = VUEHeader->Materials[VUEHeader->Materials.Num() - 1]; // The last material is for errors
						}

						SpawnedActor->AddPrimitive(Vertices, Triangles, VertexColors, Material, VUEGlobals::bCollision && bEnableCollision, VUEGlobals::bCalculateNormalsAndTangents);
					}

					RESETSECTIONARRAYS
				}
			}

			if (VUEGlobals::CreateGeometryDepthLayer == VUEGlobals::ECreateGeometryDepthLayer::Group)
			{
				if (Vertices.Num())
				{
					SpawnedActor->AddPrimitive(Vertices, Triangles, VertexColors, Material, VUEGlobals::bCollision && bEnableCollision, VUEGlobals::bCalculateNormalsAndTangents);
				}

				RESETSECTIONARRAYS
			}
		}

		if (VUEGlobals::CreateGeometryDepthLayer == VUEGlobals::ECreateGeometryDepthLayer::Container)
		{
			if (Vertices.Num())
			{
				SpawnedActor->AddPrimitive(Vertices, Triangles, VertexColors, Material, VUEGlobals::bCollision, VUEGlobals::bCalculateNormalsAndTangents);
			}

			RESETSECTIONARRAYS
		}
	}
}

AVUEHeader* VUEImporter::CreateHeader(VUEParserW::Header* IncomingHeader, FString FullFilePath)
{
	FString HeaderName = FString(".Header-") + IncomingHeader->NameOfDocument;

	FVector Location(0, 0, 0);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AVUEHeader* VUEHeader = (AVUEHeader*)GEditor->GetEditorWorldContext().World()->SpawnActor<AVUEHeader>(AVUEHeader::StaticClass(), Location, Rotation, SpawnInfo);
	VUEHeader->FullFilePath = FullFilePath;

	VUEHeader->Rename(*HeaderName); // That is not working. Could be a bug, so let's not remove it, yet.
	VUEHeader->SetActorLabel(*HeaderName);
	VUEHeader->SetFolderPath(IncomingHeader->NameOfDocument); // Use the folder to make use of the treeview

	VUEHeader->TargetSoftware = IncomingHeader->TargetSoftware;
	VUEHeader->SourceSoftware = IncomingHeader->SourceSoftware;
	VUEHeader->VUEVersion     = IncomingHeader->VUEVersion;
	VUEHeader->FileType       = IncomingHeader->FileType;
	VUEHeader->ModelID        = IncomingHeader->ModelID;
	VUEHeader->NameOfDocument = IncomingHeader->NameOfDocument;
	VUEHeader->RootFileName   = IncomingHeader->RootFileName;
	VUEHeader->SourcePath     = IncomingHeader->SourcePath;
	VUEHeader->Classname      = IncomingHeader->Classname;
	VUEHeader->FileSize       = IncomingHeader->FileSize;
	VUEHeader->NumberOfUIDs   = IncomingHeader->NumberOfUIDs;
	VUEHeader->NumberOfViews  = IncomingHeader->NumberOfViews;
	VUEHeader->NumberOfFiles  = IncomingHeader->NumberOfFiles;
	VUEHeader->NumberOfParts  = IncomingHeader->NumberOfParts;

	return VUEHeader;
}

void VUEImporter::CreateMaterials(VUEParserW::VUEParser& VUEParserInstance, AVUEHeader* VUEHeaderInstance)
{
	// Get base materials. Used to create instances.
	UMaterial* BaseMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("Material'/Game/VUEImporter/VUEMaterial.VUEMaterial'")));
	UMaterial* MaterialTranslucent = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("Material'/Game/VUEImporter/VUEMaterialTranslucent.VUEMaterialTranslucent'")));

	// Read materials from the parser and generate converted instances for the engine
	for (int MaterialIndex = 0; MaterialIndex < VUEParserInstance.ListOfMaterialsCount; MaterialIndex++)
	{
		VUEParserW::Material* InMaterial = VUEParserInstance.ListOfMaterials[MaterialIndex];
		UMaterialInstanceDynamic* MaterialInstance;

		VUEParserW::Color AmbientColor = InMaterial->AmbientColor;
		VUEParserW::Color DiffuseColor = InMaterial->DiffuseColor;
		VUEParserW::Color SpecularColor = InMaterial->SpecularColor;

		if (DiffuseColor.A < 1.0f && VUEGlobals::bUseTransparency)
		{
			MaterialInstance = UMaterialInstanceDynamic::Create(MaterialTranslucent, NULL);
		}
		else
		{
			MaterialInstance = UMaterialInstanceDynamic::Create(BaseMaterial, NULL);
		}
		
		// MaterialInstance->Rename(*FString::Printf(TEXT("Material %s [%i]"), InMaterial->File->Name, InMaterial->SimpleIndex));

		MaterialInstance->SetVectorParameterValue("AmbientColor", FLinearColor(AmbientColor.R, AmbientColor.G, AmbientColor.B, AmbientColor.A));
		MaterialInstance->SetVectorParameterValue("DiffuseColor", FLinearColor(DiffuseColor.R, DiffuseColor.G, DiffuseColor.B, DiffuseColor.A));
		MaterialInstance->SetVectorParameterValue("SpecularColor", FLinearColor(SpecularColor.R, SpecularColor.G, SpecularColor.B, SpecularColor.A));

		VUEHeaderInstance->Materials.Add(MaterialInstance);
	}

	// Create translucent material for errors (boundingboxes). This is always the last in the material array. This only works on the primitive depth!
	UMaterial* MaterialError = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("Material'/Game/VUEImporter/VUEMaterialError.VUEMaterialError'")));
	UMaterialInstanceDynamic* MaterialErrorInstance = UMaterialInstanceDynamic::Create(MaterialError, NULL);
	//MaterialErrorInstance->Rename(TEXT("MaterialErrorInstance"));
	VUEHeaderInstance->Materials.Add(MaterialErrorInstance);
}