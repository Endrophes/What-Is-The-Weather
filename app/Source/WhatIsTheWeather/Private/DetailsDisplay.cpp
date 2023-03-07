// Fill out your copyright notice in the Description page of Project Settings.


#include "DetailsDisplay.h"

// Sets default values for this component's properties
UDetailsDisplay::UDetailsDisplay()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	UE_LOG(LogTemp, Warning, TEXT("UDetailsDisplay started running"));

	// ...
}

//Called whenever this actor is being removed from a level
void UDetailsDisplay::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Socket->Close();
}

// Called when the game starts
void UDetailsDisplay::BeginPlay()
{
	Super::BeginPlay();

	const FString ServerURL = TEXT("ws://localhost:5000/"); // Your server URL. You can use ws, wss or wss+insecure.
	const FString ServerProtocol = TEXT("ws");              // The WebServer protocol you want to use.

	ATextRenderActor* TextRenderActor = Cast<ATextRenderActor>(GetOwner());
	if (TextRenderActor)
	{
		TextRenderComponent = TextRenderActor->GetTextRender();
		if (!TextRenderComponent) {
			//failure handling (TextRenderActor didn't have a valid/non-NULL TextRenderComponent)
			UE_LOG(LogTemp, Error, TEXT("Can't get UTextRenderComponent"));
		}
	}
	else {
		//failure handling (cast failed)
		UE_LOG(LogTemp, Error, TEXT("Can't get ATextRenderActor"));
	}

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
		FString display = (TEXT("Sent message: %s"), *response);
		TArray<FString> splitResponse;
		response.ParseIntoArray(splitResponse, TEXT(" "), true);
		FString rebuiltResponse = "";
		int sentenceLength = 0;
		int segmentMaxLength = 35;
		for (int i = 0; i < splitResponse.Num(); i++) {
			FString word = splitResponse[i];
			if ((sentenceLength + word.Len()) > segmentMaxLength) {
				rebuiltResponse.Append("\n " + word);
				sentenceLength = 0;
			} else {
				rebuiltResponse.Append(" " + word);
				sentenceLength += word.Len();
			}
		}
		TextRenderComponent->SetText(rebuiltResponse);
	});

	Socket->OnMessageSent().AddLambda([](const FString& MessageString) -> void {
		UE_LOG(LogTemp, Warning, TEXT("Sent message: %s"), *MessageString);
	});

	Socket->Connect();
	
}


// Called every frame
void UDetailsDisplay::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (dTime >= 60) {
		dTime = 0;
	}
	if (dTime == 0) {
		Socket->Send(TEXT("get-detail"));
	}
	dTime += DeltaTime;
}

