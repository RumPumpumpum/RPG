#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "RPGQuestData.generated.h"

USTRUCT(BlueprintType)
struct FRPGQuestData : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

public:
    FRPGQuestData()
        : QuestName(TEXT(""))
        , Description(TEXT(""))
        , Accepted(false)
        , Completed(false)
        , KilledCnt(0)
        , RequireCnt(0)
        , Reward(0)
        , QuestID(0)
    {}

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
    FString QuestName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
    bool Accepted;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
    bool Completed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
    int32 KilledCnt;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
    int32 RequireCnt;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
    int32 Reward;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
    int32 QuestID;
};