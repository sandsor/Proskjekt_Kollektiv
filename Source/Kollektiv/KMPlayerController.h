// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KMPlayerController.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FChangedWardrobe);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSpentGold);


UENUM(BlueprintType)
enum AvatarPieceType {
	HEAD, GLASSES, BODY, FEET, BACK
};
USTRUCT(BlueprintType)
struct FAvatar {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Avatar")
	UTexture2D* mBaseTexture;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Avatar")
	UTexture2D* mGlassesTexture;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Avatar")
		UTexture2D* mFeetTexture;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Avatar")
		UTexture2D* mHeadTexture;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Avatar")
		UTexture2D* mBackTexture;
	
	FAvatar() {
		mBaseTexture	= nullptr;
		mBackTexture	= nullptr;
		mGlassesTexture	= nullptr;
		mFeetTexture	= nullptr;
		mHeadTexture	= nullptr;
	}
};

USTRUCT(BlueprintType)
struct FAvatarPiece {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Avatar")
		FString mName = "Piece";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Avatar")
		UTexture2D* mTexture;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Avatar")
		TEnumAsByte<AvatarPieceType> mType = AvatarPieceType::HEAD;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Avatar")
		int mCost = 5;
	bool bIsOwnedBySignedInPlayer = false;
};

USTRUCT(BlueprintType)
struct FMember {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Member")
		FString mName = "Adam";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Member")
		int mPoints = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Member")
		FAvatar mAvatar; //The avatar refrence
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Member")
		TArray<FString> mUnlockedAvatarPieces; //Names of the pieces that is unlocked for the member

	FString mHead;
	FString mBack;
	FString mFeet;
	FString mGlasses;

	FMember() {
		mName = "";
		mPoints = 0;
	}
};

USTRUCT(BlueprintType)
struct FCompletedTask {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Member")
		FString mTaskName = "Gang";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Member")
		int mCompletedDate = 10;
};

USTRUCT(BlueprintType)
struct FTask {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Task")
		FString mTaskName = "kasse";					   
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Task")
		FString mTaskDesc = "Vask ting";				  
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Task")
		FMember mMember;								  
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Task")
		int mRemainingDays = 0;						
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Task")
		FDateTime TimeCreated{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Task")
		FDateTime TimeCompleted{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Task")
		bool bIsCompleted = false;
	FTask() {
		
	}
};

USTRUCT(BlueprintType)
struct FKollektiv {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kollektiv")
		FString mKollektivName = "Kollektiv";
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kollektiv")
		TArray<FMember> mMembers;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kollektiv")
		TArray<FTask> mTasks;

	FKollektiv() {
	}

};

UCLASS()
class KOLLEKTIV_API AKMPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	void LoadAllPieces();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Avatar")
	TArray<FAvatarPiece> mAllAvatarUnlockables;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Avatar")
		int number;
	FMember mSignedInMember;
	FKollektiv mActiveKollektiv;

	void DivideOutTasks();

	FMember GetMember(FString name);
	UFUNCTION(BlueprintCallable)
		void SaveMember(FMember m);
	FKollektiv GetKollektiv(FString name);
	UFUNCTION(BlueprintCallable)
		void SaveKollektiv(FKollektiv k);
	UFUNCTION(BlueprintCallable)
	bool AddTaskToKollektiv(FTask t);
	UFUNCTION(BlueprintCallable)
	bool AddMemberToKollektiv(FMember m);

	//Avatar
	UFUNCTION(BlueprintCallable)
	TArray<FAvatarPiece> GetAllAvatarUnlockables(); //All possible unlockable items for the avatar
	UFUNCTION(BlueprintCallable)
		TArray<FAvatarPiece> GetSignedInMembersUnlockedPieces(); //The pieces that the player has unlocked
	UFUNCTION(BlueprintCallable)
		bool EquipAvatarPiece(FAvatarPiece p);
	UFUNCTION(BlueprintCallable)
		bool BuyAvatarPiece(FAvatarPiece p);

	UFUNCTION(BlueprintCallable)
		TArray<FTask> GetAllTasks();
	UFUNCTION(BlueprintCallable)
		TArray<FTask> GetSignedInMembersTasks();
	UFUNCTION(BlueprintCallable)
		void CompleteTask(FTask t);

	UFUNCTION(BlueprintCallable)
	bool CanLogIn(FString name);
	UFUNCTION(BlueprintCallable)
	bool SignIn(FString memberName, FString kollektivName);
	UFUNCTION(BlueprintImplementableEvent)
		void SingedIn();
	UFUNCTION(BlueprintImplementableEvent)
		void OpenSignInScreen();
	UFUNCTION(BlueprintCallable)
	void SignOut();
	UFUNCTION(BlueprintCallable)
		FMember GetSignedInMember() { return mSignedInMember; };


	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "UpdatedWardrobe"))
		FChangedWardrobe OnUpdateWardrobe;
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "UpdateGold"))
		FSpentGold OnUpdateGold;
	UFUNCTION(BlueprintCallable)
	FDateTime GetDateTime() { return FDateTime::Now(); };
	UFUNCTION(BlueprintCallable)
	int DaysInMonth();
};
