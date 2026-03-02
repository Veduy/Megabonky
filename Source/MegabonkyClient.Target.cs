// Copyright is owned by Veduy.

using UnrealBuildTool;
using System.Collections.Generic;

public class MegabonkyClientTarget : TargetRules
{
	public MegabonkyClientTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Client;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.AddRange( new string[] { "Megabonky" } );
	}
}
