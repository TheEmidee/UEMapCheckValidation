namespace UnrealBuildTool.Rules
{
    public class MapCheckValidation : ModuleRules
    {
        public MapCheckValidation( ReadOnlyTargetRules Target )
            : base( Target )
        {
            PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

            PublicDependencyModuleNames.AddRange(
                new string[] { 
                    "Core",
                    "CoreUObject",
                    "Engine",
                    "TargetPlatform",
                    "AssetRegistry",
                    "DeveloperSettings",
                    "AIModule"
                }
            );

            PrivateDependencyModuleNames.AddRange(
                new string[] {
                    "UnrealEd"
                }
            );

            PrivateIncludePaths.AddRange(
                new string[]
                {
                    "Private"
                }
            );
        }
    }
}
