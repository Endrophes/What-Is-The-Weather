// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WebSocketsModule.h" // Module definition
#include "IWebSocket.h" // Socket definition
#include "Engine/TextRenderActor.h"
#include "Components/TextRenderComponent.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DetailsDisplay.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WHATISTHEWEATHER_API UDetailsDisplay : public UActorComponent
{
	GENERATED_BODY()

public:
	UDetailsDisplay();
	TSharedPtr<IWebSocket> Socket;
	UTextRenderComponent* TextRenderComponent;
	float dTime;

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
