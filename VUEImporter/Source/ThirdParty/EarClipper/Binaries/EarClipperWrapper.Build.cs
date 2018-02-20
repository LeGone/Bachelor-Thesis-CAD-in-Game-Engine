// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class EarClipperWrapper : ModuleRules
{
    public EarClipperWrapper(ReadOnlyTargetRules Target) : base(Target)
    {
		Type = ModuleType.External;

		// Add the import library
		PublicLibraryPaths.Add(ModuleDirectory);
		PublicAdditionalLibraries.Add("EarClipperWrapper.lib");

        // Delay-load the DLL, so we can load it from the right place first
        PublicDelayLoadDLLs.Add("EarClipperWrapper.dll");
    }
}
