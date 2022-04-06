// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"
#include <iostream>
#include <fstream>
#include <sstream>
//#include "Game.h"
// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AMyActor::SignIn(FString memberName)
{
	UE_LOG(LogTemp, Warning, TEXT("Loading member : %s"), *memberName);
	FMember m = GetMember(memberName);

	//Print all members and kollektiver
	UE_LOG(LogTemp, Warning, TEXT("Alle pointers stored :"));
	for (int i = 0; i < allKollektiver.Num(); i++) {
		UE_LOG(LogTemp, Warning, TEXT("Kollektiv : %s"), *allKollektiver[i].mName);
	}

	for (int i = 0; i < allMembers.Num(); i++) {
		UE_LOG(LogTemp, Warning, TEXT("Member : %s"), *allMembers[i].mName);
	}
	return true;
}

FMember AMyActor::GetMember(FString name)
{
	for (int i = 0; i < allMembers.Num(); i++) {
		if (allMembers[i].mName == name) {
			return allMembers[i];
		}
	}
	FString memberName = name;
	
	//read file to get points
	name.Append(".txt");
	std::ifstream file;
	//Add content directory to the member name
	FString fileLoc = FPaths::ProjectContentDir();
	//Add the specific file ending
	fileLoc.Append(name);
	//Open the file, converts fstring to std::string
	file.open(std::string(TCHAR_TO_UTF8(*fileLoc)));
	//Coundt open the file
	if (!file) {
		UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Did not load text from file"));
		UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Expected file location: %s"), *fileLoc);
		UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Creating new file  %s"), *fileLoc);

		FMember member;
		member.mName = memberName;
		UE_LOG(LogTemp, Warning, TEXT("Created member with name = %s"), *member.mName);

		SaveMember(member);
		AddMember(member);
		return member;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Did load text from file %s"), *name);
	}
	//Parse the file
	std::string oneLine;
	std::string oneWord;
	std::string tmp;
	
	FMember m;
	m.mName = memberName;
	while (std::getline(file, oneLine, '\n')) {
		//To store one word
		std::stringstream sstream;
		sstream << oneLine;
		//Reset the word for each line
		oneWord = "";
		//Put it into oneWord
		sstream >> oneWord;

		//check for member to load
		if (oneWord == "k") {
			//Store member name
			sstream >> tmp;
			
			m.mKollektiver.Add((UTF8_TO_TCHAR(tmp.c_str())));
			//This is a kollektiv about the be declared
			UE_LOG(LogTemp, Warning, TEXT("Found member: %s"), *memberName);
			continue;
		}
		
		if (oneWord == "p") {
			sstream >> tmp;
			m.mPoints = std::stoi(tmp);
		}
	}

	AddMember(m);

	
	return m;

}

FKollektiv AMyActor::GetKollektiv(FString name) {
	for (int i = 0; i < allKollektiver.Num(); i++) {
		if (allKollektiver[i].mName == name) {
			return allKollektiver[i];
		}
	}
	FString kollektivName = name;
	//Check if the kollektiv is already loaded
	for (int i = 0; i < allKollektiver.Num(); i++) {
		if (allKollektiver[i].mName == kollektivName) {
			return allKollektiver[i];
		}
	}
	//Create a new kollektiv 

	kollektivName.Append(".txt");
	std::ifstream file;
	//Add content directory to the member name
	FString fileLoc = FPaths::ProjectContentDir();
	//Add the specific file ending
	fileLoc.Append(kollektivName);
	//Open the file, converts fstring to std::string
	file.open(std::string(TCHAR_TO_UTF8(*fileLoc)));
	//Coundt open the file
	if (!file) {
		UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Did not load text from file"));
		UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Expected file location: %s"), *fileLoc);
		//Create a new file
		FKollektiv k;
		k.mName = name;
		SaveKollektiv(k);
		return k;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Did load text from file %s"), *kollektivName);
	}

	std::string oneLine;
	std::string oneWord;
	std::string tmp;

	FString kName = name;
	FKollektiv kollektiv;

	while (std::getline(file, oneLine, '\n')) {
		//To store one word
		std::stringstream sstream;
		sstream << oneLine;
		//Reset the word for each line
		oneWord = "";
		//Put it into oneWord
		sstream >> oneWord;

		//check for member to load
		if (oneWord == "m") {
			//Store member name
			sstream >> tmp;
			kName = tmp.c_str();

			//Adds the member name to member to load
			kollektiv.mMembers.Add(UTF8_TO_TCHAR(tmp.c_str()));
			//This is a kollektiv about the be declared
			UE_LOG(LogTemp, Warning, TEXT("Found member: %s"), *kName);
			continue;
		}

		//check for room to load
		if (oneWord == "r") {
			//Store the room name
			sstream >> tmp;
			UE_LOG(LogTemp, Warning, TEXT("Found room: %s"), *tmp.c_str());
			continue;
		}
	}
	
	//Load all the rooms found in this kollektiv, load all the tasks found in this room

	kollektiv.mName = kollektivName;
	return kollektiv;
}



void AMyActor::SaveKollektiv(FKollektiv k)
{
}

void AMyActor::SaveMember(FMember m) {
	//Add content directory to the member name
	FString fileLoc = FPaths::ProjectContentDir();
	//Add the specific file ending
	fileLoc.Append(m.mName + ".txt");
	std::ofstream file(TCHAR_TO_UTF8(*fileLoc), std::ios_base::out);
	if (file.is_open()) {
		for (int i = 0; i < m.mKollektiver.Num(); i++) {
			file << "k " << TCHAR_TO_UTF8(*m.mKollektiver[i]) << "\n"; 
		}

		file << "p " << std::to_string(m.mPoints);
	}

	file.close();
}

void AMyActor::AddMember(FMember m)
{
	//Check if there is already a pointer to the member
	for (int i = 0; i < allMembers.Num(); i++) {
		if (allMembers[i].mName == m.mName) {
			return;
		}
	}
	allMembers.Add(m);
}

void AMyActor::AddKollektiv(FKollektiv k)
{
	//Check if the kollektiv is already loaded
	for (int i = 0; i < allKollektiver.Num(); i++) {
		if (allKollektiver[i].mName == k.mName) {
			return;
		}
	}
	allKollektiver.Add(k);
}

TArray<FKollektiv> AMyActor::GetSignedInMembersKollektiver()
{
	TArray<FKollektiv> kollektiver;
	for (int i = 0; i < signedInMember.mKollektiver.Num(); i++) {
		FKollektiv k = GetKollektiv(signedInMember.mKollektiver[i]);
		kollektiver.Add(k);
	}
	return kollektiver;
}
