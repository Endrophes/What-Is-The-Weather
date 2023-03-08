// Copyright 2023 - Bastien A. Auxer - All Rights Reserved

#pragma once

#include "WebSocketsModule.h" // Module definition
#include "IWebSocket.h" // Socket definition

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

	//
	void query(const FString& query);

	//
	template<typename FunctorType, typename... VarTypes>
	void addCallback(FunctorType&& InFunctor, VarTypes... Vars)
	{
		Socket->OnMessage().AddLambda(InFunctor, Vars...);
	}
};
