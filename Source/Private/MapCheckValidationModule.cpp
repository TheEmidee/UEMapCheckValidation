#include "MapCheckValidationModule.h"

class FMapCheckValidationModule final : public IMapCheckValidationModule
{
public:
    void StartupModule() override;
    void ShutdownModule() override;
};

IMPLEMENT_MODULE( FMapCheckValidationModule, MapCheckValidation )

void FMapCheckValidationModule::StartupModule()
{
}

void FMapCheckValidationModule::ShutdownModule()
{
}