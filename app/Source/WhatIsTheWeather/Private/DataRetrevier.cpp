// Copyright 2023 - Bastien A. Auxer - All Rights Reserved

#include "DataRetrevier.h"

DataRetrevier::DataRetrevier()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;
	UE_LOG(LogTemp, Warning, TEXT("UWeatherDisplay started running"));
	if (!FModuleManager::Get().IsModuleLoaded("WebSockets"))
	{
		UE_LOG(LogTemp, Warning, TEXT("LOAD THE F**ing MODULE"));
		FModuleManager::Get().LoadModule("WebSockets");
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("WebSockets module loaded"));
	}
	// ...

	const FString ServerURL = TEXT("ws://localhost:5000/"); // Your server URL. You can use ws, wss or wss+insecure.
	const FString ServerProtocol = TEXT("ws"); // The WebServer protocol you want to use.
	Socket = FWebSocketsModule::Get().CreateWebSocket(ServerURL, ServerProtocol);

	// We bind all available events
	Socket->OnConnected().AddLambda([]() -> void {
		UE_LOG(LogTemp, Warning, TEXT("Connection Established"));
	});

	Socket->OnConnectionError().AddLambda([](const FString & err) -> void {
		UE_LOG(LogTemp, Error, TEXT("Connection Failed: %s"), *err);
	});

	Socket->OnMessage().AddLambda([&](const FString response) -> void {
		UE_LOG(LogTemp, Warning, TEXT("Response from Server: %s"), *response);
		data = (response);
	});

	Socket->OnMessageSent().AddLambda([](const FString& MessageString) -> void {
		UE_LOG(LogTemp, Warning, TEXT("Sent message: %s"), *MessageString);
	});

	Socket->Connect();
}

DataRetrevier::~DataRetrevier()
{
	Socket->Close();
}

void DataRetrevier::query(const FString& pathing)
{
	Socket->Send(*pathing);
}

const FString& DataRetrevier::getData()
{
	return data;
}
