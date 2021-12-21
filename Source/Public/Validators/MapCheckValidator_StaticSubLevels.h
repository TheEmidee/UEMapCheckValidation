#pragma once

#include <MapCheckValidatorBase.h>

#include <CoreMinimal.h>

#include "MapCheckValidator_StaticSubLevels.generated.h"

/*
 * This validator checks that sub-levels ending with a defined suffix are set to static.
 * It also checks that all primitive components of the actors of those sub-levels have their mobility set to Static too
 */
UCLASS()
class MAPCHECKVALIDATION_API AMapCheckValidator_StaticSubLevels final : public AMapCheckValidatorBase
{
    GENERATED_BODY()

public:

    AMapCheckValidator_StaticSubLevels();

#if WITH_EDITOR
    void CheckForErrors() override;
#endif

private:

    /*
     * This will only check the sublevels which end with that value
     */
    UPROPERTY( EditAnywhere )
    FString MapSuffix;

    UPROPERTY( EditAnywhere )
    TArray< TSubclassOf< UPrimitiveComponent > > PrimitiveComponentClassesToIgnore;
};
