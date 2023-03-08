// Copyright 2023 - Bastien A. Auxer - All Rights Reserved

#pragma once

#include "DataRetrevier.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/TextRenderActor.h"

#include "LocationDisplay.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WHATISTHEWEATHER_API ULocationDisplay : public UActorComponent
{
	GENERATED_BODY()

public:
	ULocationDisplay();
	UTextRenderComponent* TextRenderComponent;
	float dTime;

private:
	DataRetrevier dataRetrevier;

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
