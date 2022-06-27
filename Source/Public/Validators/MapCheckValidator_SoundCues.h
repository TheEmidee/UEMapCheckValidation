#pragma once

#include "MapCheckValidatorBase.h"

#include "MapCheckValidator_SoundCues.generated.h"

UCLASS()
class MAPCHECKVALIDATION_API AMapCheckValidator_SoundCues final : public AMapCheckValidatorBase
{
    GENERATED_BODY()

public:
#if WITH_EDITOR
    void CheckForErrors() override;
#endif
};
