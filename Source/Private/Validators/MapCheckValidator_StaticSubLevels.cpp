#include "Validators/MapCheckValidator_StaticSubLevels.h"

#include "EngineUtils.h"

#include <Misc/UObjectToken.h>

AMapCheckValidator_StaticSubLevels::AMapCheckValidator_StaticSubLevels()
{
    MapSuffix = TEXT( "_Art" );
}

#if WITH_EDITOR

void AMapCheckValidator_StaticSubLevels::CheckForErrors()
{
    Super::CheckForErrors();

    FMessageLog MapCheck( "MapCheck" );

    if ( const auto * world = GetWorld() )
    {
        const auto & streaming_levels = world->GetStreamingLevels();

        for ( const auto * streaming_level : streaming_levels )
        {
            FString package_root;
            FString package_name;
            FString package_path;

            FPackageName::SplitLongPackageName( streaming_level->GetWorldAssetPackageName(), package_root, package_path, package_name );

            if ( !package_name.EndsWith( MapSuffix ) )
            {
                continue;
            }

            if ( !streaming_level->bIsStatic )
            {
                MapCheck.Warning()
                    ->AddToken( FUObjectToken::Create( this ) )
                    ->AddToken( FTextToken::Create( FText::FromString( FString::Printf( TEXT( "SubLevel %s" ), *package_name ) ) ) )
                    ->AddToken( FTextToken::Create( FText::FromString( TEXT( "must be set static" ) ) ) );
            }

            for ( TActorIterator< AActor > It( streaming_level->GetWorldAsset().Get() ); It; ++It )
            {
                AActor * Actor = *It;
            }
        }
    }
}
#endif