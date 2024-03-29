#include "Validators/MapCheckValidator_Collisions.h"

#include <Logging/MessageLog.h>
#include <Misc/UObjectToken.h>

#if WITH_EDITOR
void AMapCheckValidator_Collisions::CheckForErrors()
{
    Super::CheckForErrors();

    FMessageLog map_check( "MapCheck" );

    for ( TObjectIterator< UPrimitiveComponent > primitive_component_iterator; primitive_component_iterator; ++primitive_component_iterator )
    {
        const auto * primitive_component = *primitive_component_iterator;
        if ( !IsValid( primitive_component ) )
        {
            continue;
        }

        const auto * actor = primitive_component->GetOwner();
        if ( !IsValid( actor ) )
        {
            continue;
        }

        const auto * level = actor->GetLevel();
        if ( !IsValid( level ) )
        {
            continue;
        }

        if ( level->IsCurrentLevel() )
        {
            continue;
        }

        const auto collision_profile = primitive_component->GetCollisionProfileName();

        if ( ForbiddenPresetNames.Contains( collision_profile ) )
        {
            map_check.Error()
                ->AddToken( FUObjectToken::Create( this ) )
                ->AddToken( FTextToken::Create( FText::FromString( "Actor" ) ) )
                ->AddToken( FUObjectToken::Create( actor ) )
                ->AddToken( FTextToken::Create(
                    FText::FromString(
                        FString::Printf( TEXT( "in map %s has component %s with forbidden collision preset %s." ),
                            *level->GetName(),
                            *primitive_component->GetName(),
                            *collision_profile.ToString() ) ) ) );
        }

        for ( const auto channel : ForbiddenBlockChannels )
        {
            const auto response = primitive_component->GetCollisionResponseToChannel( channel );
            if ( response == ECR_Block )
            {
                map_check.Error()
                    ->AddToken( FUObjectToken::Create( this ) )
                    ->AddToken( FTextToken::Create( FText::FromString( "Actor" ) ) )
                    ->AddToken( FUObjectToken::Create( actor ) )
                    ->AddToken( FTextToken::Create(
                        FText::FromString(
                            FString::Printf( TEXT( "in map %s has component %s with forbidden blocking collision respone %s." ),
                                *level->GetName(),
                                *primitive_component->GetName(),
                                *collision_profile.ToString() ) ) ) );
            }
        }
    }
}
#endif