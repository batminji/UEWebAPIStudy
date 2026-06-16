// Fill out your copyright notice in the Description page of Project Settings.


#include "RankingSubsystem.h"
#include "Dom/JsonObject.h"						// Json 객체를 다루기 위한 헤더
#include "Dom/JsonValue.h"						// Json 값들을 다루기 위한 헤더
#include "Serialization/JsonWriter.h"			// Json 문자열을 생성하기 위한 헤더
#include "Serialization/JsonReader.h"			// Json 문자열을 읽기 위한 헤더
#include "Serialization/JsonSerializer.h"		// Json 직렬화와 역직렬화를 위한 헤더

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
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	JsonObject->SetNumberField(TEXT("Id"), GameResult.Id);
	JsonObject->SetStringField(TEXT("UserId"), GameResult.UserId);
	JsonObject->SetStringField(TEXT("UserName"), GameResult.UserName);
	JsonObject->SetStringField(TEXT("UserEmail"), GameResult.UserEmail);
	JsonObject->SetNumberField(TEXT("Score"), GameResult.Score);
	JsonObject->SetStringField(TEXT("FDate"), GameResult.FDate);

	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	return OutputString;
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
