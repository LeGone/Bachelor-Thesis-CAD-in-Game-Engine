#pragma once
#include "Public/Containers/UnrealString.h"

class VUEGlobals
{
public:

	enum ECreateGeometryDepthLayer
	{
		Container,
		Group,
		Primitive
	};

	static bool                      bCalculateNormalsAndTangents;
	static bool                      bDrawBoundingBoxOnError;
	static bool                      bUseTransparency;
	static ECreateGeometryDepthLayer CreateGeometryDepthLayer;
	static float                     ScaleFactor;
	static bool                      bCollision;
	static int                       SphereNumberOfEdges;
	static int                       TorusNumberOfEdges;
	static int                       TorusNumberOfCircleEdges;
	static int                       ProjectionNumberOfEdges;
	static int                       RevolutionNumberOfEdges;
	static int                       RevolutionNumberOfCircleEdges;
	static FString                   LastImportedFile;
};