// Fill out your copyright notice in the Description page of Project Settings.


#include "Game.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

Game::Game()
{
	SignIn("Adam");

	//Print all members and kollektiver
	UE_LOG(LogTemp, Warning, TEXT("Alle pointers stored :"));
	for (int i = 0; i < allKollektiver.Num(); i++) {
		UE_LOG(LogTemp, Warning, TEXT("Kollektiv : %s"), *allKollektiver[i]->mName);
	}

	for (int i = 0; i < allMembers.Num(); i++) {
		UE_LOG(LogTemp, Warning, TEXT("Member : %s"), *allMembers[i]->mName);
	}
}

Game::~Game()
{
}

void Game::SignIn(FString memberName)
{
	Member* m = LoadMember(memberName);
	if (m) {
		signedInMember = m;
	}
	else {
		//Create new member with this name
		m = new Member(memberName, 0);
		//Add the new member to all members
		AddMember(m);
		//Set it to the signedinmember
		signedInMember = m;
		//Create a file for the new member
		SaveMember(m);
	}
}
//Create a pointer from a loaded file
Kollektiv* Game::LoadKollektiv(FString kollektivName)
{
	//Check if the kollektiv is already loaded
	for (int i = 0; i < allKollektiver.Num(); i++) {
		if (allKollektiver[i]->mName == kollektivName) {
			return allKollektiver[i];
		}
	}
	//Create a new kollektiv 
	Kollektiv* kollektiv = new Kollektiv(kollektivName);
	
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
		return nullptr;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Did load text from file %s"), *kollektivName);
	}

	std::string oneLine;
	std::string oneWord;
	std::string tmp;
	TArray<FString> membersToLoad;
	TArray<FString> roomsToLoad;
	FString kName;
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
			membersToLoad.Add(tmp.c_str());
			//This is a kollektiv about the be declared
			UE_LOG(LogTemp, Warning, TEXT("Found member: %s"), *kName);
			continue;
		}

		//check for room to load
		if (oneWord == "r") {
			//Store the room name
			sstream >> tmp;
			roomsToLoad.Add(tmp.c_str());
			UE_LOG(LogTemp, Warning, TEXT("Found room: %s"), *tmp.c_str());
			continue;
		}
	}
	//Load all the members found in this kollektiv file and add them to the kollektiv
	for (int i = 0; i < membersToLoad.Num(); i++) {
		Member* m = LoadMember(membersToLoad[i]);
		if (m) {
			kollektiv->mMembers.Add(LoadMember(membersToLoad[i]));
		}
	}
	//Load all the rooms found in this kollektiv, load all the tasks found in this room
	
	return kollektiv;
}
//Create txt file from pointer
void Game::SaveKollektiv(Kollektiv* k)
{

}
//Create a pointer from laoded file
Member* Game::LoadMember(FString memberName)
{
	FString name = memberName;
	//Check if there is already a pointer to the member
	for (int i = 0; i < allMembers.Num(); i++) {
		if (!allMembers[i]) {
			
		}else if (allMembers[i]->mName == memberName) {
			return allMembers[i];
		}
	}
	//read file to get points
	memberName.Append(".txt");
	std::ifstream file;
	//Add content directory to the member name
	FString fileLoc = FPaths::ProjectContentDir();
	//Add the specific file ending
	fileLoc.Append(memberName);
	//Open the file, converts fstring to std::string
	file.open(std::string(TCHAR_TO_UTF8(*fileLoc)));
	//Coundt open the file
	if (!file) {
		UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Did not load text from file"));
		UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Expected file location: %s"), *fileLoc);
		return nullptr;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Did load text from file %s"), *memberName);
	}

	std::string oneLine;
	std::string oneWord;
	std::string tmp{};

	TArray<FString> kollektiverToLoad;
	int points = 0;
	FString mName;
	while (std::getline(file, oneLine, '\n')) {
		
		//To store one word
		std::stringstream sstream;
		sstream << oneLine;
		//Reset the word for each line
		oneWord = "";
		tmp = "";
		//Put it into oneWord
		sstream >> oneWord;

		if (oneWord == "k") {
			sstream >> tmp;
			mName = tmp.c_str();
			kollektiverToLoad.Add(tmp.c_str());
			
			//This is a kollektiv about the be declared
			UE_LOG(LogTemp, Warning, TEXT("Found kollektiv: %s"), *mName);
			
			continue;
		}

		if (oneWord == "p") {
			sstream >> tmp;
			points = std::stoi(tmp);
			//This is a kollektiv about the be declared
			//UE_LOG(LogTemp, Warning, TEXT("Found points: %s"), *tmp.c_str());
			//Returns a new member, initalized with the name sent in and the points found in their file
			
			continue;
		}
	}

	

	Member* m = new Member(name, points);
	AddMember(m);
	//Loads the members
	for (int i = 0; i < kollektiverToLoad.Num(); i++) {
		Kollektiv* k = LoadKollektiv(kollektiverToLoad[i]);
		if (k) {
			AddKollektiv(k);
			m->mKollektiver.Add(k);
		}
		
	}
	return m;

	return nullptr;
}
//Create txt file from pointer
void Game::SaveMember(Member* m) {
	//Write the file of a member
	std::fstream file;
}

//Adding to total amount refrences
void Game::AddMember(Member* m)
{
	//Check if there is already a pointer to the member
	for (int i = 0; i < allMembers.Num(); i++) {
		if (allMembers[i]->mName == m->mName) {
			return;
		}
	}
	allMembers.Add(m);
}

void Game::AddKollektiv(Kollektiv* k)
{
	//Check if the kollektiv is already loaded
	for (int i = 0; i < allKollektiver.Num(); i++) {
		if (allKollektiver[i]->mName == k->mName) {
			return;
		}
	}
	allKollektiver.Add(k);
}
