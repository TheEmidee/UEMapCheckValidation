#include "Validators/MapCheckValidator_PhysicalMaterials.h"

#include "Materials/Material.h"

#include <EngineUtils.h>
#include <Misc/UObjectToken.h>

// This comes from https://alanedwardes.com/blog/posts/custom-map-checker-ue4/

#if WITH_EDITOR
void AMapCheckValidator_PhysicalMaterials::CheckForErrors()
{
    Super::CheckForErrors();

    FMessageLog MapCheck( "MapCheck" );

    TSet< const UMaterialInterface * > seen_materials;

    for ( TActorIterator< AActor > actor_iterator( GetWorld() ); actor_iterator; ++actor_iterator )
    {
        const auto * actor = *actor_iterator;

        if ( actor->IsEditorOnly() )
        {
            continue;
        }

        const auto * actor_level = actor->GetLevel();
        const auto actor_level_name = actor_level->GetOuter()->GetName();

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

            if ( !primitive_component->GetCollisionEnabled() )
            {
                continue;
            }

            const auto num_materials = primitive_component->GetNumMaterials();
            for ( auto material_index = 0; material_index < num_materials; material_index++ )
            {
                const auto * material_interface = primitive_component->GetMaterial( material_index );
                if ( material_interface == nullptr )
                {
                    continue;
                }

                if ( seen_materials.Contains( material_interface ) )
                {
                    continue;
                }

                seen_materials.Add( material_interface );

                if ( material_interface->GetBlendMode() == EBlendMode::BLEND_Additive )
                {
                    continue;
                }

                if ( auto * material = material_interface->GetMaterial() )
                {
                    if ( material->MaterialDomain != MD_Surface )
                    {
                        continue;
                    }
                }

                if ( material_interface->GetPhysicalMaterial() == nullptr || material_interface->GetPhysicalMaterial() == GEngine->DefaultPhysMaterial )
                {
                    MapCheck.Warning()
                        ->AddToken( FUObjectToken::Create( this ) )
                        ->AddToken( FTextToken::Create( FText::FromString( "Actor" ) ) )
                        ->AddToken( FUObjectToken::Create( actor ) )
                        ->AddToken( FTextToken::Create( FText::FromString( FString::Printf( TEXT( "in map %s" ), *actor_level_name ) ) ) )
                        ->AddToken( FTextToken::Create( FText::FromString( "on component" ) ) )
                        ->AddToken( FUObjectToken::Create( primitive_component ) )
                        ->AddToken( FTextToken::Create( FText::FromString( "has material" ) ) )
                        ->AddToken( FUObjectToken::Create( material_interface ) )
                        ->AddToken( FTextToken::Create( FText::FromString( "which does not have a valid physical material" ) ) );
                }
            }
        }
    }
}
#endif