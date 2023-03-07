// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WebSocketsModule.h" // Module definition
#include "IWebSocket.h" // Socket definition
#include "Engine/TextRenderActor.h"
#include "Components/TextRenderComponent.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LocationDisplay.generated.h"

/**
 * Created and maintains a websocket to the source.
 */
class WHATISTHEWEATHER_API DataRetrevier
{
private:
	TSharedPtr<IWebSocket> Socket;
	float dTime;
	FString data;

public:
	DataRetrevier();
	~DataRetrevier();

	//Sends out a call to get updated info
	void update(const FString& pathing);

	//After calling to upodate the data, it will path through and find the string value of what to display
	const FString& getData();
};
