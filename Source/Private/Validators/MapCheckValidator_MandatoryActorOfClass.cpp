#include "Validators/MapCheckValidator_MandatoryActorOfClass.h"

#include <Kismet/GameplayStatics.h>
#include <Logging/MessageLog.h>
#include <Misc/UObjectToken.h>

#if WITH_EDITOR

void AMapCheckValidator_MandatoryActorOfClass::CheckForErrors()
{
    Super::CheckForErrors();

    FMessageLog MapCheck( "MapCheck" );

    for ( const auto & mandatory_class : MandatoryActorClasses )
    {
        if ( UGameplayStatics::GetActorOfClass( GetWorld(), mandatory_class ) == nullptr )
        {
            MapCheck.Error()
                ->AddToken( FUObjectToken::Create( this ) )
                ->AddToken( FTextToken::Create( FText::FromString( FString::Printf( TEXT( "No actor of class %s was found in the map %s." ), *GetNameSafe( mandatory_class ), *GetNameSafe( GetWorld() ) ) ) ) );
        }
    }
}

#endif