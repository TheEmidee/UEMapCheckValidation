#pragma once

#include <CoreMinimal.h>
#include <WorldPartition/WorldPartitionBuilder.h>

#include "WorldPartitionMapCheckBuilder.generated.h"

UCLASS()
class MAPCHECKVALIDATION_API UWorldPartitionMapCheckBuilder final : public UWorldPartitionBuilder
{
    GENERATED_BODY()

public:
    bool RequiresCommandletRendering() const override;
    ELoadingMode GetLoadingMode() const override;

protected:
    bool RunInternal( UWorld * world, const FCellInfo & cell_info, FPackageSourceControlHelper & package_helper ) override;
};