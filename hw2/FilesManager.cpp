#include "FilesManager.h"

#include <iostream>
#include <fstream>
#include <stdio.h>

#include "GameConfig.h"

FilesManager::FilesManager()
{
}

FilesManager::~FilesManager()
{
}

std::vector<std::string> FilesManager::getFileNames()
{
	std::vector<std::string> fileNames;

	std::string spgs("spgList"); //Name of the result file

	//Build command
	std::string command("dir /b ");
	command += GameConfig::getLevelsPath();
	command += "*.spg > ";
	command += spgs;

	//Execute command
	system(command.c_str());

	//Read file names from the result file
	std::ifstream spgsFile(spgs);
	std::string str;
	while(std::getline(spgsFile, str))
	{
		fileNames.push_back(str);
	}
	spgsFile.close();

	//Clean-up
	remove(spgs.c_str());

	return fileNames;
}

bool FilesManager::getScreenId(const std::string & filePath, unsigned & id)
{
	std::ifstream levelFile(filePath);
	std::string idLine;
	std::getline(levelFile, idLine);
	levelFile.close();

	return (sscanf(idLine.c_str(), "ScreenID = %u", &id) == 1);
}
