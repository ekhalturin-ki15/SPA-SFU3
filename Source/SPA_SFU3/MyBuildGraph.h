// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyBuildGraph.generated.h"

USTRUCT(BlueprintType)
struct FLableGraph : public FTableRowBase
{
    GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString sLabel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float fWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString sTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int iCourse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString sComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int iCountComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float fNormWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int X;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int Y;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int Z;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor Color;
};


USTRUCT(BlueprintType)
struct FRibGraph : public FTableRowBase
{
    GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int L;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int R;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString sType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString sLabel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float fWeight;
};

UCLASS(Blueprintable, ClassGroup="Custom",meta=(BlueprintSpawnableComponent))
class SPA_SFU3_API UMyBuildGraph : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyBuildGraph();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "MyArray")
    TArray<FLableGraph> arrLableGraph;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "MyArray")
    TArray<FRibGraph> arrRibGraph;
   
	UFUNCTION(BlueprintImplementableEvent, Category = "Function")
    void AfterBeginPlay();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


private:
    TMap<int, FLableGraph> mapLableGraph;
    TMap<int, FRibGraph>   mapRibGraph;

    //public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
