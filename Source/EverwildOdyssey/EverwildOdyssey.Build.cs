using UnrealBuildTool;

public class EverwildOdyssey : ModuleRules
{
    public EverwildOdyssey(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "UMG",
            "CommonUI",
            "AIModule",
            "GameplayTasks",
            "NavigationSystem"
        });
    }
}
