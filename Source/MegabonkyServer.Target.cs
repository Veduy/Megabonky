// Copyright is owned by Veduy.

using UnrealBuildTool;
using System.Collections.Generic;

public class MegabonkyServerTarget : TargetRules
{
	public MegabonkyServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.AddRange( new string[] { "Megabonky" } );
	}
}
