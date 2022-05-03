#include "Validators/MapCheckValidator_WorldSettings.h"

#include <EngineUtils.h>
#include <Misc/UObjectToken.h>

#if WITH_EDITOR
void AMapCheckValidator_WorldSettings::CheckForErrors()
{
    Super::CheckForErrors();

    FMessageLog MapCheck( "MapCheck" );

    if ( const auto * world = GetWorld() )
    {
        if ( auto * required_settings_class = RequiredWorldSettingsClass.LoadSynchronous() )
        {
            if ( !world->GetWorldSettings()->IsA( required_settings_class ) )
            {
                MapCheck.Error()
                    ->AddToken( FUObjectToken::Create( this ) )
                    ->AddToken( FTextToken::Create( FText::FromString( FString::Printf( TEXT( "The world settings of the level must be of type %s." ), *GetNameSafe( required_settings_class ) ) ) ) );
            }
        }
    }
}
#endif