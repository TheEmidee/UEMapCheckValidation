#include "Validators/MapCheckValidator_StaticMeshesLODs.h"

#include <EngineUtils.h>
#include <Misc/UObjectToken.h>

#if WITH_EDITOR
void AMapCheckValidator_StaticMeshesLODs::CheckForErrors()
{
    Super::CheckForErrors();

    FMessageLog MapCheck( "MapCheck" );

    for ( TActorIterator< AActor > actor_iterator( GetWorld() ); actor_iterator; ++actor_iterator )
    {
        const auto * actor = *actor_iterator;

        if ( actor->IsEditorOnly() )
        {
            continue;
        }

        const auto * actor_level = actor->GetLevel();
        const auto actor_level_name = actor_level->GetOuter()->GetName();

        TInlineComponentArray< UStaticMeshComponent * > static_mesh_components( actor );

        for ( const auto * primitive_component : static_mesh_components )
        {
            if ( primitive_component == nullptr )
            {
                continue;
            }

            if ( primitive_component->IsEditorOnly() )
            {
                continue;
            }

            auto * static_mesh = primitive_component->GetStaticMesh();

            if ( static_mesh == nullptr )
            {
                MapCheck.Error()
                    ->AddToken( FUObjectToken::Create( this ) )
                    ->AddToken( FTextToken::Create( FText::FromString( "Actor" ) ) )
                    ->AddToken( FUObjectToken::Create( actor ) )
                    ->AddToken( FTextToken::Create( FText::FromString( FString::Printf( TEXT( "in map %s" ), *actor_level_name ) ) ) )
                    ->AddToken( FTextToken::Create( FText::FromString( "on component" ) ) )
                    ->AddToken( FUObjectToken::Create( primitive_component ) )
                    ->AddToken( FTextToken::Create( FText::FromString( "has no static mesh" ) ) );

                continue;
            }

            if ( static_mesh->GetNumLODs() <= 1 )
            {
                MapCheck.Warning()
                    ->AddToken( FUObjectToken::Create( this ) )
                    ->AddToken( FTextToken::Create( FText::FromString( "Actor" ) ) )
                    ->AddToken( FUObjectToken::Create( actor ) )
                    ->AddToken( FTextToken::Create( FText::FromString( FString::Printf( TEXT( "in map %s" ), *actor_level_name ) ) ) )
                    ->AddToken( FTextToken::Create( FText::FromString( "on component" ) ) )
                    ->AddToken( FUObjectToken::Create( primitive_component ) )
                    ->AddToken( FTextToken::Create( FText::FromString( "has no LODs" ) ) );

                continue;
            }

            if ( static_mesh->LODGroup == NAME_None )
            {
                MapCheck.Warning()
                    ->AddToken( FUObjectToken::Create( this ) )
                    ->AddToken( FTextToken::Create( FText::FromString( "Actor" ) ) )
                    ->AddToken( FUObjectToken::Create( actor ) )
                    ->AddToken( FTextToken::Create( FText::FromString( FString::Printf( TEXT( "in map %s" ), *actor_level_name ) ) ) )
                    ->AddToken( FTextToken::Create( FText::FromString( "on component" ) ) )
                    ->AddToken( FUObjectToken::Create( primitive_component ) )
                    ->AddToken( FTextToken::Create( FText::FromString( "has no LOD group" ) ) );
            }
        }
    }
}
#endif