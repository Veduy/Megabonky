// Copyright is owned by Veduy.

using UnrealBuildTool;
using System.Collections.Generic;

public class MegabonkyTarget : TargetRules
{
	public MegabonkyTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;

        ExtraModuleNames.AddRange( new string[] { "Megabonky" } );
	}
}
