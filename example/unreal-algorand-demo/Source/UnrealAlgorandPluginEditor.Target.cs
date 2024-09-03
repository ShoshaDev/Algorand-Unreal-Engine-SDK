// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class UnrealAlgorandPluginEditorTarget : TargetRules
{
	public UnrealAlgorandPluginEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		ExtraModuleNames.AddRange( new string[] { "UnrealAlgorandPlugin" } );
	}
}
