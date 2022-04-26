#pragma once

#include "MapCheckValidatorBase.h"

#include <CoreMinimal.h>

#include "MapCheckValidator_NoActorOfClass.generated.h"

UCLASS()
class MAPCHECKVALIDATION_API AMapCheckValidator_NoActorOfClass final : public AMapCheckValidatorBase
{
    GENERATED_BODY()

public:
#if WITH_EDITOR
    void CheckForErrors() override;
#endif

private:
    UPROPERTY( EditAnywhere, meta = ( AllowAbstract = "true" ) )
    TArray< TSubclassOf< AActor > > ForbiddenActorClasses;
};
