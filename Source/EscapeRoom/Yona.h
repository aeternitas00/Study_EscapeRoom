#pragma once

#include "CoreMinimal.h"
#include "EscapeRoomCharacter.h"
#include "BatPet.h"
#include "Yona.generated.h"

UCLASS()
class ESCAPEROOM_API AYona : public AEscapeRoomCharacter
{
    GENERATED_BODY()

public:
    AYona();

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components|Pet",meta= (AllowPrivateAccess = true))
    TSubclassOf<class ABatPet> BatPetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components|Pet", meta = (AllowPrivateAccess = true))
    ABatPet* MyPet;

    // Ability Func
    void MovePetDirect();
    void SwitchWithPet();

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Pet")
    UInputAction* MoveDirectAction;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Pet")
    UInputAction* SwitchAction;
};
