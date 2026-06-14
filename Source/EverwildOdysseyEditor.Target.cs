using UnrealBuildTool;
using System.Collections.Generic;

public class EverwildOdysseyEditorTarget : TargetRules
{
    public EverwildOdysseyEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V6;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.Add("EverwildOdyssey");
    }
}
