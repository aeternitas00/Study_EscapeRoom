#pragma once

#include "EscapeRoom.h"
#include "Possessable/EscapeRoomCharacterBase.h"
#include "Possessable/Yona/BatPet.h"
#include "Yona.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class ESCAPEROOM_API AYona : public AEscapeRoomCharacterBase
{
    GENERATED_BODY()

public:
    AYona();

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<USpringArmComponent> CameraBoom;

    /** Follow camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UCameraComponent> FollowCamera;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components|Pet", meta = (AllowPrivateAccess = true))
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

    FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};
