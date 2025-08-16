// Fill out your copyright notice in the Description page of Project Settings.

#include "MyBuildGraph.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/Paths.h"
#include "MyGameModeBase.h"

// Sets default values for this component's properties
UMyBuildGraph::UMyBuildGraph() : iScale(10), iHowManyLable(20)
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
        FString sFileContent;
        FString sLablePath = sFilePath;
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

                //UE_LOG(LogTemp, Log, TEXT("%s"), *sLine);
                //UE_LOG(LogTemp, Log, TEXT("%s"), *GM->sStringParse);
                TArray<FString> sCells;
                sLine.ParseIntoArray(sCells, *GM->sStringParse, true);

                int iNum = -1;
                int id   = FCString::Atoi(*sCells[++iNum]);

                FString& sText = mapLableGraph.FindOrAdd(id).sLabel = sCells[++iNum];
                mapLableGraph.FindOrAdd(id).dWeight                 = FCString::Atof(*sCells[++iNum]);
                mapLableGraph.FindOrAdd(id).sTag                    = sCells[++iNum];
                mapLableGraph.FindOrAdd(id).iCourse                 = FCString::Atoi(*sCells[++iNum]);
                mapLableGraph.FindOrAdd(id).sComp                   = sCells[++iNum];
                mapLableGraph.FindOrAdd(id).iCountComp              = FCString::Atoi(*sCells[++iNum]);
                float dWeight = mapLableGraph.FindOrAdd(id).dNormWeight = FCString::Atof(*sCells[++iNum]);
                mapLableGraph.FindOrAdd(id).vPos.X                      = FCString::Atoi(*sCells[++iNum]);
                mapLableGraph.FindOrAdd(id).vPos.Y                      = FCString::Atoi(*sCells[++iNum]);
                mapLableGraph.FindOrAdd(id).vPos.Z                      = FCString::Atoi(*sCells[++iNum]);
                FVector& vPos                                           = mapLableGraph.FindOrAdd(id).vPos;
                FColor&  fColor = mapLableGraph.FindOrAdd(id).fColor = GM->StringToColor(sCells[++iNum]);
                //DrawText(sText, vPos, fColor, dWeight);
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

                int& L = mapRibGraph.FindOrAdd(Y).L = FCString::Atoi(*sCells[++iNum]);
                int& R = mapRibGraph.FindOrAdd(Y).R = FCString::Atoi(*sCells[++iNum]);
                mapRibGraph.FindOrAdd(Y).sType      = sCells[++iNum];
                mapRibGraph.FindOrAdd(Y).sLabel     = sCells[++iNum];
                float& fWeight = mapRibGraph.FindOrAdd(Y).fWeight = FCString::Atof(*sCells[++iNum]);

                DrawDebugLine(GetWorld(),
                              mapLableGraph[L].vPos,
                              mapLableGraph[R].vPos,
                              mapLableGraph[L].fColor,
                              true,
                              0.f,
                              0,
                              fWeight * iScale);
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, L"Failed to load Rib.csv");
        }

        arrLableGraph.SetNum(mapLableGraph.Num());
        for (const auto& [Key, Value] : mapLableGraph)
        {
            if (arrLableGraph.Num() <= Key) UE_LOG(LogTemp, Error, L"Bad index arrLableGraph");
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
}

void UMyBuildGraph::DrawText(const FString& sText, const FVector& vPos, const FColor& fColor, const float& dWeight)
{
    DrawDebugString(GetWorld(),
                    vPos,
                    sText,    // текст
                    nullptr,
                    fColor,
                    9999999.0f,    // время жизни (сек)
                    true
                    );

    // Для видимости
    //DrawDebugString(GetWorld(),
    //                vPos,
    //                sText,    // текст
    //                nullptr,
    //                FColor::Black,
    //                9999999.0f,    // время жизни (сек)
    //                true,
    //                dWeight / 2.f);
}

void UMyBuildGraph::ReShaderText()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC)
    {
        APawn* PlayerPawn = PC->GetPawn();
        if (PlayerPawn)
        {
            FVector vLoc = PlayerPawn->GetActorLocation();

            TArray<TPair<FVector, int>> arrSortLableDist;

            int i = -1;
            for (const auto& fLable : arrLableGraph)
            {
                ++i;
                TPair<FVector, int> fNew(fLable.vPos - vLoc, i);
                arrSortLableDist.Add(fNew);
            }
            arrSortLableDist.Sort([](const TPair<FVector, int>& L, const TPair<FVector, int>& R) 
                { return L.Key.Length() < R.Key.Length(); });

            for (i = 0; i < iHowManyLable; ++i)
            {
                const auto& fLab = arrLableGraph[i];
                DrawText(fLab.sLabel, fLab.vPos, fLab.fColor, fLab.dWeight);
            }
        }
    }
}

//// Called every frame
// void UMyBuildGraph::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//     Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//     // ...
// }