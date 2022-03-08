#include "Validators/MapCheckValidator_StreamingLevels.h"

#include <Kismet/KismetStringLibrary.h>
#include <Misc/UObjectToken.h>

FMapCheckValidatorStreamingLevelFlags::FMapCheckValidatorStreamingLevelFlags() :
    bShouldBeVisible( false ),
    bShouldBeAlwaysLoaded( false ),
    bShouldBlockOnLoad( false ),
    bShouldBlockOnUnload( false ),
    bDisableDistanceStreaming( false )
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

            const auto long_package_name = streaming_level->GetWorldAssetPackageName();
            FPackageName::SplitLongPackageName( long_package_name, package_root, package_path, package_name );

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

            const auto check_flag = [ &MapCheck, this, package_name ]( const bool actual_value, const bool required_value, const FString & flag_name ) {
                if ( actual_value != required_value )
                {
                    MapCheck.Warning()
                        ->AddToken( FUObjectToken::Create( this ) )
                        ->AddToken( FTextToken::Create( FText::FromString( FString::Printf( TEXT( "SubLevel %s" ), *package_name ) ) ) )
                        ->AddToken( FTextToken::Create( FText::FromString( FString::Printf( TEXT( "must have the %s flag set to %s" ), *flag_name, *UKismetStringLibrary::Conv_BoolToString( required_value ) ) ) ) );
                }
            };

            const auto can_check_level = [ &long_package_name ]( const FMapCheckValidatorStreamingLevelFlags & streaming_level_flags ) {
                if ( streaming_level_flags.MapNameRequiredTokens.Num() > 0 )
                {
                    for ( const auto & token : streaming_level_flags.MapNameRequiredTokens )
                    {
                        if ( long_package_name.Contains( token ) )
                        {
                            return true;
                        }
                    }
                    return false;
                }

                for ( const auto & token : streaming_level_flags.MapNameExcludedTokens )
                {
                    if ( long_package_name.Contains( token ) )
                    {
                        return false;
                    }
                }

                return true;
            };

            for ( const auto & streaming_level_flags : StreamingLevelFlags )
            {
                if ( !can_check_level( streaming_level_flags ) )
                {
                    continue;
                }

                check_flag( streaming_level->GetShouldBeVisibleFlag(), streaming_level_flags.bShouldBeVisible, TEXT( "Visible" ) );
                check_flag( streaming_level->ShouldBeAlwaysLoaded(), streaming_level_flags.bShouldBeAlwaysLoaded, TEXT( "AlwaysLoaded" ) );
                check_flag( streaming_level->bShouldBlockOnLoad, streaming_level_flags.bShouldBlockOnLoad, TEXT( "BlockOnLoad" ) );
                check_flag( streaming_level->bShouldBlockOnUnload, streaming_level_flags.bShouldBlockOnUnload, TEXT( "BlockOnUnload" ) );
                check_flag( streaming_level->bDisableDistanceStreaming, streaming_level_flags.bDisableDistanceStreaming, TEXT( "DisableDistanceStreaming" ) );

                break;
            }
        }
    }
}
#endif