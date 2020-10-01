#pragma once

#include <Commandlets/Commandlet.h>

#include "MapCheckValidationCommandlet.generated.h"

UCLASS( CustomConstructor )
class MAPCHECKVALIDATION_API UMapCheckValidationCommandlet final : public UCommandlet
{
    GENERATED_BODY()

public:

    UMapCheckValidationCommandlet();

    int32 Main( const FString & params ) override;
};
