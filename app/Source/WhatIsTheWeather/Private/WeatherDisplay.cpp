// Copyright 2023 - Bastien A. Auxer - All Rights Reserved

#include "WeatherDisplay.h"
#include "Modules/ModuleManager.h"

// Sets default values for this component's properties
UWeatherDisplay::UWeatherDisplay()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame. You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

//Called whenever this actor is being removed from a level
void UWeatherDisplay::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

}

// Called when the game starts
void UWeatherDisplay::BeginPlay()
{
	Super::BeginPlay();

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

	dataRetrevier.addCallback([&](const FString response) -> void {
		UE_LOG(LogTemp, Warning, TEXT("Response from Server: %s"), *response);
		FString display = (TEXT("Sent message: %s"), *response);
		TextRenderComponent->SetText(response);
	});
}


// Called every frame
void UWeatherDisplay::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (dTime >= 60) {
		dTime = 0;
	}
	if (dTime == 0) {
		dataRetrevier.query(TEXT("get-temp"));
	}
	dTime += DeltaTime;
}

