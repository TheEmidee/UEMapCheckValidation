#include "WorldPartitionMapCheckBuilder.h"

bool UWorldPartitionMapCheckBuilder::RequiresCommandletRendering() const
{
    return false;
}

UWorldPartitionBuilder::ELoadingMode UWorldPartitionMapCheckBuilder::GetLoadingMode() const
{
    return UWorldPartitionBuilder::ELoadingMode::EntireWorld;
}

bool UWorldPartitionMapCheckBuilder::RunInternal( UWorld * world, const FCellInfo & cell_info, FPackageSourceControlHelper & package_helper )
{
    return GEditor->HandleMapCommand( TEXT( "CHECK" ), *GLog, world );
}
