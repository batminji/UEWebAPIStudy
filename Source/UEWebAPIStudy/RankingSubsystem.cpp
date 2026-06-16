// Fill out your copyright notice in the Description page of Project Settings.


#include "RankingSubsystem.h"
#include "Dom/JsonObject.h"						// Json 객체를 다루기 위한 헤더
#include "Dom/JsonValue.h"						// Json 값들을 다루기 위한 헤더
#include "Serialization/JsonWriter.h"			// Json 문자열을 생성하기 위한 헤더
#include "Serialization/JsonReader.h"			// Json 문자열을 읽기 위한 헤더
#include "Serialization/JsonSerializer.h"		// Json 직렬화와 역직렬화를 위한 헤더

static void JsonObjectToGameResult(const TSharedPtr<FJsonObject>& JsonObject, FGameResult& OutGameResult)
{
	if(!JsonObject.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("JsonObjectToGameResult: JsonObject is invalid."));
		return;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("JsonObjectToGameResult: JsonObject is valid."));
	}

	double TempId = -1.0f;
	if (JsonObject->TryGetNumberField(TEXT("Id"), TempId) || JsonObject->TryGetNumberField(TEXT("id"), TempId))
	{
		OutGameResult.Id = static_cast<int32>(TempId);
	}
	if (!JsonObject->TryGetStringField(TEXT("UserId"), OutGameResult.UserId))
	{
		JsonObject->TryGetStringField(TEXT("userId"), OutGameResult.UserId);
	}
	if (!JsonObject->TryGetStringField(TEXT("UserName"), OutGameResult.UserName))
	{
		JsonObject->TryGetStringField(TEXT("userName"), OutGameResult.UserName)	;
	}
	if(!JsonObject->TryGetStringField(TEXT("UserEmail"), OutGameResult.UserEmail))
	{
		JsonObject->TryGetStringField(TEXT("userEmail"), OutGameResult.UserEmail);
	}
	double TempScore = -1.0f;
	if (JsonObject->TryGetNumberField(TEXT("Score"), TempScore) || JsonObject->TryGetNumberField(TEXT("score"), TempScore))
	{
		OutGameResult.Score = static_cast<int32>(TempScore);
	}
	if(!JsonObject->TryGetStringField(TEXT("Date"), OutGameResult.Date))
	{
		JsonObject->TryGetStringField(TEXT("date"), OutGameResult.Date);
	}
}

void URankingSubsystem::AddGameResult(const FGameResult& NewResult)
{

}

void URankingSubsystem::GetGameResults()
{
}

void URankingSubsystem::GetGameResultById(int32 TargetId)
{
}

void URankingSubsystem::UpdateGameResult(int32 TargetId, const FGameResult& UpdatedResult)
{
}

void URankingSubsystem::DeleteGameResult(int32 TargetId)
{
}

FString URankingSubsystem::ConvertGameResultToJson(const FGameResult& GameResult)
{
	return FString();
}

void URankingSubsystem::OnAddGameResultResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
}

void URankingSubsystem::OnGetGameResultsResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
}

void URankingSubsystem::OnGetGameResultByIdResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
}

void URankingSubsystem::OnUpdateGameResultResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
}

void URankingSubsystem::OnDeleteGameResultResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
}
