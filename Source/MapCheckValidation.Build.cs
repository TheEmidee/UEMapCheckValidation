namespace UnrealBuildTool.Rules
{
    public class MapCheckValidation : ModuleRules
    {
        public MapCheckValidation( ReadOnlyTargetRules Target )
            : base( Target )
        {
            PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
            bEnforceIWYU = true;
            
            //PrivateIncludePaths.Add("MapCheckValidation/Private");

            PublicDependencyModuleNames.AddRange(
                new string[] { 
                    "Core",
                    "CoreUObject",
                    "Engine",
                    "TargetPlatform",
                    "AssetRegistry"
                }
            );

            PrivateDependencyModuleNames.AddRange(
                new string[] {
                    "Slate",
                    "SlateCore",
                    "UnrealEd",
                    "AssetRegistry",
                    "EditorStyle",
                    "Blutility"
                }
            );
        }
    }
}
