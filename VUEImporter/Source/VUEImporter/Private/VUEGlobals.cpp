#include "VUEGlobals.h"

VUEGlobals::ECreateGeometryDepthLayer VUEGlobals::CreateGeometryDepthLayer = VUEGlobals::ECreateGeometryDepthLayer::Primitive;
bool  VUEGlobals::bDrawBoundingBoxOnError                                  = true;
bool  VUEGlobals::bUseTransparency                                         = true;
bool  VUEGlobals::bCalculateNormalsAndTangents                             = true;
float VUEGlobals::ScaleFactor                                              = 100.0f;
bool  VUEGlobals::bCollision                                               = true;
int   VUEGlobals::SphereNumberOfEdges                                      = 10;
int   VUEGlobals::TorusNumberOfEdges                                       = 10;
int   VUEGlobals::TorusNumberOfCircleEdges                                 = 10;
int   VUEGlobals::ProjectionNumberOfEdges                                  = 10;
int   VUEGlobals::RevolutionNumberOfEdges                                  = 10;
int   VUEGlobals::RevolutionNumberOfCircleEdges                            = 10;
FString VUEGlobals::LastImportedFile                                       = "";