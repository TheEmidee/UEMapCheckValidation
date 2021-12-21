#pragma once

#include "MapCheckValidatorBase.h"

#include <CoreMinimal.h>

#include "MapCheckValidator_PhysicalMaterials.generated.h"

/*
 * This validator will check that all the primitive components of all the actors in the scene that have a material assigned also have a physical material
 */
UCLASS()
class MAPCHECKVALIDATION_API AMapCheckValidator_PhysicalMaterials final : public AMapCheckValidatorBase
{
    GENERATED_BODY()

public:
#if WITH_EDITOR
    void CheckForErrors() override;
#endif
};
