using UnrealBuildTool;
using System.Collections.Generic;

public class EverwildOdysseyTarget : TargetRules
{
    public EverwildOdysseyTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V6;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.Add("EverwildOdyssey");
    }
}
