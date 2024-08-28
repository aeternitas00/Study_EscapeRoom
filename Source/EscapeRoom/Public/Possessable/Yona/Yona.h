#pragma once

#include "EscapeRoom.h"
#include "Possessable/EscapeRoomCharacterBase.h"
#include "Possessable/Yona/BatPet.h"

#include "Ability/EscapeRoomAbilityComponent.h"
#include "Yona.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UEscapeRoomAbilityComponent;

UCLASS( Blueprintable, ClassGroup=(Ability), meta=(BlueprintSpawnableComponent) )
class ESCAPEROOM_API UEscapeRoomYonaAbility : public UEscapeRoomAbilityComponent
{
    GENERATED_BODY()

public:
    TObjectPtr<AYona> OwnerYona;

public:	
    // Sets default values for this component's properties
    UEscapeRoomYonaAbility();

    virtual void MainAbility_Implementation(const FInputActionInstance& Instance);
    virtual void SubAbility1_Implementation(const FInputActionInstance& Instance);
    virtual void SubAbility2_Implementation(const FInputActionInstance& Instance);
};



UCLASS()
class ESCAPEROOM_API AYona : public AEscapeRoomCharacterBase
{
    GENERATED_BODY()

public:
    AYona();

protected:
    virtual void BeginPlay() override;
    //virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<USpringArmComponent> CameraBoom;

    /** Follow camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UCameraComponent> FollowCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UEscapeRoomYonaAbility> YonaAbilityComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components|Pet", meta = (AllowPrivateAccess = true))
    TSubclassOf<class ABatPet> BatPetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components|Pet", meta = (AllowPrivateAccess = true))
    ABatPet* MyPet;

    // Ability Func


public:
    //UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Pet")
    //UInputAction* MoveDirectAction;

    //UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Pet")
    //UInputAction* SwitchAction;

    void MovePetDirect();
    void SwitchWithPet();

    FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};


