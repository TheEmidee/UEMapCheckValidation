#include "Validators/MapCheckValidator_StaticSubLevels.h"

#include <EngineUtils.h>
#include <Misc/UObjectToken.h>
#include <Particles/ParticleSystemComponent.h>

AMapCheckValidator_StaticSubLevels::AMapCheckValidator_StaticSubLevels()
{
    MapSuffix = TEXT( "_Art" );

    PrimitiveComponentClassesToIgnore.Emplace( USkeletalMeshComponent::StaticClass() );
    PrimitiveComponentClassesToIgnore.Emplace( UFXSystemComponent::StaticClass() );
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
        }

        // It's impossible to get all actors from a sub-level only.
        // So, we must get all actors and filter by their ULevel.
        for ( TActorIterator< AActor > actor_iterator( GetWorld() ); actor_iterator; ++actor_iterator )
        {
            const AActor * actor = *actor_iterator;

            if ( actor->IsEditorOnly() )
            {
                continue;
            }

            const auto * actor_level = actor->GetLevel();
            const auto actor_level_name = actor_level->GetOuter()->GetName();

            if ( actor_level_name.EndsWith( MapSuffix ) )
            {
                TInlineComponentArray< UPrimitiveComponent * > primitive_components( actor );

                for ( const auto * primitive_component : primitive_components )
                {
                    if ( primitive_component == nullptr )
                    {
                        continue;
                    }

                    if ( primitive_component->IsEditorOnly() )
                    {
                        continue;
                    }

                    if ( PrimitiveComponentClassesToIgnore.FindByPredicate( [ primitive_component ]( const TSubclassOf< UPrimitiveComponent > & primitive_component_class_to_ignore ) {
                             return primitive_component->IsA( primitive_component_class_to_ignore );
                         } ) != nullptr )
                    {
                        continue;
                    }

                    if ( primitive_component->Mobility != EComponentMobility::Static )
                    {
                        MapCheck.Warning()
                            ->AddToken( FUObjectToken::Create( this ) )
                            ->AddToken( FTextToken::Create( FText::FromString( "Actor" ) ) )
                            ->AddToken( FUObjectToken::Create( actor ) )
                            ->AddToken( FTextToken::Create( FText::FromString( FString::Printf( TEXT( "in map %s" ), *actor_level_name ) ) ) )
                            ->AddToken( FTextToken::Create( FText::FromString( "has a component" ) ) )
                            ->AddToken( FUObjectToken::Create( primitive_component ) )
                            ->AddToken( FTextToken::Create( FText::FromString( "with a non-static mobility. All components in a static sub-level must be static." ) ) );
                    }
                }
            }
        }
    }
}
#endif