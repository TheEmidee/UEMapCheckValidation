#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>

#include "MapCheckValidatorBase.generated.h"

UCLASS( Abstract )
class MAPCHECKVALIDATION_API AMapCheckValidatorBase : public AActor
{
    GENERATED_BODY()

public:
    AMapCheckValidatorBase();

#if WITH_EDITOR
    void CheckForErrors() override;
#endif
};
