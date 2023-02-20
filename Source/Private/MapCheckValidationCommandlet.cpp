#include "MapCheckValidationCommandlet.h"

#include <AI/NavigationSystemBase.h>
#include <Editor.h>
#include <Engine/LevelStreaming.h>
#include <Engine/World.h>
#include <Misc/PackageName.h>

// ReSharper disable once CppInconsistentNaming
DEFINE_LOG_CATEGORY_STATIC( LogMapCheckValidation, Log, All )

UMapCheckValidationCommandlet::UMapCheckValidationCommandlet()
{
    LogToConsole = false;
}

int32 UMapCheckValidationCommandlet::Main( const FString & params )
{
    UE_LOG( LogMapCheckValidation, Display, TEXT( "--------------------------------------------------------------------------------------------" ) );
    UE_LOG( LogMapCheckValidation, Display, TEXT( "Running MapCheckValidation Commandlet" ) );
    TArray< FString > tokens;
    TArray< FString > switches;
    TMap< FString, FString > params_map;
    ParseCommandLine( *params, tokens, switches, params_map );

    TArray< FString > package_names;

    for ( const auto & param_key_pair : params_map )
    {
        if ( param_key_pair.Key == "Maps" )
        {
            auto map_parameter_value = param_key_pair.Value;

            const auto add_package = [ &package_names ]( const FString & package_name ) {
                FString map_file;
                FPackageName::SearchForPackageOnDisk( package_name, nullptr, &map_file );

                if ( map_file.IsEmpty() )
                {
                    UE_LOG( LogMapCheckValidation, Error, TEXT( "Could not find package %s" ), *package_name );
                }
                else
                {
                    package_names.Add( *map_file );
                }
            };

            // Allow support for -Map=Value1+Value2+Value3
            TArray< FString > maps_package_names;
            map_parameter_value.ParseIntoArray( maps_package_names, TEXT( "+" ) );

            if ( maps_package_names.Num() > 0 )
            {
                for ( const auto & map_package_name : maps_package_names )
                {
                    add_package( map_package_name );
                }
            }
            else
            {
                add_package( map_parameter_value );
            }
        }
    }

    if ( package_names.Num() == 0 )
    {
        UE_LOG( LogMapCheckValidation, Error, TEXT( "No maps were checked" ) );
        return 2;
    }

    for ( const auto & package_name : package_names )
    {
        UE_LOG( LogMapCheckValidation, Display, TEXT( "Will process %s" ), *package_name );

        auto * package = LoadPackage( nullptr, *package_name, 0 );
        if ( package == nullptr )
        {
            UE_LOG( LogMapCheckValidation, Error, TEXT( "Cannot load package %s" ), *package_name );
            return 2;
        }

        auto * world = UWorld::FindWorldInPackage( package );
        if ( world == nullptr )
        {
            UE_LOG( LogMapCheckValidation, Error, TEXT( "Cannot get a world in the package %s" ), *package_name );
            return 2;
        }

        world->WorldType = EWorldType::Editor;
        world->AddToRoot();
        if ( !world->bIsWorldInitialized )
        {
            UWorld::InitializationValues ivs;
            ivs.RequiresHitProxies( false );
            ivs.ShouldSimulatePhysics( false );
            ivs.EnableTraceCollision( false );
            ivs.CreateNavigation( true );
            ivs.CreateAISystem( false );
            ivs.AllowAudioPlayback( false );
            ivs.CreatePhysicsScene( true );

            world->InitWorld( ivs );
            world->PersistentLevel->UpdateModelComponents();
            world->UpdateWorldComponents( true, false );
        }

        auto & world_context = GEditor->GetEditorWorldContext( true );
        world_context.SetCurrentWorld( world );
        GWorld = world;

        world->LoadSecondaryLevels( true, nullptr );

        FNavigationSystem::AddNavigationSystemToWorld( *world, FNavigationSystemRunMode::EditorMode );

        const auto & streaming_levels = world->GetStreamingLevels();

        for ( auto * streaming_level : streaming_levels )
        {
            streaming_level->SetShouldBeVisible( true );
            streaming_level->SetShouldBeLoaded( true );

#if WITH_EDITOR
            streaming_level->SetShouldBeVisibleInEditor( true );
#endif
        }

        UE_LOG( LogMapCheckValidation, Display, TEXT( "Load %i streaming levels for world %s" ), streaming_levels.Num(), *world->GetName() );

        world->FlushLevelStreaming( EFlushLevelStreamingType::Full );

        GEditor->HandleMapCommand( TEXT( "CHECK" ), *GLog, world );

        world->RemoveFromRoot();

        world_context.SetCurrentWorld( nullptr );
        GWorld = nullptr;

        UE_LOG( LogMapCheckValidation, Display, TEXT( "Finished processing of %s" ), *package_name );
    }

    UE_LOG( LogMapCheckValidation, Display, TEXT( "Successfully finished running MapCheckValidation Commandlet" ) );
    UE_LOG( LogMapCheckValidation, Display, TEXT( "--------------------------------------------------------------------------------------------" ) );
    return 0;
}
