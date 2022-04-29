
#include "MissionFrameworkEditorPrivatePCH.h"
#include "Rewards/MissionRewardPropertyMutate.h"
#include "Editor/UnrealEd/Public/Tests/AutomationEditorCommon.h"
#include "AutomationTest.h"
#include "Engine/World.h"
#include "Json.h"
#include "JsonUtilities.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPropertyRewardMutatorTests, "Plugins.MissionFramework.FPropertyRewardMutator", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::EditorContext)

bool FPropertyRewardMutatorTests::RunTest(const FString& Parameters)
{
	//TODO: Perhaps load a different map or something like that? This works tho, so that is pretty neat
	if (UWorld* TestWorld = AutomationEditorCommonUtils::CreateNewMap())
	{
		//TEST: Operation Codes
		{
			if (AActor* TestActor = TestWorld->SpawnActor<AActor>(AActor::StaticClass()))
			{
				FMissionRewardPropertyMutateConfig Configuration;
				Configuration.PropertyPath = FMissionDynamicPropertyPath("CustomTimeDilation");
				Configuration.MutationOperation = EMissionRewardPropertyMutateOp::Set;
				Configuration.ValueToApplyAsString = "56.78";

				FPropertyRewardMutator::Mutate(TestActor, Configuration);
				TestEqual(TEXT("Should set Custom Time Dilation to 56.78"), TestActor->CustomTimeDilation, 56.78f);

				TestActor->Destroy();
			}
			else
			{
				return false;
			}
		}

		{
			if (AActor* TestActor = TestWorld->SpawnActor<AActor>(AActor::StaticClass()))
			{
				FMissionRewardPropertyMutateConfig Configuration;
				Configuration.PropertyPath = FMissionDynamicPropertyPath("CustomTimeDilation");
				Configuration.MutationOperation = EMissionRewardPropertyMutateOp::Add;
				Configuration.ValueToApplyAsString = "100.0f";

				float Expected = TestActor->CustomTimeDilation + 100.0f;

				FPropertyRewardMutator::Mutate(TestActor, Configuration);
				TestEqual(TEXT("Should have done an Add operation on CustomTimeDilation"), TestActor->CustomTimeDilation, Expected);

				TestActor->Destroy();
			}
			else
			{
				return false;
			}
		}

		{
			if (AActor* TestActor = TestWorld->SpawnActor<AActor>(AActor::StaticClass()))
			{
				FMissionRewardPropertyMutateConfig Configuration;
				Configuration.PropertyPath = FMissionDynamicPropertyPath("CustomTimeDilation");
				Configuration.MutationOperation = EMissionRewardPropertyMutateOp::Subtract;
				Configuration.ValueToApplyAsString = "10.0f";

				float Expected = TestActor->CustomTimeDilation - 10.0f;

				FPropertyRewardMutator::Mutate(TestActor, Configuration);
				TestEqual(TEXT("Should have done a Subtract operation on CustomTimeDilation"), TestActor->CustomTimeDilation, Expected);

				TestActor->Destroy();
			}
			else
			{
				return false;
			}
		}

		{
			if (AActor* TestActor = TestWorld->SpawnActor<AActor>(AActor::StaticClass()))
			{
				FMissionRewardPropertyMutateConfig Configuration;
				Configuration.PropertyPath = FMissionDynamicPropertyPath("CustomTimeDilation");
				Configuration.MutationOperation = EMissionRewardPropertyMutateOp::Divide;
				Configuration.ValueToApplyAsString = "7.0f";

				float Expected = TestActor->CustomTimeDilation / 7.0f;

				FPropertyRewardMutator::Mutate(TestActor, Configuration);
				TestEqual(TEXT("Should have done a Divide operation on CustomTimeDilation"), TestActor->CustomTimeDilation, Expected);

				TestActor->Destroy();
			}
			else
			{
				return false;
			}
		}

		{
			if (AActor* TestActor = TestWorld->SpawnActor<AActor>(AActor::StaticClass()))
			{
				FMissionRewardPropertyMutateConfig Configuration;
				Configuration.PropertyPath = FMissionDynamicPropertyPath("CustomTimeDilation");
				Configuration.MutationOperation = EMissionRewardPropertyMutateOp::Multiply;
				Configuration.ValueToApplyAsString = "110.0f";

				float Expected = TestActor->CustomTimeDilation * 110.0f;

				FPropertyRewardMutator::Mutate(TestActor, Configuration);
				TestEqual(TEXT("Should have done a Multiply operation on CustomTimeDilation"), TestActor->CustomTimeDilation, Expected);

				TestActor->Destroy();
			}
			else
			{
				return false;
			}
		}

		//TEST: Class Filter
		{
			if (AActor* TestActor = TestWorld->SpawnActor<AActor>(AActor::StaticClass()))
			{
				FMissionRewardPropertyMutateConfig Configuration;
				Configuration.PropertyPath = FMissionDynamicPropertyPath("CustomTimeDilation");
				Configuration.MutationOperation = EMissionRewardPropertyMutateOp::Set;
				Configuration.ValueToApplyAsString = "123.45";

				FPropertyRewardMutator::Mutate(TestActor, Configuration);
				TestNotEqual(TEXT("Should not have applied change to type of AActor"), TestActor->CustomTimeDilation, 123.45f);

				TestActor->Destroy();
			}
			else
			{
				return false;
			}
		}

		{
			if (ACharacter* TestCharacter = TestWorld->SpawnActor<ACharacter>(ACharacter::StaticClass()))
			{
				FMissionRewardPropertyMutateConfig Configuration;
				Configuration.PropertyPath = FMissionDynamicPropertyPath("CustomTimeDilation");
				Configuration.MutationOperation = EMissionRewardPropertyMutateOp::Set;
				Configuration.ValueToApplyAsString = "125.0";

				FPropertyRewardMutator::Mutate(TestCharacter, Configuration);
				TestEqual(TEXT("Should have applied a Set operation on CustomTimeDilation"), TestCharacter->CustomTimeDilation, 125.0f);

				TestCharacter->Destroy();
			}
			else
			{
				return false;
			}
		}

		//TEST: Reflection Traversing
		{
			if (AActor* TestActor = TestWorld->SpawnActor<AActor>(AActor::StaticClass()))
			{
				FMissionRewardPropertyMutateConfig Configuration;
				Configuration.PropertyPath = FMissionDynamicPropertyPath("TIME_Time");
				Configuration.MutationOperation = EMissionRewardPropertyMutateOp::Set;
				Configuration.ValueToApplyAsString = "Was it not there??!?";

				FString TestStringOne;
				if (FJsonObjectConverter::UStructToJsonObjectString(TestActor->GetClass(),
					TestActor,
					TestStringOne,
					0,
					0,
					0,
					nullptr,
					false))
				{
					FPropertyRewardMutator::Mutate(TestActor, Configuration);

					FString TestStringTwo;
					if (FJsonObjectConverter::UStructToJsonObjectString(TestActor->GetClass(),
						TestActor,
						TestStringTwo,
						0,
						0,
						0,
						nullptr,
						false))
					{
						TestEqual(TEXT("Should not have applied change at all"), TestStringOne, TestStringTwo);
					}
					else
					{
						TestActor->Destroy();
						return false;
					}
				}
				else
				{
					TestActor->Destroy();
					return false;
				}
				TestActor->Destroy();
			}
			else
			{
				return false;
			}
		}

		TestWorld->DestroyWorld(false, nullptr);
		return true;
	}
	else
	{
		return false;
	}
}

#endif