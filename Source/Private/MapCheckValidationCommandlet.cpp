#include "MapCheckValidationCommandlet.h"

#include "MapCheckSettings.h"
#include "MapCheckValidatorBase.h"

#include <Editor.h>
#include <Engine/LevelStreaming.h>
#include <Engine/World.h>
#include <Misc/PackageName.h>

// ReSharper disable once CppInconsistentNaming
DEFINE_LOG_CATEGORY_STATIC( LogMapCheckValidation, Warning, All )

UMapCheckValidationCommandlet::UMapCheckValidationCommandlet()
{
    LogToConsole = false;
}

int32 UMapCheckValidationCommandlet::Main( const FString & params )
{
    UE_LOG( LogMapCheckValidation, Log, TEXT( "--------------------------------------------------------------------------------------------" ) );
    UE_LOG( LogMapCheckValidation, Log, TEXT( "Running MapCheckValidation Commandlet" ) );
    TArray< FString > tokens;
    TArray< FString > switches;
    TMap< FString, FString > params_map;
    ParseCommandLine( *params, tokens, switches, params_map );

    TArray< FString > package_names;

    for ( const auto & param_key_pair : params_map )
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
                    package_names.Add( *map_file );
            };

            // Allow support for -Map=Value1+Value2+Value3
            TArray< FString > maps_package_names;
            map_parameter_value.ParseIntoArray( maps_package_names, TEXT( "," ) );

            if ( maps_package_names.Num() > 0 )
                for ( const auto & map_package_name : maps_package_names )
                    add_package( map_package_name );
            else
                add_package( map_parameter_value );

            /*for ( auto plus_index = maps.Find( TEXT( "+" ), ESearchCase::CaseSensitive ); plus_index != INDEX_NONE; plus_index = maps.Find( TEXT( "+" ), ESearchCase::CaseSensitive ) )
            {
                const auto next_map = maps.Left( plus_index );

                if ( next_map.Len() > 0 )
                {
                    add_package( next_map );
                }

                maps.RightInline( maps.Len() - ( plus_index + 1 ), false );
            }

            add_package( maps );*/
        }

    if ( package_names.Num() == 0 )
    {
        UE_LOG( LogMapCheckValidation, Error, TEXT( "No maps were checked" ) );
        return 2;
    }

    for ( const auto & package_name : package_names )
    {
        UE_LOG( LogMapCheckValidation, Log, TEXT( "Will process %s" ), *package_name );

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
            ivs.CreateNavigation( false );
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

        const auto & streaming_levels = world->GetStreamingLevels();

        for ( auto * streaming_level : streaming_levels )
        {
            streaming_level->SetShouldBeVisible( true );
            streaming_level->SetShouldBeLoaded( true );
        }

        UE_LOG( LogMapCheckValidation, Log, TEXT( "Load %i streaming levels for world %s" ), streaming_levels.Num(), *world->GetName() );

        world->FlushLevelStreaming( EFlushLevelStreamingType::Full );

        if ( const auto * settings = GetDefault< UMapCheckSettings >() )
        {
            for ( const auto & validator_class_soft_ptr : settings->Validators )
            {
                if ( auto * validator_class = validator_class_soft_ptr.LoadSynchronous() )
                {
                    auto * validator = world->SpawnActor( validator_class );

                    if ( validator == nullptr )
                    {
                        UE_LOG( LogMapCheckValidation, Warning, TEXT( "Impossible to spawn the map check validator of type %s" ), *validator_class->GetName() );
                    }
                    else
                    {
                        UE_LOG( LogMapCheckValidation, Log, TEXT( "Spawned the map check validator of type %s" ), *validator_class->GetName() );
                    }
                }
            }
        }

        GEditor->HandleMapCommand( TEXT( "CHECK" ), *GLog, world );

        world->RemoveFromRoot();

        world_context.SetCurrentWorld( nullptr );
        GWorld = nullptr;

        UE_LOG( LogMapCheckValidation, Log, TEXT( "Finished processing of %s" ), *package_name );
    }

    UE_LOG( LogMapCheckValidation, Log, TEXT( "Successfully finished running MapCheckValidation Commandlet" ) );
    UE_LOG( LogMapCheckValidation, Log, TEXT( "--------------------------------------------------------------------------------------------" ) );
    return 0;
}
