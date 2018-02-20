#pragma once

#include "VUEImporterPlugin.h"

class VUETriangulation
{
private:

	/*
	* IndexOfVertices is used whenever you want to store multiple shapes in one section.
	* It's static and every function in this file increases it.
	* Call ResetIndexOfVertices from outside if you need to reset it.
	*/
	static int IndexOfVertices;

public:

	static void ResetIndexOfVertices();

	/*
	* Triangulates a polygon using EarClipper library.
	*
	* @pre InVertices must have at least 3 vertices
	* @param InVertices - Contains all the vertices that the library needs to triangulate
	* @param OutIndices - Upon return, will contain a list of indices
	* @post Always check if the number of indices is greater than 0 (OutIndices.Num > 0). If not, the library was unable to triangulate.
	* @modify IndexOfVertices - Increased by the amount of newly created vertices
	*/
	static void TriangulatePolygon(TArray<FVector>& InVertices, TArray<int32>& InTriangles, FVector Normal);

	/*
	* Generates a 3d box out of a box shape. Commonly used for boundingboxes.
	*
	* @pre Uses VUEGlobals::ScaleFactor to scale every vertices. Make sure, that it is greater than 0.
	* @param OutVertices      - Upon return, will contain a list of vertices
	* @param OutIndices       - Upon return, will contain a list of indices
	* @param OutVertexColors  - Upon return, will contain a list of white colors
	* @modify IndexOfVertices - Increased by the amount of newly created vertices
	*/
	static void Box(TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FColor>& VertexColors, FBox BoxShape);

	/*
	* Generates a sphere using position and radius.
	*
	* @pre Uses VUEGlobals::ScaleFactor to scale every vertices. Make sure, that it is greater than 0.
	*      Use VUEGlobals::SphereEdgeCount to increase or decrease detail.
	* @param OutVertices      - Upon return, will contain a list of vertices
	* @param OutIndices       - Upon return, will contain a list of indices
	* @param OutVertexColors  - Upon return, will contain a list of white colors
	* @param Position         - The location you want to place the sphere
	* @param Radius           - The radius of the sphere
	* @modify IndexOfVertices - Increased by the amount of newly created vertices
	* @see Based on DrawSphere from Engine\Source\Runtime\Engine\Private\PrimitiveDrawingUtils.cpp
	*/
	static void Sphere(TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FColor>& VertexColors, FVector Position, float Radius);

	/*
	* Generates a torus.
	*
	* @pre Uses VUEGlobals::ScaleFactor to scale every vertices. Make sure, that it is greater than 0.
	*      Use VUEGlobals::TorusNumberOfEdges and VUEGlobals::TorusNumberOfCircleEdges to increase or decrease detail.
	* @param OutVertices      - Upon return, will contain a list of vertices
	* @param OutIndices       - Upon return, will contain a list of indices
	* @param OutVertexColors  - Upon return, will contain a list of white colors
	* @param Position         - The location you want to place the sphere
	* @param Orientation1     - The up orientation. The torus will be rotated on this axis by a total of 360°.
	* @param Orientation2     - The orientation
	* @param Radius           - The radius of the torus
	* @param Thickness        - The Thickness of the torus
	* @modify IndexOfVertices - Increased by the amount of newly created vertices
	*/
	static void Torus(TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FColor>& VertexColors, FVector Position, FVector Orientation1, FVector Orientation2, int Radius, int Thickness);


	/*
	*  VUE specific primitives
	*/

	/*
	* Projection3D. Uses an extrusion depth to project a 3d shape out of a 2d shape.
	*
	* @pre Uses VUEGlobals::ScaleFactor to scale every vertices. Make sure, that it is greater than 0.
	*      Use VUEGlobals::TorusNumberOfEdges and VUEGlobals::TorusNumberOfCircleEdges to increase or decrease detail.
	*      Only Ellipse3D is supported!
	* @param Projection  - The VUE primitive
	* @param OutVertices - Upon return, will contain a list of vertices
	* @param OutIndices  - Upon return, will contain a list of indices
	* @post If subtype Ellipse3D: Also generates top and bottom circles to create a closed mesh.
	* @modify IndexOfVertices - Increased by the amount of newly created vertices
	* @return true if succeeded, false otherwise
	* @see VUEParserW::Primitive::Projection3D
	*/
	static bool Projection3D(VUEParserW::Primitive::Projection3D* Projection, TArray<FVector>& Vertices, TArray<int32>& Triangles);

	/*
	* Revolution3D. Creates a 3D shape out of 2D shape by bending it around a radius.
	*
	* @pre Uses VUEGlobals::ScaleFactor to scale every vertices. Make sure, that it is greater than 0.
	*      Use VUEGlobals::TorusNumberOfEdges and VUEGlobals::TorusNumberOfCircleEdges to increase or decrease detail.
	*      Only Ellipse3D is supported!
	* @param Revolution       - The VUE primitive
	* @param OutVertices      - Upon return, will contain a list of vertices
	* @param OutIndices       - Upon return, will contain a list of indices
	* @modify IndexOfVertices - Increased by the amount of newly created vertices
	* @return true if succeeded, false otherwise
	* @see VUEParserW::Primitive::Revolution3D
	*/
	static bool Revolution3D(VUEParserW::Primitive::Revolution3D* Revolution, TArray<FVector>& Vertices, TArray<int32>& Triangles);

	/*
	* Torus3D creates a torus by using the VUE primitive named Torus3D.
	*
	* @pre Uses VUEGlobals::ScaleFactor to scale every vertices. Make sure, that it is greater than 0.
	*      Use VUEGlobals::TorusNumberOfEdges and VUEGlobals::TorusNumberOfCircleEdges to increase or decrease detail.
	*      Only works on simple torus without any childs (Torus->PrimitiveContainersCount is zero).
	* @param Torus            - The VUE primitive
	* @param OutVertices      - Upon return, will contain a list of vertices
	* @param OutIndices       - Upon return, will contain a list of indices
	* @param OutVertexColors  - Upon return, will contain a list of white colors
	* @modify IndexOfVertices - Increased by the amount of newly created vertices
	* @return true if succeeded, false otherwise
	* @see VUEParserW::Primitive::Torus3D
	*/
	static bool Torus3D(VUEParserW::Primitive::Torus3D* Torus, TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FColor>& VertexColors);
};