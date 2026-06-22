// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Http.h"
#include "RankingSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FGameResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Result")
	int32 Id = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Result")
	FString UserId = "";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Result")
	FString UserName = "";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Result")
	FString UserEmail = "";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Result")
	int32 Score = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Result")
	FString Date = "";
};

// Add, Update, Delete 결과용 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionSuccessSignature, bool, bSuccess);

// 전체 랭킹 조회 결과용 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGetRankingListSignature, const TArray<FGameResult>&, RankingList);

// 단일 랭킹 조회 결과용 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGetSingleRankingSignature, bool, bSuccess, const FGameResult&, GameResult);

UCLASS(BlueprintType)
class UEWEBAPISTUDY_API URankingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Ranking")
	void AddGameResult(FGameResult NewResult);

	UFUNCTION(BlueprintCallable, Category = "Ranking")
	void GetGameResults();

	UFUNCTION(BlueprintCallable, Category = "Ranking")
	void GetGameResultById(int32 TargetId);

	UFUNCTION(BlueprintCallable, Category = "Ranking")
	void UpdateGameResult(FGameResult UpdatedResult);

	UFUNCTION(BlueprintCallable, Category = "Ranking")
	void DeleteGameResult(int32 TargetId);

	// Delegates
	UPROPERTY(BlueprintAssignable, Category = "Ranking|Delegates")
	FOnActionSuccessSignature OnActionCompleted;

	UPROPERTY(BlueprintAssignable, Category = "Ranking|Delegates")
	FOnGetRankingListSignature OnGetRankingListCompleted;

	UPROPERTY(BlueprintAssignable, Category = "Ranking|Delegates")
	FOnGetSingleRankingSignature OnGetSingleRankingCompleted;

private:
	// ASP.NET WebAPI 주소
	FString BaseUrl = TEXT("http://localhost:9090/api/Ranking");

private:
	// Post
	void OnAddGameResultResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	// Get
	void OnGetGameResultsResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void OnGetGameResultByIdResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	// Put
	void OnUpdateGameResultResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	// Delete
	void OnDeleteGameResultResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
