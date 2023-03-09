// Copyright 2023 - Bastien A. Auxer - All Rights Reserved

#pragma once

#include "DataRetrevier.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/TextRenderActor.h"

#include "GeneralDataDisplay.generated.h"

UENUM()
enum class EDataTargets : uint8
{
	local  UMETA(DisplayName = "Local"),
	temp   UMETA(DisplayName = "Temp"),
	detail UMETA(DisplayName = "Detail")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WHATISTHEWEATHER_API UGeneralDataDisplay : public UActorComponent
{
	GENERATED_BODY()

public:

	// Sets default values for this component's properties
	UGeneralDataDisplay();
	UTextRenderComponent* TextRenderComponent;
	float dTime;

	UPROPERTY(EditAnywhere, Meta = (Bitmask, BitmaskEnum = "EDataTargets"))
	EDataTargets targetProp;

	UPROPERTY(EditAnywhere, Meta = (Bitmask))
	bool bAdjustSpan = true;

	UPROPERTY(EditAnywhere, Meta = (Bitmask))
	uint8 segmentMaxLength = 35;

	//
	static FString EnumToQuery(EDataTargets Enum);

private:
	DataRetrevier dataRetrevier;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called when the game ends
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
