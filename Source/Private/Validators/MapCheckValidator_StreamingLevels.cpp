#include "Validators/MapCheckValidator_StreamingLevels.h"

#include <Misc/UObjectToken.h>

FMapCheckValidatorStreamingLevelFlags::FMapCheckValidatorStreamingLevelFlags() :
    bMustBeSet( false )
{
}

AMapCheckValidator_StreamingLevels::AMapCheckValidator_StreamingLevels()
{
    LevelPrefix = TEXT( "L_" );
    bLevelStreamingLevelsMustBeInRootFolder = true;
    LevelsRootFolder = TEXT( "" );
    bLevelStreamingLevelNamesMustStartWithPersistentLevelName = true;
}

#if WITH_EDITOR

void AMapCheckValidator_StreamingLevels::CheckForErrors()
{
    Super::CheckForErrors();

    FMessageLog MapCheck( "MapCheck" );

    if ( const auto * world = GetWorld() )
    {
        const auto & streaming_levels = world->GetStreamingLevels();
        const auto world_name = world->GetMapName();
        auto map_folder_name = world_name;

        if ( !LevelPrefix.IsEmpty() )
        {
            map_folder_name.RemoveFromStart( LevelPrefix );
        }

        for ( const auto * streaming_level : streaming_levels )
        {
            FString package_root;
            FString package_name;
            FString package_path;

            FPackageName::SplitLongPackageName( streaming_level->GetWorldAssetPackageName(), package_root, package_path, package_name );

            if ( bLevelStreamingLevelsMustBeInRootFolder )
            {
                auto root_folder = LevelsRootFolder;
                root_folder.RemoveFromStart( TEXT( "/" ) );
                root_folder.RemoveFromEnd( TEXT( "/" ) );

                const auto folder_name = FString::Printf( TEXT( "%s/%s" ), *root_folder, *map_folder_name );

                if ( !package_path.StartsWith( folder_name ) )
                {
                    MapCheck.Warning()
                        ->AddToken( FUObjectToken::Create( this ) )
                        ->AddToken( FTextToken::Create( FText::FromString( FString::Printf( TEXT( "SubLevel %s" ), *package_name ) ) ) )
                        ->AddToken( FTextToken::Create( FText::FromString( FString::Printf( TEXT( "must be in the folder : %s" ), *folder_name ) ) ) );
                }
            }

            if ( bLevelStreamingLevelNamesMustStartWithPersistentLevelName )
            {
                if ( !package_name.RemoveFromStart( world_name ) )
                {
                    MapCheck.Warning()
                        ->AddToken( FUObjectToken::Create( this ) )
                        ->AddToken( FTextToken::Create( FText::FromString( FString::Printf( TEXT( "SubLevel %s" ), *package_name ) ) ) )
                        ->AddToken( FTextToken::Create( FText::FromString( FString::Printf( TEXT( "must start with the name of the persistent level : %s" ), *world_name ) ) ) );
                }
            }

            const auto check_boolean = [ &MapCheck, this, package_name ]( const bool actual_value, const FMapCheckValidatorStreamingLevelFlags & flag, const FString & error_message ) {
                for ( const auto & token : flag.MapNameRequiredTokens )
                {
                    if ( !package_name.Contains( token ) )
                    {
                        return;
                    }
                }

                for ( const auto & token : flag.MapNameExcludedTokens )
                {
                    if ( package_name.Contains( token ) )
                    {
                        return;
                    }
                }

                if ( actual_value != flag.bMustBeSet )
                {
                    MapCheck.Warning()
                        ->AddToken( FUObjectToken::Create( this ) )
                        ->AddToken( FTextToken::Create( FText::FromString( FString::Printf( TEXT( "SubLevel %s" ), *package_name ) ) ) )
                        ->AddToken( FTextToken::Create( FText::FromString( error_message ) ) );
                }
            };

            check_boolean( streaming_level->GetShouldBeVisibleFlag(), ShouldBeVisibleFlagCheck, "must be visible" );
            check_boolean( streaming_level->ShouldBeAlwaysLoaded(), ShouldAlwaysBeLoadedFlagCheck, "must not be always loaded" );
            check_boolean( streaming_level->bShouldBlockOnLoad, ShouldBlockOnLoadFlagCheck, "must not block on load" );
            check_boolean( streaming_level->bShouldBlockOnUnload, ShouldBlockOnUnloadFlagCheck, "must not block on unload" );
            check_boolean( streaming_level->bDisableDistanceStreaming, DisableDistanceStreamingFlagCheck, "must not disable distance streaming" );
        }
    }
}
#endif