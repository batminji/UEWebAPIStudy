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


};

UCLASS()
class UEWEBAPISTUDY_API URankingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
};
