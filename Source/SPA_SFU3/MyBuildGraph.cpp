// Fill out your copyright notice in the Description page of Project Settings.

#include "MyBuildGraph.h"
#include "Misc/Paths.h"
#include "MyGameModeBase.h"

// Sets default values for this component's properties
UMyBuildGraph::UMyBuildGraph()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;

    // ...
}

// Called when the game starts
void UMyBuildGraph::BeginPlay()
{
    Super::BeginPlay();
    AMyGameModeBase* GM = GetWorld()->GetAuthGameMode<AMyGameModeBase>();
    if (!GM) return;


    FString sFilePath = FPaths::ProjectContentDir();
    {
        FString  sFileContent;
        FString  sLablePath   = sFilePath;
        sLablePath += L"/Label.csv";
        if (GM->LoadFileCP1251(sLablePath, sFileContent))
        {
            UE_LOG(LogTemp, Log, TEXT("OK Label.csv"));

            TArray<FString> arrLines;
            sFileContent.ParseIntoArrayLines(arrLines);

            int Y = -1;
            for (const FString& sLine : arrLines)
            {
                ++Y;
                if (Y == 0) continue;    // Не считываем заголовок

                UE_LOG(LogTemp, Log, TEXT("%s"), *sLine);
                UE_LOG(LogTemp, Log, TEXT("%s"), *GM->sStringParse);
                TArray<FString> sCells;
                sLine.ParseIntoArray(sCells, *GM->sStringParse, true);

                int         iNum = -1;
                int         id   = FCString::Atoi(*sCells[++iNum]);

                mapLableGraph.FindOrAdd(id).sLabel = sCells[++iNum];
                mapLableGraph.FindOrAdd(id).fWeight = FCString::Atof(*sCells[++iNum]);
                mapLableGraph.FindOrAdd(id).sTag    = sCells[++iNum];
                mapLableGraph.FindOrAdd(id).iCourse  = FCString::Atoi(*sCells[++iNum]);
                mapLableGraph.FindOrAdd(id).sComp   = sCells[++iNum];
                mapLableGraph.FindOrAdd(id).iCountComp  = FCString::Atoi(*sCells[++iNum]);
                mapLableGraph.FindOrAdd(id).fNormWeight = FCString::Atof(*sCells[++iNum]);
                mapLableGraph.FindOrAdd(id).X           = FCString::Atoi(*sCells[++iNum]);
                mapLableGraph.FindOrAdd(id).Y           = FCString::Atoi(*sCells[++iNum]);
                mapLableGraph.FindOrAdd(id).Z           = FCString::Atoi(*sCells[++iNum]);
                mapLableGraph.FindOrAdd(id).Color       = GM->StringToColor(sCells[++iNum]);

            }
        }
        else
        {
            UE_LOG(LogTemp, Error, L"Failed to load Label.csv");
        }
    }

    {
        FString sFileContent;
        FString sRibPath = sFilePath;
        sRibPath += L"/Rib.csv";
        if (GM->LoadFileCP1251(sRibPath, sFileContent))
        {
            UE_LOG(LogTemp, Log, TEXT("OK Rib.csv"));

            TArray<FString> arrLines;
            sFileContent.ParseIntoArrayLines(arrLines);

            int Y = -2;
            for (const FString& sLine : arrLines)
            {
                ++Y;
                if (Y == -1) continue;    // Не считываем заголовок
                TArray<FString> sCells;
                sLine.ParseIntoArray(sCells, *GM->sStringParse, true);

                FRibGraph fRowGraph;
                int       iNum = -1;

                mapRibGraph.FindOrAdd(Y).L          = FCString::Atoi(*sCells[++iNum]);
                mapRibGraph.FindOrAdd(Y).R          = FCString::Atoi(*sCells[++iNum]);
                mapRibGraph.FindOrAdd(Y).sType      = sCells[++iNum];
                mapRibGraph.FindOrAdd(Y).sLabel     = sCells[++iNum];
                mapRibGraph.FindOrAdd(Y).fWeight    = FCString::Atof(*sCells[++iNum]);

            }
        }
        else
        {
            UE_LOG(LogTemp, Error, L"Failed to load Rib.csv");
        }

        arrLableGraph.SetNum(mapLableGraph.Num()); 
        for (const auto& [Key, Value] : mapLableGraph)
        {
            if (arrLableGraph.Num() <= Key) 
                UE_LOG(LogTemp, Error, L"Bad index arrLableGraph");
            arrLableGraph[Key] = Value;
        }

        arrRibGraph.SetNum(mapRibGraph.Num());
        for (const auto& [Key, Value] : mapRibGraph)
        {
            if (arrRibGraph.Num() <= Key)
            {
                UE_LOG(LogTemp, Error, L"Bad index arrRibGraph");
                break;
            }
            arrRibGraph[Key] = Value;
        }

    }

    AfterBeginPlay();
    /*FString FileDataTablePath = FilePath;
    UDataTable* MyTable = LoadObject<UDataTable>(nullptr, L"/Game/New/Data/FRowGraph.FRowGraph");

    if (MyTable)
    {
        static const FString ContextString(TEXT("My Context"));
        TArray<FRowGraph*>   AllRows;
        MyTable->GetAllRows<FRowGraph>(ContextString, AllRows);

        for (auto* Row : AllRows)
        {
            UE_LOG(LogTemp, Log, TEXT("X: %d, Y: %d, Z: %d, "), Row->X, Row->Y, Row->Z);
        }
    }*/

    // ...
}

//// Called every frame
//void UMyBuildGraph::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//    // ...
//}