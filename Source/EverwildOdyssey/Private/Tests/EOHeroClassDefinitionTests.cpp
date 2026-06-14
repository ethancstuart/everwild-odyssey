#if WITH_DEV_AUTOMATION_TESTS

#include "Core/EOHeroClassDefinition.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOHeroClassCatalogTest, "EverwildOdyssey.Core.HeroClassCatalog", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEOHeroClassCatalogTest::RunTest(const FString& Parameters)
{
    const TArray<FEOHeroClassDefinition> Classes = FEOWildClassCatalog::BuildStartingClasses();
    TestEqual(TEXT("The alpha exposes four starting classes."), Classes.Num(), 4);

    TSet<EEOHeroClass> UniqueClasses;
    for (const FEOHeroClassDefinition& Definition : Classes)
    {
        TestTrue(TEXT("Class definition is alpha-valid."), Definition.IsValidForAlpha());
        UniqueClasses.Add(Definition.HeroClass);
    }

    TestTrue(TEXT("Runeblade exists."), UniqueClasses.Contains(EEOHeroClass::Runeblade));
    TestTrue(TEXT("Starshaper exists."), UniqueClasses.Contains(EEOHeroClass::Starshaper));
    TestTrue(TEXT("Pathwarden exists."), UniqueClasses.Contains(EEOHeroClass::Pathwarden));
    TestTrue(TEXT("Lightbound Vanguard exists."), UniqueClasses.Contains(EEOHeroClass::LightboundVanguard));

    FEOHeroClassDefinition Vanguard;
    TestTrue(TEXT("Can look up Lightbound Vanguard."), FEOWildClassCatalog::TryGetClassDefinition(EEOHeroClass::LightboundVanguard, Vanguard));
    TestEqual(TEXT("Vanguard role is Defender."), static_cast<uint8>(Vanguard.PrimaryRole), static_cast<uint8>(EEOPartyRole::Defender));

    return true;
}

#endif
