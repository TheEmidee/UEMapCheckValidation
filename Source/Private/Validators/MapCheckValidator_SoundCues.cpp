#include "Validators/MapCheckValidator_SoundCues.h"

#include <Components/AudioComponent.h>
#include <Misc/UObjectToken.h>
#include <Sound/SoundCue.h>

void AMapCheckValidator_SoundCues::CheckForErrors()
{
    Super::CheckForErrors();

    FMessageLog MapCheck( "MapCheck" );

    for ( TObjectIterator< UAudioComponent > audio_component_iterator; audio_component_iterator; ++audio_component_iterator )
    {
        const auto * audio_component = *audio_component_iterator;

        if ( !IsValid( audio_component ) )
        {
            continue;
        }

        const auto * audio_actor = audio_component->GetOwner();

        if ( !IsValid( audio_actor ) )
        {
            continue;
        }

        const auto * audio_level = audio_actor->GetLevel();

        if ( !IsValid( audio_level ) )
        {
            continue;
        }

        const auto * sound = audio_component->Sound;

        if ( !IsValid( sound ) )
        {
            MapCheck.Error()
                ->AddToken( FUObjectToken::Create( this ) )
                ->AddToken( FTextToken::Create( FText::FromString( "Actor" ) ) )
                ->AddToken( FUObjectToken::Create( audio_actor ) )
                ->AddToken( FTextToken::Create(
                    FText::FromString(
                        FString::Printf(
                            TEXT( "in map %s does not have a valid Sound Cue set." ),
                            *audio_level->GetOuter()->GetName() ) ) ) );

            continue;
        }

        const auto * sound_cue = Cast< USoundCue >( sound );

        if ( !IsValid( sound_cue ) )
        {
            MapCheck.Error()
                ->AddToken( FUObjectToken::Create( this ) )
                ->AddToken( FTextToken::Create( FText::FromString( "Actor" ) ) )
                ->AddToken( FUObjectToken::Create( audio_actor ) )
                ->AddToken( FTextToken::Create(
                    FText::FromString(
                        FString::Printf(
                            TEXT( "in map %s has a Sound set, but it is not a Sound Cue." ),
                            *audio_level->GetOuter()->GetName() ) ) ) );

            continue;
        }

        const auto * sound_class = sound_cue->SoundClassObject;

        if ( !IsValid( sound_class ) )
        {
            MapCheck.Error()
                ->AddToken( FUObjectToken::Create( this ) )
                ->AddToken( FTextToken::Create( FText::FromString( "Sound Cue" ) ) )
                ->AddToken( FUObjectToken::Create( sound_cue ) )
                ->AddToken( FTextToken::Create( FText::FromString( "does not have a valid Sound Class set." ) ) );
        }
    }
}
