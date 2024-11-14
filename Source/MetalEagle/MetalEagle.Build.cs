// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class MetalEagle : ModuleRules
{
	public MetalEagle(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" ,"HeadMountedDisplay","MediaAssets","UMG"});


        PrivateDependencyModuleNames.AddRange(new string[] {  });

        MinFilesUsingPrecompiledHeaderOverride = 1;
        bFasterWithoutUnity = true;
    }
}
