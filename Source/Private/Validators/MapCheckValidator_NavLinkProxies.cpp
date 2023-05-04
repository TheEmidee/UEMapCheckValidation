#include "Validators/MapCheckValidator_NavLinkProxies.h"

#include "NavLinkCustomComponent.h"
#include "Navigation/NavLinkProxy.h"

#include <EngineUtils.h>
#include <Misc/UObjectToken.h>

#if WITH_EDITOR
void AMapCheckValidator_NavLinkProxies::CheckForErrors()
{
    Super::CheckForErrors();

    FMessageLog MapCheck( "MapCheck" );

    for ( TActorIterator< ANavLinkProxy > actor_iterator( GetWorld() ); actor_iterator; ++actor_iterator )
    {
        const auto * actor = *actor_iterator;

        if ( actor->IsEditorOnly() )
        {
            continue;
        }

        const auto * actor_level = actor->GetLevel();
        const auto actor_level_name = actor_level->GetOuter()->GetName();

        TArray< FNavigationLink > point_links;
        TArray< FNavigationSegmentLink > segment_links;

        actor->GetNavigationLinksArray( point_links, segment_links );

        const auto check_supported_agents_for_link = [ & ]( const FNavigationLink & nav_link ) {
            if ( !nav_link.SupportedAgents.IsSame( SupportedAgents ) )
            {
                MapCheck.Error()
                    ->AddToken( FUObjectToken::Create( this ) )
                    ->AddToken( FTextToken::Create( FText::FromString( "NavLinkProxy" ) ) )
                    ->AddToken( FUObjectToken::Create( actor ) )
                    ->AddToken( FTextToken::Create( FText::FromString( "in map" ) ) )
                    ->AddToken( FTextToken::Create( FText::FromString( actor_level_name ) ) )
                    ->AddToken( FTextToken::Create( FText::FromString( "has invalid SupportAgents." ) ) );
            }
        };

        const auto check_supported_agents_for_links = [ & ]( const TArray< FNavigationLink > & navigation_links ) {
            for ( const auto & nav_link : navigation_links )
            {
                check_supported_agents_for_link( nav_link );
            }
        };

        const auto * smart_link_component = actor->GetSmartLinkComp();
        const auto smart_link_modifier = smart_link_component->GetLinkModifier();
        auto point_links_without_smart_link_modifier = point_links;
        point_links_without_smart_link_modifier.RemoveAll( [ smart_link_modifier ]( const FNavigationLink & link ) {
            return link.Left == smart_link_modifier.Left && link.Right == smart_link_modifier.Right && link.Direction == smart_link_modifier.Direction && link.SupportedAgents.IsSame( smart_link_modifier.SupportedAgents );
        } );

        if ( actor->IsSmartLinkEnabled() )
        {
            if ( bForbidSmartNavLinks )
            {
                MapCheck.Error()
                    ->AddToken( FUObjectToken::Create( this ) )
                    ->AddToken( FTextToken::Create( FText::FromString( "NavLinkProxy" ) ) )
                    ->AddToken( FUObjectToken::Create( actor ) )
                    ->AddToken( FTextToken::Create( FText::FromString( "in map" ) ) )
                    ->AddToken( FTextToken::Create( FText::FromString( actor_level_name ) ) )
                    ->AddToken( FTextToken::Create( FText::FromString( "has smart link enabled." ) ) );
            }
            else if ( point_links_without_smart_link_modifier.Num() > 1 )
            {
                MapCheck.Error()
                    ->AddToken( FUObjectToken::Create( this ) )
                    ->AddToken( FTextToken::Create( FText::FromString( "NavLinkProxy" ) ) )
                    ->AddToken( FUObjectToken::Create( actor ) )
                    ->AddToken( FTextToken::Create( FText::FromString( "in map" ) ) )
                    ->AddToken( FTextToken::Create( FText::FromString( actor_level_name ) ) )
                    ->AddToken( FTextToken::Create( FText::FromString( "has smart link enabled but has also point links." ) ) );
            }
            else
            {
                check_supported_agents_for_link( smart_link_modifier );
            }
        }
        else
        {
            // The second check is here to prevent from spitting an error when we have a smart link that is valid but disabled
            if ( bForbidSimpleNavLinks && !point_links_without_smart_link_modifier.IsEmpty() )
            {
                MapCheck.Error()
                    ->AddToken( FUObjectToken::Create( this ) )
                    ->AddToken( FTextToken::Create( FText::FromString( "NavLinkProxy" ) ) )
                    ->AddToken( FUObjectToken::Create( actor ) )
                    ->AddToken( FTextToken::Create( FText::FromString( "in map" ) ) )
                    ->AddToken( FTextToken::Create( FText::FromString( actor_level_name ) ) )
                    ->AddToken( FTextToken::Create( FText::FromString( "does not have smart link enabled." ) ) );
            }
            else if ( point_links.IsEmpty() )
            {
                MapCheck.Error()
                    ->AddToken( FUObjectToken::Create( this ) )
                    ->AddToken( FTextToken::Create( FText::FromString( "NavLinkProxy" ) ) )
                    ->AddToken( FUObjectToken::Create( actor ) )
                    ->AddToken( FTextToken::Create( FText::FromString( "in map" ) ) )
                    ->AddToken( FTextToken::Create( FText::FromString( actor_level_name ) ) )
                    ->AddToken( FTextToken::Create( FText::FromString( "has simple link enabled but no points are defined." ) ) );
            }
            else
            {
                check_supported_agents_for_links( point_links );
            }
        }
    }
}
#endif