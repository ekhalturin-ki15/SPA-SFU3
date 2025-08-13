// Fill out your copyright notice in the Description page of Project Settings.
// v 1.00

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase.generated.h"

class UMyBuildGraph;
    /**
 * 
 */
UCLASS()
class SPA_SFU3_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	AMyGameModeBase();

	virtual void BeginPlay() override;

    FString sStringParse;

	FColor StringToColor(FString InString);

	bool LoadFileCP1251(FString FilePath, FString& OutString);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
    TSubclassOf<UMyBuildGraph> ptrBuildGraph;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UMyBuildGraph* fBuildGraph;
};
