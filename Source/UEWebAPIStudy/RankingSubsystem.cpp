// Fill out your copyright notice in the Description page of Project Settings.


#include "RankingSubsystem.h"
#include "Dom/JsonObject.h"						// Json 객체를 다루기 위한 헤더
#include "Dom/JsonValue.h"						// Json 값들을 다루기 위한 헤더
#include "Serialization/JsonWriter.h"			// Json 문자열을 생성하기 위한 헤더
#include "Serialization/JsonReader.h"			// Json 문자열을 읽기 위한 헤더
#include "Serialization/JsonSerializer.h"		// Json 직렬화와 역직렬화를 위한 헤더

// FJsonObject를 FGameResult 구조체로 변환하는 헬퍼 함수
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

// FGameResult 구조체를 Json 문자열로 변환하는 헬퍼 함수
static FString MakeJsonFromGameResult(const FGameResult& GameResult)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	JsonObject->SetNumberField(TEXT("Id"), GameResult.Id);
	JsonObject->SetStringField(TEXT("UserId"), GameResult.UserId);
	JsonObject->SetStringField(TEXT("UserName"), GameResult.UserName);
	JsonObject->SetStringField(TEXT("UserEmail"), GameResult.UserEmail);
	JsonObject->SetNumberField(TEXT("Score"), GameResult.Score);
	JsonObject->SetStringField(TEXT("Date"), GameResult.Date);

	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	return OutputString;
}

void URankingSubsystem::AddGameResult(FGameResult NewResult)
{
	if (NewResult.Date.IsEmpty())
	{
		NewResult.Date = FDateTime::UtcNow().ToIso8601();
	}

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();

	HttpRequest->SetURL(BaseUrl);																				// 연결할 Endpoint URL 설정

	HttpRequest->SetVerb(TEXT("POST"));																			// HTTP 메서드 설정 (POST, GET, PUT, DELETE 등)

	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));										// 서버에 Json 형식의 데이터를 보낼 것임을 알리는 헤더 설정

	HttpRequest->SetContentAsString(MakeJsonFromGameResult(NewResult));										// 요청 본문 설정 (JSON 문자열)

	HttpRequest->OnProcessRequestComplete().BindUObject(this, &URankingSubsystem::OnAddGameResultResponse);		// 요청이 완료되었을 때 호출될 콜백 함수 바인딩

	HttpRequest->ProcessRequest();																				// 요청 전송
}

void URankingSubsystem::GetGameResults()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();

	HttpRequest->SetURL(BaseUrl);																				// 연결할 Endpoint URL 설정

	HttpRequest->SetVerb(TEXT("GET"));																			// HTTP 메서드 설정 (POST, GET, PUT, DELETE 등)

	HttpRequest->OnProcessRequestComplete().BindUObject(this, &URankingSubsystem::OnGetGameResultsResponse);	// 요청이 완료되었을 때 호출될 콜백 함수 바인딩

	HttpRequest->ProcessRequest();																				// 요청 전송
}

void URankingSubsystem::GetGameResultById(int32 TargetId)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();

	HttpRequest->SetURL(BaseUrl + FString::Printf(TEXT("/%d"), TargetId));										// 연결할 Endpoint URL 설정 (ID 포함)
	
	HttpRequest->SetVerb(TEXT("GET"));																			// HTTP 메서드 설정 (POST, GET, PUT, DELETE 등)
	
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &URankingSubsystem::OnGetGameResultByIdResponse);	// 요청이 완료되었을 때 호출될 콜백 함수 바인딩
	
	HttpRequest->ProcessRequest();																				// 요청 전송
}

void URankingSubsystem::UpdateGameResult(FGameResult UpdatedResult)
{
	if (UpdatedResult.Date.IsEmpty())
	{
		UpdatedResult.Date = FDateTime::UtcNow().ToIso8601();
	}

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();

	HttpRequest->SetURL(BaseUrl + FString::Printf(TEXT("/%d"), UpdatedResult.Id));								// 연결할 Endpoint URL 설정 (ID 포함)

	HttpRequest->SetVerb(TEXT("PUT"));																			// HTTP 메서드 설정 (POST, GET, PUT, DELETE 등)

	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));										// 서버에 Json 형식의 데이터를 보낼 것임을 알리는 헤더 설정

	HttpRequest->SetContentAsString(MakeJsonFromGameResult(UpdatedResult));										// 요청 본문 설정 (JSON 문자열)

	HttpRequest->OnProcessRequestComplete().BindUObject(this, &URankingSubsystem::OnUpdateGameResultResponse);	// 요청이 완료되었을 때 호출될 콜백 함수 바인딩

	HttpRequest->ProcessRequest();																				// 요청 전송
}

void URankingSubsystem::DeleteGameResult(int32 TargetId)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	
	HttpRequest->SetURL(BaseUrl + FString::Printf(TEXT("/%d"), TargetId));										// 연결할 Endpoint URL 설정 (ID 포함)
	
	HttpRequest->SetVerb(TEXT("DELETE"));																		// HTTP 메서드 설정 (POST, GET, PUT, DELETE 등)
	
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &URankingSubsystem::OnDeleteGameResultResponse);	// 요청이 완료되었을 때 호출될 콜백 함수 바인딩
	
	HttpRequest->ProcessRequest();																				// 요청 전송
}

void URankingSubsystem::OnAddGameResultResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if(!bWasSuccessful || !Response.IsValid() || Response->GetResponseCode() != 200)
	{
		OnActionCompleted.Broadcast(false);
		return;
	}

	FString ResponseContent = Response->GetContentAsString();

	bool bSuccess = ResponseContent.Equals(TEXT("true"), ESearchCase::IgnoreCase);			// 서버에서 "true"라는 문자열이 오면 성공으로 간주

	OnActionCompleted.Broadcast(bSuccess);													// Http 통신 성공
}

void URankingSubsystem::OnGetGameResultsResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TArray<FGameResult> RankingList;
	if (!bWasSuccessful || !Response.IsValid() || Response->GetResponseCode() != 200)
	{
		OnGetRankingListCompleted.Broadcast(RankingList);
		return;
	}

	FString ResponseContent = Response->GetContentAsString();

	TArray<TSharedPtr<FJsonValue>> JsonArray;

	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseContent);

	bool bParseSuccess = FJsonSerializer::Deserialize(Reader, JsonArray);

	if (!bParseSuccess)
	{
		OnGetRankingListCompleted.Broadcast(RankingList);
		return;
	}

	for(const TSharedPtr<FJsonValue>& JsonValue : JsonArray)
	{
		TSharedPtr<FJsonObject> JsonObject = JsonValue->AsObject();
		if(JsonObject.IsValid())
		{
			FGameResult GameResult;
			JsonObjectToGameResult(JsonObject, GameResult);
			RankingList.Add(GameResult);
		}
	}

	OnGetRankingListCompleted.Broadcast(RankingList);
}

void URankingSubsystem::OnGetGameResultByIdResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid() || Response->GetResponseCode() != 200)
	{
		OnGetSingleRankingCompleted.Broadcast(false, FGameResult());
		return;
	}

	FString ResponseContent = Response->GetContentAsString();

	if(ResponseContent.IsEmpty() || ResponseContent.Equals(TEXT("null"), ESearchCase::IgnoreCase))
	{
		OnGetSingleRankingCompleted.Broadcast(false, FGameResult());
		return;
	}

	TSharedPtr<FJsonObject> JsonObject;

	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseContent);

	bool bParseSuccess = FJsonSerializer::Deserialize(Reader, JsonObject);

	if (!bParseSuccess || !JsonObject.IsValid())
	{
		OnGetSingleRankingCompleted.Broadcast(false, FGameResult());
		return;
	}

	FGameResult GameResult;

	JsonObjectToGameResult(JsonObject, GameResult);

	OnGetSingleRankingCompleted.Broadcast(true, GameResult);
}

void URankingSubsystem::OnUpdateGameResultResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid() || Response->GetResponseCode() != 200)
	{
		OnActionCompleted.Broadcast(false);
		return;
	}

	FString ResponseContent = Response->GetContentAsString();

	bool bSuccess = ResponseContent.Equals(TEXT("true"), ESearchCase::IgnoreCase);			// 서버에서 "true"라는 문자열이 오면 성공으로 간주

	OnActionCompleted.Broadcast(bSuccess);													// Http 통신 성공
}

void URankingSubsystem::OnDeleteGameResultResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid() || Response->GetResponseCode() != 200)
	{
		OnActionCompleted.Broadcast(false);
		return;
	}
	FString ResponseContent = Response->GetContentAsString();

	bool bSuccess = ResponseContent.Equals(TEXT("true"), ESearchCase::IgnoreCase);			// 서버에서 "true"라는 문자열이 오면 성공으로 간주
	
	OnActionCompleted.Broadcast(bSuccess);													// Http 통신 성공
}
