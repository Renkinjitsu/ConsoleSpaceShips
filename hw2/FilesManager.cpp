#include "FilesManager.h"

#include <iostream>
#include <fstream>
#include <stdio.h>

#include "GameConfig.h"

std::string FilesManager::_levelsPath(""); //Default path for level files (current directory)

const std::string FilesManager::FILE_EXTENSION_LEVEL(".spg");
const std::string FilesManager::FILE_EXTENSION_GAME_SAVE(".spp");
const std::string FilesManager::FILE_EXTENSION_SOLUTION(".sps");

void FilesManager::setLevelsPath(const char * const newPath)
{
	FilesManager::_levelsPath = newPath;

	//Verify that the path ends with a folder seperator ('\' or '/')
	if(FilesManager::_levelsPath.length() > 0)
	{
		const char lastCharacter = *(FilesManager::_levelsPath.end() - 1);

		const size_t seperatorIndex = FilesManager::_levelsPath.find_first_of('/');
		const char folderSeperator = (seperatorIndex == std::string::npos) ? '\\' : '/';

		if(lastCharacter != folderSeperator)
		{
			FilesManager::_levelsPath += folderSeperator;
		}
	}
}

std::ofstream * FilesManager::createSaveFile(const std::string fileName)
{
	const std::string filePath = FilesManager::_levelsPath + fileName + FilesManager::FILE_EXTENSION_GAME_SAVE;
	return new std::ofstream(filePath, std::ios::out | std::ios::trunc);
}

std::ofstream * FilesManager::createSolutionFile(const std::string fileName)
{
	const std::string filePath = FilesManager::_levelsPath + fileName + FilesManager::FILE_EXTENSION_SOLUTION;
	return new std::ofstream(filePath, std::ios::out | std::ios::trunc);
}

std::vector<std::string> FilesManager::getFilesList(const FilesManager::FileType fileType)
{
	std::vector<std::string> result;

	const char * const tempFileName = "temp";

	//Get extension string
	const std::string * fileTypeExtension;
	switch(fileType)
	{
		case FilesManager::FILE_TYPE_LEVEL:
		{
			fileTypeExtension = &FilesManager::FILE_EXTENSION_LEVEL;
		}
		break;

		case FilesManager::FILE_TYPE_SAVE:
		{
			fileTypeExtension = &FilesManager::FILE_EXTENSION_SOLUTION;
		}
		break;

		case FilesManager::FILE_TYPE_SOLUTION:
		{
			fileTypeExtension = &FilesManager::FILE_EXTENSION_SOLUTION;
		}
		break;
	}

	//Build command
	std::string command("dir /b ");
	if(FilesManager::_levelsPath.length() > 0)
	{
		command += '\"' + FilesManager::_levelsPath + '\"';
	}
	command += "*" + *fileTypeExtension + " > " + tempFileName;

	//Execute command
	system(command.c_str());

	//Read file names from the temporary result file
	std::ifstream tempFile(tempFileName);
	std::string str;
	while(std::getline(tempFile, str))
	{
		result.push_back(str);
	}

	//Clean-up
	tempFile.close();
	remove(tempFileName);

	return result;
}

bool FilesManager::getScreenId(const std::string & filePath, unsigned & id)
{
	std::ifstream levelFile(FilesManager::_levelsPath + filePath);
	std::string idLine;
	std::getline(levelFile, idLine);
	levelFile.close();

	return (sscanf(idLine.c_str(), "ScreenID = %u", &id) == 1);
}
