// Fill out your copyright notice in the Description page of Project Settings.


#include "DataRetrevier.h"

DataRetrevier::DataRetrevier()
{
	const FString ServerURL = TEXT("ws:api.weather.gov/points/39.7456,-97.0892"); // Your server URL. You can use ws, wss or wss+insecure.
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
		data = (TEXT("Sent message: %s"), *response);
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

void DataRetrevier::update(const FString& pathing)
{
	Socket->Send(*pathing);
}

const FString& DataRetrevier::getData()
{
	return data;
}
