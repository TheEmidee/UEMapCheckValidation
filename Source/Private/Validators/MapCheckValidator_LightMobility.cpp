#include "Validators/MapCheckValidator_LightMobility.h"

#include <Components/LightComponentBase.h>
#include <Engine/Light.h>
#include <EngineUtils.h>
#include <Misc/UObjectToken.h>

#if WITH_EDITOR
void AMapCheckValidator_LightMobility::CheckForErrors()
{
    Super::CheckForErrors();

    FMessageLog MapCheck( "MapCheck" );

    UClass * actor_class = nullptr;

    for ( TObjectIterator< ULightComponentBase > light_component_iterator; light_component_iterator; ++light_component_iterator )
    {
        const auto * light_component = *light_component_iterator;

        auto owner_name = light_component->GetOwner()->GetName();
        auto component_name = GetNameSafe( light_component );
        const auto light_mobility = light_component->Mobility.GetValue();
        auto light_mobility_string = UEnum::GetDisplayValueAsText( light_mobility ).ToString();

        if ( light_component->IsA< UDirectionalLightComponent >() )
        {
            if ( light_mobility != EComponentMobility::Stationary )
            {
                MapCheck.Error()
                    ->AddToken( FUObjectToken::Create( this ) )
                    ->AddToken( FTextToken::Create( FText::FromString( "Component" ) ) )
                    ->AddToken( FUObjectToken::Create( light_component ) )
                    ->AddToken( FTextToken::Create( FText::FromString( FString::Printf( TEXT( "in blueprint %s is %s with light mobility %s instead of Stationary." ), *owner_name, *component_name, *light_mobility_string ) ) ) );

                continue;
            }
        }

        if ( light_mobility != EComponentMobility::Static )
        {
            MapCheck.Error()
                ->AddToken( FUObjectToken::Create( this ) )
                ->AddToken( FTextToken::Create( FText::FromString( "Component" ) ) )
                ->AddToken( FUObjectToken::Create( light_component ) )
                ->AddToken( FTextToken::Create( FText::FromString( FString::Printf( TEXT( "in blueprint %s is %s with light mobility %s instead of Static." ), *owner_name, *component_name, *light_mobility_string ) ) ) );
        }
    }
}
#endif