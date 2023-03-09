// Copyright 2023 - Bastien A. Auxer - All Rights Reserved


#include "GeneralDataDisplay.h"

UGeneralDataDisplay::UGeneralDataDisplay()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UGeneralDataDisplay::BeginPlay()
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
		TextRenderComponent->SetText(response);
	});
	
}

void UGeneralDataDisplay::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

}

FString UGeneralDataDisplay::EnumToQuery(EDataTargets Enum)
{
	switch (Enum)
	{
	case EDataTargets::local:
		return TEXT("get-local");
	case EDataTargets::temp:
		return TEXT("get-temp");
	case EDataTargets::detail:
		return TEXT("get-detail");
	default:
		return TEXT("Invalid");
	}
}

void UGeneralDataDisplay::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (dTime >= 60) {
		dTime = 0;
	}
	if (dTime == 0) {
		dataRetrevier.query(EnumToQuery(targetProp));
	}
	dTime += DeltaTime;
}

