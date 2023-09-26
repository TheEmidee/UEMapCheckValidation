#include "Validators/MapCheckValidator_LightMobility.h"

#include <Components/DirectionalLightComponent.h>
#include <Components/LightComponentBase.h>
#include <Engine/Light.h>
#include <EngineUtils.h>
#include <Logging/MessageLog.h>
#include <Misc/UObjectToken.h>

#if WITH_EDITOR
void AMapCheckValidator_LightMobility::CheckForErrors()
{
    Super::CheckForErrors();

    FMessageLog map_check( "MapCheck" );

    for ( TObjectIterator< ULightComponentBase > light_component_iterator; light_component_iterator; ++light_component_iterator )
    {
        const auto * light_component = *light_component_iterator;

        if ( !IsValid( light_component ) )
        {
            continue;
        }

        const auto * light_actor = light_component->GetOwner();

        if ( light_actor == nullptr )
        {
            continue;
        }

        const auto light_level = light_actor->GetLevel();

        if ( light_level == nullptr )
        {
            continue;
        }

        const auto light_mobility = light_component->Mobility.GetValue();

        auto has_expected_mobility = light_mobility == EComponentMobility::Static;
        auto expected_mobility_string = FString( "Static" );
        const auto not_static_tag = FString( "NotStatic" );

        if ( light_component->IsA< UDirectionalLightComponent >() || light_component->ComponentHasTag( *not_static_tag ) )
        {
            has_expected_mobility = light_mobility == EComponentMobility::Static || EComponentMobility::Stationary;
            expected_mobility_string = FString( "Static or Stationary" );
        }

        if ( !has_expected_mobility )
        {
            const auto light_component_name = GetNameSafe( light_component );
            const auto light_mobility_string = UEnum::GetDisplayValueAsText( light_mobility ).ToString();
            auto light_level_name = light_level->GetOuter()->GetName();

            map_check.Error()
                ->AddToken( FUObjectToken::Create( this ) )
                ->AddToken( FTextToken::Create( FText::FromString( "Actor" ) ) )
                ->AddToken( FUObjectToken::Create( light_actor ) )
                ->AddToken( FTextToken::Create(
                    FText::FromString(
                        FString::Printf( TEXT( "in map %s has component %s with light mobility %s instead of %s. To allow lights to not be static, add the tag %s to the light component." ),
                            *light_level_name,
                            *light_component_name,
                            *light_mobility_string,
                            *expected_mobility_string,
                            *not_static_tag ) ) ) );
        }
    }
}
#endif