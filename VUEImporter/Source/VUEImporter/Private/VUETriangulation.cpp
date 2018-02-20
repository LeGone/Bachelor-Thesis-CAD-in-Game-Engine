/*
 * VUETriangulation
 *
 * Contains all the code needed to generate triangles out of primitives.
 * IndexOfVertices stores a global index of how many vertices exist in the current actor.
 *
 */

#include "VUETriangulation.h"
#include "VUEGlobals.h"
#include "VUEHelper.h"

#include "../EarClipperWrapper/EarClipperWrapper.hpp"

int VUETriangulation::IndexOfVertices;
void VUETriangulation::ResetIndexOfVertices()
{
	IndexOfVertices = 0;
}

void VUETriangulation::TriangulatePolygon(TArray<FVector>& InVertices, TArray<int32>& OutIndices, FVector Normal)
{
	EarClipperW::EarClipper EarClipper;

	// Feed the EarClipper library with our vertices.
	for (int Index = 0; Index < InVertices.Num(); Index++)
	{
		FVector& Vertex = InVertices[Index];
		EarClipper.AddVertex(Vertex.X, Vertex.Y, Vertex.Z);
	}

	// EarClipper needs to know what perspective it has to triangulate.
	EarClipper.SetNormal(Normal.X, Normal.Y, Normal.Z);

	EarClipper.Triangulate();

	// If the result is valid and we have at least one triangle, return the indices.
	if (EarClipper.ResultIndices && EarClipper.ResultIndicesCount >= 3) // At least one triangle
	{
		for (short Index = 0; Index < EarClipper.ResultIndicesCount; Index++)
		{
			OutIndices.Add(IndexOfVertices + EarClipper.ResultIndices[Index]);
		}

		IndexOfVertices += InVertices.Num();
	}
}

void VUETriangulation::Box(TArray<FVector>& OutVertices, TArray<int32>& OutIndices, TArray<FColor>& OutVertexColors, FBox BoxShape)
{
	BoxShape.Min *= VUEGlobals::ScaleFactor;
	BoxShape.Max *= VUEGlobals::ScaleFactor;

	OutVertices.Add(FVector(BoxShape.Min.X, BoxShape.Min.Y, BoxShape.Min.Z));
	OutVertices.Add(FVector(BoxShape.Min.X, BoxShape.Max.Y, BoxShape.Min.Z));
	OutVertices.Add(FVector(BoxShape.Min.X, BoxShape.Max.Y, BoxShape.Max.Z));
	OutVertices.Add(FVector(BoxShape.Min.X, BoxShape.Min.Y, BoxShape.Max.Z));

	OutIndices.Add(IndexOfVertices);
	OutIndices.Add(IndexOfVertices + 1);
	OutIndices.Add(IndexOfVertices + 2);

	OutIndices.Add(IndexOfVertices + 2);
	OutIndices.Add(IndexOfVertices + 3);
	OutIndices.Add(IndexOfVertices);

	OutVertices.Add(FVector(BoxShape.Min.X, BoxShape.Min.Y, BoxShape.Min.Z));
	OutVertices.Add(FVector(BoxShape.Max.X, BoxShape.Min.Y, BoxShape.Min.Z));
	OutVertices.Add(FVector(BoxShape.Max.X, BoxShape.Max.Y, BoxShape.Min.Z));
	OutVertices.Add(FVector(BoxShape.Min.X, BoxShape.Max.Y, BoxShape.Min.Z));

	IndexOfVertices += 4;

	OutIndices.Add(IndexOfVertices);
	OutIndices.Add(IndexOfVertices + 1);
	OutIndices.Add(IndexOfVertices + 2);

	OutIndices.Add(IndexOfVertices + 2);
	OutIndices.Add(IndexOfVertices + 3);
	OutIndices.Add(IndexOfVertices);

	OutVertices.Add(FVector(BoxShape.Min.X, BoxShape.Min.Y, BoxShape.Max.Z));
	OutVertices.Add(FVector(BoxShape.Min.X, BoxShape.Max.Y, BoxShape.Max.Z));
	OutVertices.Add(FVector(BoxShape.Max.X, BoxShape.Max.Y, BoxShape.Max.Z));
	OutVertices.Add(FVector(BoxShape.Max.X, BoxShape.Min.Y, BoxShape.Max.Z));

	IndexOfVertices += 4;

	OutIndices.Add(IndexOfVertices);
	OutIndices.Add(IndexOfVertices + 1);
	OutIndices.Add(IndexOfVertices + 2);

	OutIndices.Add(IndexOfVertices + 2);
	OutIndices.Add(IndexOfVertices + 3);
	OutIndices.Add(IndexOfVertices);

	OutVertices.Add(FVector(BoxShape.Min.X, BoxShape.Min.Y, BoxShape.Min.Z));
	OutVertices.Add(FVector(BoxShape.Min.X, BoxShape.Min.Y, BoxShape.Max.Z));
	OutVertices.Add(FVector(BoxShape.Max.X, BoxShape.Min.Y, BoxShape.Max.Z));
	OutVertices.Add(FVector(BoxShape.Max.X, BoxShape.Min.Y, BoxShape.Min.Z));

	IndexOfVertices += 4;

	OutIndices.Add(IndexOfVertices);
	OutIndices.Add(IndexOfVertices + 1);
	OutIndices.Add(IndexOfVertices + 2);

	OutIndices.Add(IndexOfVertices + 2);
	OutIndices.Add(IndexOfVertices + 3);
	OutIndices.Add(IndexOfVertices);

	OutVertices.Add(FVector(BoxShape.Min.X, BoxShape.Max.Y, BoxShape.Min.Z));
	OutVertices.Add(FVector(BoxShape.Max.X, BoxShape.Max.Y, BoxShape.Min.Z));
	OutVertices.Add(FVector(BoxShape.Max.X, BoxShape.Max.Y, BoxShape.Max.Z));
	OutVertices.Add(FVector(BoxShape.Min.X, BoxShape.Max.Y, BoxShape.Max.Z));

	IndexOfVertices += 4;

	OutIndices.Add(IndexOfVertices);
	OutIndices.Add(IndexOfVertices + 1);
	OutIndices.Add(IndexOfVertices + 2);

	OutIndices.Add(IndexOfVertices + 2);
	OutIndices.Add(IndexOfVertices + 3);
	OutIndices.Add(IndexOfVertices);

	OutVertices.Add(FVector(BoxShape.Max.X, BoxShape.Max.Y, BoxShape.Max.Z));
	OutVertices.Add(FVector(BoxShape.Max.X, BoxShape.Max.Y, BoxShape.Min.Z));
	OutVertices.Add(FVector(BoxShape.Max.X, BoxShape.Min.Y, BoxShape.Min.Z));
	OutVertices.Add(FVector(BoxShape.Max.X, BoxShape.Min.Y, BoxShape.Max.Z));

	IndexOfVertices += 4;

	OutIndices.Add(IndexOfVertices);
	OutIndices.Add(IndexOfVertices + 1);
	OutIndices.Add(IndexOfVertices + 2);

	OutIndices.Add(IndexOfVertices + 2);
	OutIndices.Add(IndexOfVertices + 3);
	OutIndices.Add(IndexOfVertices);

	IndexOfVertices += 4;

	for (int I = 0; I < 24; I++)
	{
		OutVertexColors.Add(FColor::White);
	}
}

void VUETriangulation::Sphere(TArray<FVector>& OutVertices, TArray<int32>& OutIndices, TArray<FColor>& OutVertexColors, FVector Position, float Radius)
{
	// The first/last arc are on top of each other.
	int32 NumVerts = (VUEGlobals::SphereNumberOfEdges + 1) * (VUEGlobals::SphereNumberOfEdges + 1);
	FDynamicMeshVertex* Verts = (FDynamicMeshVertex*)FMemory::Malloc(NumVerts * sizeof(FDynamicMeshVertex));

	// Calculate verts for one arc
	FDynamicMeshVertex* ArcVerts = (FDynamicMeshVertex*)FMemory::Malloc((VUEGlobals::SphereNumberOfEdges + 1) * sizeof(FDynamicMeshVertex));

	for (int32 Index = 0; Index < VUEGlobals::SphereNumberOfEdges + 1; Index++)
	{
		FDynamicMeshVertex* ArcVert = &ArcVerts[Index];

		float Angle = ((float)Index / VUEGlobals::SphereNumberOfEdges) * PI;

		// Note- unit sphere, so position always has mag of one. We can just use it for normal!			
		ArcVert->Position.X = 0.0f;
		ArcVert->Position.Y = FMath::Sin(Angle);
		ArcVert->Position.Z = FMath::Cos(Angle);
	}

	// Then rotate this arc NumSides+1 times.
	for (int32 S = 0; S < VUEGlobals::SphereNumberOfEdges + 1; S++)
	{
		FRotator ArcRotator(0, 360.f * (float)S / VUEGlobals::SphereNumberOfEdges, 0);
		FRotationMatrix ArcRot(ArcRotator);

		for (int32 V = 0; V<VUEGlobals::SphereNumberOfEdges + 1; V++)
		{
			int32 VIx = (VUEGlobals::SphereNumberOfEdges + 1)*S + V;

			Verts[VIx].Position = ArcRot.TransformPosition(ArcVerts[V].Position);
		}
	}

	// Add all of the vertices we generated to the mesh builder.
	for (int32 VertIdx = 0; VertIdx < NumVerts; VertIdx++)
	{
		OutVertices.Add(Verts[VertIdx].Position * (Radius * VUEGlobals::ScaleFactor) + Position * VUEGlobals::ScaleFactor);
		OutVertexColors.Add(Verts[VertIdx].Color);
	}

	// Add all of the OutIndices we generated to the mesh builder.
	for (int32 S = 0; S < VUEGlobals::SphereNumberOfEdges; S++)
	{
		int32 A0Start = (S + 0) * (VUEGlobals::SphereNumberOfEdges + 1);
		int32 A1Start = (S + 1) * (VUEGlobals::SphereNumberOfEdges + 1);

		for (int32 R = 0; R<VUEGlobals::SphereNumberOfEdges; R++)
		{
			OutIndices.Add(IndexOfVertices + A0Start + R);
			OutIndices.Add(IndexOfVertices + A1Start + R);
			OutIndices.Add(IndexOfVertices + A0Start + R + 1);

			OutIndices.Add(IndexOfVertices + A1Start + R);
			OutIndices.Add(IndexOfVertices + A1Start + R + 1);
			OutIndices.Add(IndexOfVertices + A0Start + R + 1);
		}
	}

	IndexOfVertices += NumVerts;

	// Free our local copy of verts and arc verts
	FMemory::Free(Verts);
	FMemory::Free(ArcVerts);
}

void VUETriangulation::Torus(TArray<FVector>& OutVertices, TArray<int32>& OutIndices, TArray<FColor>& OutVertexColors, FVector Position, FVector Orientation1, FVector Orientation2, int Radius, int Thickness)
{
	// Calculate 
	float RadiusStepAngle = 360 / VUEGlobals::TorusNumberOfEdges;
	float CircleStepAngle = 360 / VUEGlobals::TorusNumberOfCircleEdges;
	float RadiusCurrentAngle = 0;

	for (int CurrentRadiusStep = 0; CurrentRadiusStep < VUEGlobals::TorusNumberOfEdges; CurrentRadiusStep++)
	{
		FVector FinalRadiusPosition = Orientation2.RotateAngleAxis(RadiusCurrentAngle, Orientation1);
		FinalRadiusPosition = FinalRadiusPosition * Radius + Position;

		FVector DirectionFromCenterToRadius = FinalRadiusPosition - Position;
		DirectionFromCenterToRadius.Normalize();
		FVector FinalRadiusDirection = FVector::CrossProduct(Orientation1, DirectionFromCenterToRadius);

		RadiusCurrentAngle += RadiusStepAngle;

		float CircleCurrentAngle = 0;
		for (int CurrentCircleStep = 0; CurrentCircleStep < VUEGlobals::TorusNumberOfCircleEdges; CurrentCircleStep++)
		{
			FVector FinalCirclePosition = DirectionFromCenterToRadius.RotateAngleAxis(CircleCurrentAngle, FinalRadiusDirection);
			FinalCirclePosition = FinalCirclePosition * Thickness + FinalRadiusPosition;

			OutVertices.Add(FinalCirclePosition);
			OutVertexColors.Add(FColor::Cyan);

			CircleCurrentAngle += CircleStepAngle;
		}
	}

	// Indices
	for (int CurrentRadiusStep = 0; CurrentRadiusStep < VUEGlobals::TorusNumberOfEdges - 1; CurrentRadiusStep++)
	{
		int Current;
		for (int CurrentCircleStep = 0; CurrentCircleStep < VUEGlobals::TorusNumberOfCircleEdges - 1; CurrentCircleStep++)
		{
			Current = IndexOfVertices + CurrentRadiusStep * VUEGlobals::TorusNumberOfCircleEdges + CurrentCircleStep;

			OutIndices.Add(Current);
			OutIndices.Add(Current + VUEGlobals::TorusNumberOfCircleEdges);
			OutIndices.Add(Current + 1);

			OutIndices.Add(Current + 1);
			OutIndices.Add(Current + VUEGlobals::TorusNumberOfCircleEdges);
			OutIndices.Add(Current + VUEGlobals::TorusNumberOfCircleEdges + 1);
		}

		// Reuse first vertices
		Current = IndexOfVertices + CurrentRadiusStep * VUEGlobals::TorusNumberOfCircleEdges;
		OutIndices.Add(Current + VUEGlobals::TorusNumberOfCircleEdges - 1);
		OutIndices.Add(Current + VUEGlobals::TorusNumberOfCircleEdges + VUEGlobals::TorusNumberOfCircleEdges - 1);
		OutIndices.Add(Current);

		OutIndices.Add(Current);
		OutIndices.Add(Current + VUEGlobals::TorusNumberOfCircleEdges + VUEGlobals::TorusNumberOfCircleEdges - 1);
		OutIndices.Add(Current + VUEGlobals::TorusNumberOfCircleEdges);
	}

	// Connect last and first
	int First, Last;
	for (int CurrentCircleStep = 0; CurrentCircleStep < VUEGlobals::TorusNumberOfCircleEdges - 1; CurrentCircleStep++)
	{
		First = IndexOfVertices + CurrentCircleStep;
		Last = IndexOfVertices + (VUEGlobals::TorusNumberOfEdges - 1) * VUEGlobals::TorusNumberOfCircleEdges + CurrentCircleStep;

		OutIndices.Add(First);
		OutIndices.Add(First + 1);
		OutIndices.Add(Last);

		OutIndices.Add(Last);
		OutIndices.Add(First + 1);
		OutIndices.Add(Last + 1);
	}

	First = IndexOfVertices + VUEGlobals::TorusNumberOfCircleEdges;
	Last = OutVertices.Num();

	OutIndices.Add(First - 1);
	OutIndices.Add(Last - VUEGlobals::TorusNumberOfCircleEdges);
	OutIndices.Add(Last - 1);

	OutIndices.Add(First - VUEGlobals::TorusNumberOfCircleEdges);
	OutIndices.Add(Last - VUEGlobals::TorusNumberOfCircleEdges);
	OutIndices.Add(First - 1);

	IndexOfVertices += VUEGlobals::TorusNumberOfEdges * VUEGlobals::TorusNumberOfCircleEdges;
}

/*
 *  VUE specific primitives
 */
bool VUETriangulation::Projection3D(VUEParserW::Primitive::Projection3D* Projection, TArray<FVector>& OutVertices, TArray<int32>& OutIndices)
{
	// Only Ellipse3D is supported
	if (Projection->SubType->Type == VUEParserW::ContainerType::Ellipse3D)
	{
		VUEParserW::Primitive::Ellipse3D* Ellipse3D = static_cast<VUEParserW::Primitive::Ellipse3D*>(Projection->SubType);

		FVector ExtrusionDepth = VUEHelper::VUEVector3DToFVector(Projection->ExtrusionDepth) * VUEGlobals::ScaleFactor;
		FVector Orientation    = VUEHelper::VUEVector3DToFVector(Ellipse3D->Orientation);
		FVector Radius         = VUEHelper::VUEVector3DToFVector(Ellipse3D->Radius) * VUEGlobals::ScaleFactor;
		FVector Position       = VUEHelper::VUEVector3DToFVector(Ellipse3D->Position) * VUEGlobals::ScaleFactor;
		
		float Step = 360 / VUEGlobals::ProjectionNumberOfEdges;
		float Angle = 0;
		for (int Index = 0; Index < VUEGlobals::ProjectionNumberOfEdges; Index++)
		{
			Angle += Step;
			FVector RotatedRadius = Radius.RotateAngleAxis(Angle, Orientation);

			FVector Position1;
			Position1.X = Position.X + RotatedRadius.X;
			Position1.Y = Position.Y + RotatedRadius.Y;
			Position1.Z = Position.Z + RotatedRadius.Z;

			FVector Position2 = Position1 + ExtrusionDepth;

			OutVertices.Add(Position1);
			OutVertices.Add(Position2);
		}

		int VerticesCount = VUEGlobals::ProjectionNumberOfEdges * 2;

		for (int Index = 0; Index < VerticesCount - 2; Index +=2)
		{
			OutIndices.Add(IndexOfVertices + Index);
			OutIndices.Add(IndexOfVertices + Index + 2);
			OutIndices.Add(IndexOfVertices + Index + 1);

			OutIndices.Add(IndexOfVertices + Index + 1);
			OutIndices.Add(IndexOfVertices + Index + 2);
			OutIndices.Add(IndexOfVertices + Index + 3);
		}

		// Reuse first vertices
		OutIndices.Add(IndexOfVertices + VerticesCount - 2);
		OutIndices.Add(IndexOfVertices);
		OutIndices.Add(IndexOfVertices + VerticesCount - 1);

		OutIndices.Add(IndexOfVertices + VerticesCount - 1);
		OutIndices.Add(IndexOfVertices);
		OutIndices.Add(IndexOfVertices + 1);

		// Draw top and bottom circles
		// Use the first two points as base points. Like triangle_fan from OpenGL
		for (int32 Index = 1; Index < VerticesCount - 2; Index++)
		{
			if (Index % 2)
			{
				OutIndices.Add(IndexOfVertices + 1);
				OutIndices.Add(IndexOfVertices + Index);
				OutIndices.Add(IndexOfVertices + Index + 2);
			}
			else
			{
				OutIndices.Add(IndexOfVertices);
				OutIndices.Add(IndexOfVertices + Index + 2);
				OutIndices.Add(IndexOfVertices + Index);
			}
		}

		IndexOfVertices += VerticesCount;

		return true;
	}

	return false;
}

bool VUETriangulation::Revolution3D(VUEParserW::Primitive::Revolution3D* Revolution, TArray<FVector>& OutVertices, TArray<int32>& OutIndices)
{
	// Only Ellipse3D is supported
	if (Revolution->SubType->Type == VUEParserW::ContainerType::Ellipse3D)
	{
		VUEParserW::Primitive::Ellipse3D* Ellipse3D = static_cast<VUEParserW::Primitive::Ellipse3D*>(Revolution->SubType);

		double RevolutionAngle        = Revolution->Angle;
		double EllipseRatio           = Ellipse3D->Ratio;

		FVector RevolutionPosition    = VUEHelper::VUEVector3DToFVector(Revolution->Position) * VUEGlobals::ScaleFactor; // Centerpoint
		FVector RevolutionOrientation = VUEHelper::VUEVector3DToFVector(Revolution->Orientation);
		FVector EllipsePosition       = VUEHelper::VUEVector3DToFVector(Ellipse3D->Position) * VUEGlobals::ScaleFactor;
		FVector EllipseRadius         = VUEHelper::VUEVector3DToFVector(Ellipse3D->Radius) * VUEGlobals::ScaleFactor;
		FVector EllipseOrientation    = VUEHelper::VUEVector3DToFVector(Ellipse3D->Orientation);
		
		FVector DrawLocation = EllipsePosition - RevolutionPosition;

		RevolutionAngle = FMath::RadiansToDegrees(RevolutionAngle);

		double RadiusStepAngle = RevolutionAngle / (VUEGlobals::RevolutionNumberOfEdges - 1);
		double CircleStepAngle = 360 / VUEGlobals::RevolutionNumberOfCircleEdges;

		double RadiusCurrentAngle = 0;
		for (int Step = 0; Step < VUEGlobals::RevolutionNumberOfEdges; Step++)
		{
			FVector FinalPosition = DrawLocation.RotateAngleAxis(RadiusCurrentAngle, RevolutionOrientation);
			FinalPosition += RevolutionPosition;

			FVector DrawOrientation = EllipseOrientation.RotateAngleAxis(RadiusCurrentAngle, RevolutionOrientation);
			FVector DrawRadius = EllipseRadius.RotateAngleAxis(RadiusCurrentAngle, RevolutionOrientation);

			RadiusCurrentAngle += RadiusStepAngle;

			float CircleCurrentAngle = 0;
			for (int CircleStep = 0; CircleStep < VUEGlobals::RevolutionNumberOfCircleEdges; CircleStep++)
			{
				FVector CircleRotation = DrawRadius.RotateAngleAxis(CircleCurrentAngle, DrawOrientation);

				CircleCurrentAngle += CircleStepAngle;

				OutVertices.Add(FinalPosition + CircleRotation);
			}
		}

		for (int Step = 0; Step < VUEGlobals::RevolutionNumberOfEdges - 1; Step++)
		{
			int Current;
			for (int CircleStep = 0; CircleStep < VUEGlobals::RevolutionNumberOfCircleEdges - 1; CircleStep++)
			{
				Current = IndexOfVertices + Step * VUEGlobals::RevolutionNumberOfCircleEdges + CircleStep;
				
				OutIndices.Add(Current);
				OutIndices.Add(Current + VUEGlobals::RevolutionNumberOfCircleEdges);
				OutIndices.Add(Current + 1);

				OutIndices.Add(Current + 1);
				OutIndices.Add(Current + VUEGlobals::RevolutionNumberOfCircleEdges);
				OutIndices.Add(Current + VUEGlobals::RevolutionNumberOfCircleEdges + 1);
			}

			// Reuse first vertices
			Current = IndexOfVertices + Step * VUEGlobals::RevolutionNumberOfCircleEdges;
			OutIndices.Add(Current + VUEGlobals::RevolutionNumberOfCircleEdges - 1);
			OutIndices.Add(Current + VUEGlobals::RevolutionNumberOfCircleEdges + VUEGlobals::RevolutionNumberOfCircleEdges - 1);
			OutIndices.Add(Current);

			OutIndices.Add(Current);
			OutIndices.Add(Current + VUEGlobals::RevolutionNumberOfCircleEdges + VUEGlobals::RevolutionNumberOfCircleEdges - 1);
			OutIndices.Add(Current + VUEGlobals::RevolutionNumberOfCircleEdges);
		}

		IndexOfVertices += VUEGlobals::RevolutionNumberOfEdges * VUEGlobals::RevolutionNumberOfCircleEdges;

		return true;
	}

	return false;
}

bool VUETriangulation::Torus3D(VUEParserW::Primitive::Torus3D* Torus, TArray<FVector>& OutVertices, TArray<int32>& OutIndices, TArray<FColor>& OutVertexColors)
{
	if (Torus->PrimitiveContainersCount == 0)
	{
		VUETriangulation::Torus(OutVertices, OutIndices, OutVertexColors, VUEHelper::VUEVector3DToFVector(Torus->Position) * VUEGlobals::ScaleFactor, VUEHelper::VUEVector3DToFVector(Torus->Orientation1), VUEHelper::VUEVector3DToFVector(Torus->Orientation2), Torus->Radius * VUEGlobals::ScaleFactor, Torus->Thickness * VUEGlobals::ScaleFactor);

		return true;
	}

	return false;
}