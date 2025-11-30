// Copyright is owned by Veduy.

using UnrealBuildTool;
using System.Collections.Generic;

public class MegabonkyEditorTarget : TargetRules
{
	public MegabonkyEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;

        ExtraModuleNames.AddRange( new string[] { "Megabonky" } );
	}
}
