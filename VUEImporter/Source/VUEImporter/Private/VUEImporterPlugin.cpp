// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "VUEImporterPlugin.h"
#include "VUEImporterStyle.h"
#include "VUEImporterCommands.h"
#include "VUEImporter.h"
#include "VUEGlobals.h"
#include "VUEHelper.h"

#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Input/STextComboBox.h"
#include "Widgets/Layout/SSplitter.h"
#include "IPluginManager.h"
#include "DrawDebugHelpers.h"

static const FName VUEImporterTabName("VUEImporter");

#define LOCTEXT_NAMESPACE "FVUEImporterModule"

void FVUEImporterModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FVUEImporterStyle::Initialize();
	FVUEImporterStyle::ReloadTextures();

	FVUEImporterCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FVUEImporterCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FVUEImporterModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FVUEImporterModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FVUEImporterModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(VUEImporterTabName, FOnSpawnTab::CreateRaw(this, &FVUEImporterModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FVUEImporterTabTitle", "VUEImporter"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	FString BaseDir = IPluginManager::Get().FindPlugin("VUEImporter")->GetBaseDir();

	FString LibraryPath = FPaths::Combine(*BaseDir, TEXT("/Source/ThirdParty/VUEParser/Binaries/VUEParserCPPWrapper.dll"));
	LoadLibrary(*LibraryPath);

	LibraryPath = FPaths::Combine(*BaseDir, TEXT("/Source/ThirdParty/EarClipper/Binaries/EarClipperWrapper.dll"));
	LoadLibrary(*LibraryPath);
}

void FVUEImporterModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FVUEImporterStyle::Shutdown();

	FVUEImporterCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(VUEImporterTabName);
}

TSharedRef<SDockTab> FVUEImporterModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	static TArray<TSharedPtr<FString>> AttributesList;
	TSharedPtr<FString> SharedPrimitiveEntry = MakeShareable(new FString("Primitive"));
	AttributesList.Add(MakeShareable(new FString("Container")));
	AttributesList.Add(MakeShareable(new FString("Group")));
	AttributesList.Add(SharedPrimitiveEntry);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SScrollBox)
			+SScrollBox::Slot().Padding(10, 5)
			[
				SNew(SVerticalBox)

				+SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SButton)
					.Text(FText::FromString("Select VUE File"))
					.OnClicked_Raw(this, &FVUEImporterModule::OnClickedImportButton)
				]
			]
			+SScrollBox::Slot().Padding(10, 5)
			[
				SNew(SVerticalBox)

				+SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SButton)
					.Text(FText::FromString("Flush Lines And Points"))
					.OnClicked_Raw(this, &FVUEImporterModule::OnClickedFlushLinesAndPointsButton)
				]
			]
			+SScrollBox::Slot().Padding(10, 5)
			[
				SNew(SVerticalBox)

				+SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0, 0, 0, 2)
				[
					SNew(STextBlock)
					.Text(FText::FromString("Scale Factor"))
				]

				+SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SSpinBox<float>)
					.MinValue(1.0f)
					.MaxValue(1000.0f)
					.Value(VUEGlobals::ScaleFactor)
					.OnValueChanged_Lambda([](float NewValue)
					{
						VUEGlobals::ScaleFactor = NewValue;
					})
				]
			]
			+SScrollBox::Slot().Padding(10, 5)
			[
				SNew(SVerticalBox)

				+SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0, 0, 0, 2)
				[
					SNew(STextBlock)
					.Text(FText::FromString("Generate mesh section per"))
				]

				+SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(STextComboBox)
					.OptionsSource(&AttributesList)
					.OnSelectionChanged_Lambda([](TSharedPtr<FString> Name, ESelectInfo::Type SelectionInfo)
					{
						if (Name.IsValid())
						{
							// There is no index
							if (Name->Equals(FString("Container")))
							{
								VUEGlobals::CreateGeometryDepthLayer = VUEGlobals::ECreateGeometryDepthLayer::Container;
							}
							else if (Name->Equals(FString("Group")))
							{
								VUEGlobals::CreateGeometryDepthLayer = VUEGlobals::ECreateGeometryDepthLayer::Group;
							}
							else
							{
								VUEGlobals::CreateGeometryDepthLayer = VUEGlobals::ECreateGeometryDepthLayer::Primitive;
							}
						}
					})
					.InitiallySelectedItem(SharedPrimitiveEntry)
				]
			]
			+SScrollBox::Slot().Padding(10, 5)
			[
				SNew(SCheckBox)

				.OnCheckStateChanged_Lambda([](ECheckBoxState NewState)
				{
					if (NewState == ECheckBoxState::Checked)
					{
						VUEGlobals::bCalculateNormalsAndTangents = true;
					}
					else
					{
						VUEGlobals::bCalculateNormalsAndTangents = false;
					}
				})
				.IsChecked(VUEGlobals::bCalculateNormalsAndTangents ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
				.Content()
				[
					SNew(STextBlock)
					.Text(FText::FromString("Calculate normals and tangents"))
				]
			]
			+SScrollBox::Slot().Padding(10, 5)
			[
				SNew(SCheckBox)

				.OnCheckStateChanged_Lambda([](ECheckBoxState NewState)
				{
					if (NewState == ECheckBoxState::Checked)
					{
						VUEGlobals::bCollision = true;
					}
					else
					{
						VUEGlobals::bCollision = false;
					}
				})
				.IsChecked(VUEGlobals::bCollision ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
				.Content()
				[
					SNew(STextBlock)
					.Text(FText::FromString("Enable collision"))
				]
			]
			+SScrollBox::Slot().Padding(10, 5)
			[
				SNew(SCheckBox)

				.OnCheckStateChanged_Lambda([](ECheckBoxState NewState)
				{
					if (NewState == ECheckBoxState::Checked)
					{
						VUEGlobals::bDrawBoundingBoxOnError = true;
					}
					else
					{
						VUEGlobals::bDrawBoundingBoxOnError = false;
					}
				})
				.IsChecked(VUEGlobals::bDrawBoundingBoxOnError ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
				.Content()
				[
					SNew(STextBlock)
					.Text(FText::FromString("Draw Boundingbox on error"))
				]
			]
			+SScrollBox::Slot().Padding(10, 5)
			[
				SNew(SCheckBox)

				.OnCheckStateChanged_Lambda([](ECheckBoxState NewState)
				{
					if (NewState == ECheckBoxState::Checked)
					{
						VUEGlobals::bUseTransparency = true;
					}
					else
					{
						VUEGlobals::bUseTransparency = false;
					}
				})
				.IsChecked(VUEGlobals::bUseTransparency ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
				.Content()
				[
					SNew(STextBlock)
					.Text(FText::FromString("Use transparency materials"))
				]
			]
		];
}

void FVUEImporterModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(VUEImporterTabName);
}

void FVUEImporterModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FVUEImporterCommands::Get().OpenPluginWindow);
}

void FVUEImporterModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FVUEImporterCommands::Get().OpenPluginWindow);
}

FReply FVUEImporterModule::OnClickedFlushLinesAndPointsButton()
{
	VUEHelper::FlushLineBatcher();

	return FReply::Handled();
}

FReply FVUEImporterModule::OnClickedImportButton()
{
	UE_LOG(LogTemp, Log, TEXT("Importing VUE File"));

	TArray<FString> Files;
	if (VUEHelper::OpenVUEFileDialog(Files))
	{
		VUEGlobals::LastImportedFile = *Files[0];
		VUEImporter* VUEImporterInstance = new VUEImporter();
		VUEImporterInstance->ImportVUEFile(*VUEGlobals::LastImportedFile);
		delete VUEImporterInstance;

		UE_LOG(LogTemp, Log, TEXT("Finished importing VUE File"));
	}

	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FVUEImporterModule, VUEImporter)