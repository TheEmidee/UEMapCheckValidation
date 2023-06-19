#include "Validators/MapCheckValidator_NoActorOfClass.h"

#include <EngineUtils.h>
#include <Logging/MessageLog.h>
#include <Misc/UObjectToken.h>

#if WITH_EDITOR
void AMapCheckValidator_NoActorOfClass::CheckForErrors()
{
    Super::CheckForErrors();

    FMessageLog map_check( "MapCheck" );

    for ( const auto & forbidden_class : ForbiddenActorClasses )
    {
        for ( TActorIterator< AActor > actor_iterator( GetWorld(), forbidden_class ); actor_iterator; ++actor_iterator )
        {
            const auto * actor = *actor_iterator;

            if ( actor->IsEditorOnly() )
            {
                continue;
            }

            if ( actor->GetClass() == forbidden_class )
            {
                const auto * actor_level = actor->GetLevel();
                const auto actor_level_name = actor_level->GetOuter()->GetName();

                map_check.Error()
                    ->AddToken( FUObjectToken::Create( this ) )
                    ->AddToken( FTextToken::Create( FText::FromString( "Actor" ) ) )
                    ->AddToken( FUObjectToken::Create( actor ) )
                    ->AddToken( FTextToken::Create( FText::FromString( FString::Printf( TEXT( "in map %s is an actor of type %s or derived and is forbidden in the level." ), *actor_level_name, *GetNameSafe( forbidden_class ) ) ) ) );
            }
        }
    }
}
#endif