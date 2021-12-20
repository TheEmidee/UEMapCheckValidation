namespace UnrealBuildTool.Rules
{
    public class MapCheckValidation : ModuleRules
    {
        public MapCheckValidation( ReadOnlyTargetRules Target )
            : base( Target )
        {
            PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
            bEnforceIWYU = true;
            
            PublicDependencyModuleNames.AddRange(
                new string[] { 
                    "Core",
                    "CoreUObject",
                    "Engine",
                    "TargetPlatform",
                    "AssetRegistry",
                    "DeveloperSettings"
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
                    "MapCheckValidation/Private"
                }
            );
        }
    }
}
