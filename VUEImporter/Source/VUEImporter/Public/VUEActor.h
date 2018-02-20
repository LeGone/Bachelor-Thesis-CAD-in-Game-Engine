#pragma once

#include "RuntimeMeshComponent.h"
#include "VUEImporterPlugin.h"
#include "VUEActor.generated.h"

UENUM()
enum class EDefinitionSuperClasses : uint8
{
	Unknown,
	Cableway,
	Cabling,
	Common,
	Conduit,
	Equipment,
	HVAC,
	Piping,
	Structure,
	Supports,
	Systems
};

UENUM()
enum class EDefinitionType
{
	Unknown,
	P3DCableway = 80003,
	P3DCableTrayComponent = 80056,
	P3DCableTray = 80053,
	P3DCablewayTurnTransitionPathFeature = 80023,
	P3DCablewayAlongLegPathFeature = 80017,
	P3DCablewayBranchPathFeature = 80018,
	P3DCablewayEndPathFeature = 80019,
	P3DCablewaySlantTransitionPathFeature = 80052,
	P3DCablewayStraightPathFeature = 80020,
	P3DCablewayTransitionAlongLegPathFeature = 80021,
	P3DCablewayTurnPathFeature = 80022,
	P3DCable = 80001,
	P3DCableEndPathFeature = 80015,
	P3DCableEnterExitPathFeature = 80062,
	P3DCableRun = 80002,
	P3DCableStraightPathFeature = 80016,
	P3DCableSegmentPathFeature = 80063,
	P3DElectricalPort = 60029,
	P3DCableMarker = 80066,
	P3DDistributionConnection = 80007,
	P3DControlPoint = 10057,
	P3DZone = 70006,
	P3DInterferenceVolume = 70007,
	P3DArea = 70008,
	P3DConduitAlongLegPathFeature = 80049,
	P3DConduitBranchPathFeature = 80050,
	P3DConduitEndPathFeature = 80048,
	P3DConduitStraightPathFeature = 80047,
	P3DConduitTurnPathFeature = 80051,
	P3DConduitComponent = 80046,
	P3DConduit = 80044,
	P3DConduitPort = 80057,
	P3DConduitRun = 80045,
	P3DEquipmentFoundationPort = 20006,
	P3DEquipment = 20002,
	P3DDesignEquipment = 20009,
	P3DSmartEquipment = 20014,
	P3DEquipmentComponent = 20016,
	P3DCableTrayNozzle = 20012,
	P3DCableTrayPort = 80058,
	P3DCableNozzle = 20005,
	P3DConduitNozzle = 20011,
	P3DHVACNozzle = 20004,
	P3DPipeNozzle = 20003,
	P3DDuctAlongLegPathFeature = 80024,
	P3DDuctBranchPathFeature = 80025,
	P3DDuctEndPathFeature = 80026,
	P3DDuct = 80009,
	P3DDuctRun = 80010,
	P3DDuctSlantTransPathFeature = 80027,
	P3DDuctSplitPathFeature = 80028,
	P3DDuctStraightPathFeature = 80029,
	P3DDuctSurfaceMountPathFeature = 80039,
	P3DDuctTransAlongLegPathFeature = 80030,
	P3DDuctTurnPathFeature = 80031,
	P3DDuctTurnTransPathFeature = 80032,
	P3DHVACPort = 60051,
	P3DDuctComponent = 80043,
	P3DDuctSpool = 80071,
	P3DPipeRun = 80013,
	P3DPipingComponent = 80005,
	P3DPipeInstrument = 80054,
	P3DPipeSpecialty = 80055,
	P3DPipeTap = 80060,
	P3DWeld = 80042,
	P3DWeld2 = 1010021,
	P3DPipe = 80012,
	P3DPipeGasket = 80040,
	P3DPipeTurnPathFeature = 80038,
	P3DPipeAlongLegPathFeature = 80034,
	P3DPipeBranchPathFeature = 80035,
	P3DPipeEndPathFeature = 80036,
	P3DPipeStraightPathFeature = 80037,
	P3DPipeTapFeature = 80059,
	P3DSpool = 300002,
	P3DPenetrationSpool = 300005,
	P3DPipePort = 60017,
	P3DPipeBoltSet = 80041,
	P3DValveOperator = 60083,
	P3DPipeClamp = 80067,
	P3DFooting = 240121,
	P3DFrameConnection = 240020,
	P3DHandrail = 240141,
	P3DLadder = 240081,
	P3DSlab = 240024,
	P3DStair = 240091,
	P3DMemberSystemLinear = 240011,
	P3DStructuralPort = 280125,
	P3DMemberPartPrismatic = 240012,
	P3DEquipmentFoundation = 240066,
	P3DStructInsulation = 240160,
	P3DStiffenerPart = 1090062,
	P3DPlatePart = 1090063,
	P3DProfilePart = 1090065,
	P3DBeamPart = 1090066,
	CCollarPart = 1100002,
	CStandAlonePlatePart = 220003,
	CStandAlonePlatePart2 = 280001,
	CStandAloneStiffenerPart = 220005,
	CImpPlate = 1500000,
	CImpStiffner = 1500001,
	CEdgeReinforcementProfilePart = 220009,
	SPSWallPart = 240053,
	CSPSMemberPartCurve = 240029,
	CSPSFootingComponent = 240122,
	CStructCustomPlatePart = 280090,
	P3DHangerDuctSupport = 140037,
	P3DHangerCableTraySupport = 140036,
	P3DHangerCombinedSupport = 140043,
	P3DHangerConduitSupport = 140042,
	P3DHangerConnComponent = 140038,
	P3DHangerDesignSupport = 140049,
	P3DHangerLogicalComponent = 140040,
	P3DHangerPipeSupport = 140035,
	P3DHangerStdComponent = 140039,
	P3DHangerSupport = 140030,
	P3DHangerSupportComponent = 140032,
	P3DPipelineSystem = 210007,
	P3DConduitSystem = 210009,
	P3DDuctingSystem = 210001,
	P3DElectricalSystem = 210002,
	P3DMachinerySystem = 210004,
	P3DPipingSystem = 210005,
	P3DStructuralSystem = 210006,
	// Just some space ... because I can :D
	CSPGCoordinateSystem = 290001,
	CStructAssemblyConnection = 28051,
	CMNSTRAssemblyConnection = 28056,
	StructPlatePart = 280123,
	Shape = 20007,
	CPPrismaticShape = 20013,
	CSPGAxisForRuler = 290002,
	CSPGElevationPlane = 290003,
	CSPGGridPlane = 290004,
	CSPGGridLine = 290005,
	CSPGShipCoordinateSystem = 290007,
	CSPGRadialPlane = 290008,
	CSPGRadialCylinder = 290009,
	CSPGGridArc = 290010,
	CHgrCombinedSupport = 140041,
	CPMLogicalDistributionConn = 80008,
	CPLogicalCable = 80068,
	CPRteCableTrayDistribConn = 80069,
	CPPipeSurfaceMountFeat = 80070,
	CSPSFrameConnection = 240017,
	CSPSMemberSystemCurve = 240027,
	SPSWallRun = 240050,
	SPSWallSystem = 240052,
	CSPSFoundationComponent = 240067
};

UENUM()
enum class EContainerType
{
		Unknown,
		Line3D = 0x03,
		LineString3D = 0x04,
		ComplexString3D = 0xC,
		Plane3D = 0xE,
		Ellipse3D = 0xF,
		EllipticalArc3D = 0x10,
		Cone3D = 0x17,
		BspSurface3D = 0x18,
		Boundaries3D = 0x19,
		BspCurve3D = 0x1B,
		Sphere3D = 0x1D,
		Torus3D = 0x1E,
		Projection3D = 0x1F,
		Revolution3D = 0x20,
		Ruled3D = 0x21,
		Point3D = 0x28,
		PolyMesh3D = 0x32,

		// Not official
		Cylindrical3D = 0xD2,
		NoGroup = 0xE0,
		BooleanObject = 0xE6,
		Generic = 0xE7
};

UENUM()
enum class EDisplayAspectType
{
	None UMETA(DisplayName = "None"),
	SimplePhysical = 1 UMETA(DisplayName = "Simple Physical"),
	DetailedPhysical = 16 UMETA(DisplayName = "Detailed Physical"),
	Insulation = 32 UMETA(DisplayName = "Insulation"),
	Operation = 64 UMETA(DisplayName = "Operation"),
	Maintenance = 128 UMETA(DisplayName = "Maintenance"),
	ReferenceGeometry = 256 UMETA(DisplayName = "Reference Geometry"),
	CenterLine = 512 UMETA(DisplayName = "Center Line")
};

UCLASS(config = Editor)
class AVUEActor : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, Category = VUE)
	AVUEHeader* VUEHeader;

	UPROPERTY(VisibleAnywhere, Category = VUE)
	FString File;

	UPROPERTY(VisibleAnywhere, Category = VUE)
	EDefinitionSuperClasses SuperClass;

	UPROPERTY(VisibleAnywhere, Category = VUE)
	EDefinitionType DefinitionType;

	UPROPERTY(VisibleAnywhere, Category = VUE)
	EContainerType ContainerType;

	UPROPERTY(VisibleAnywhere, Category = VUE)
	TArray<EDisplayAspectType> ContainedDisplayAspect;

	AVUEActor();

	//void Draw();
	void AddPrimitive(const TArray<FVector>& Vertices, const TArray<int32>& Triangles, const TArray<FColor>& VertexColors, UMaterialInstanceDynamic* Material, const bool bEnableCollision = false, const bool bCalculateNormalsAndTangents = false);

protected:

private:

	int SectionIndex;
	UMaterial* RawMaterial;

	UPROPERTY(VisibleAnywhere)
	URuntimeMeshComponent* Mesh;
};