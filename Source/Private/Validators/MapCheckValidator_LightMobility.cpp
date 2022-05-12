#include "Validators/MapCheckValidator_LightMobility.h"

#include <EngineUtils.h>
#include <Misc/UObjectToken.h>

#if WITH_EDITOR
void AMapCheckValidator_LightMobility::CheckForErrors()
{
    Super::CheckForErrors();

    FMessageLog MapCheck( "MapCheck" );

    UClass * actor_class = nullptr;

    for ( TActorIterator< AActor > actor_iterator( GetWorld(), actor_class ); actor_iterator; ++actor_iterator )
    {
        const auto * actor = *actor_iterator;

        if ( actor->IsEditorOnly() )
        {
            continue;
        }

        if ( actor->GetClass() == actor_class )
        {
            const auto * actor_level = actor->GetLevel();
            const auto actor_level_name = actor_level->GetOuter()->GetName();

            MapCheck.Error()
                ->AddToken( FUObjectToken::Create( this ) )
                ->AddToken( FTextToken::Create( FText::FromString( "Actor" ) ) )
                ->AddToken( FUObjectToken::Create( actor ) )
                ->AddToken( FTextToken::Create( FText::FromString( FString::Printf( TEXT( "in map %s is an actor of type %s or derived and is forbidden in the level." ), *actor_level_name, *GetNameSafe( actor_class ) ) ) ) );
        }
    }
}
#endif