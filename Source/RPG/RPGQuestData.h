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
    {}

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
    FString QuestName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
    bool Accepted;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
    bool Completed;
};