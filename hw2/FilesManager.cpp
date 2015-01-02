#include "FilesManager.h"

#include <iostream>
#include <fstream>
#include <stdio.h>

#include "GameConfig.h"

std::string FilesManager::_workingDirectory(""); //Default path for level files

const std::string FilesManager::FILE_EXTENSION_LEVEL(".spg");
const std::string FilesManager::FILE_EXTENSION_GAME_SAVE(".spp");
const std::string FilesManager::FILE_EXTENSION_SOLUTION(".sps");

void FilesManager::changeDirectory(const char * const newPath)
{
	FilesManager::_workingDirectory = newPath;

	//Verify that the path ends with a folder seperator ('\' or '/')
	if(FilesManager::_workingDirectory.length() > 0)
	{
		const char lastCharacter = *(FilesManager::_workingDirectory.end() - 1);

		const size_t seperatorIndex = FilesManager::_workingDirectory.find_first_of('/');
		const char folderSeperator = (seperatorIndex == std::string::npos) ? '\\' : '/';

		if(lastCharacter != folderSeperator)
		{
			FilesManager::_workingDirectory += folderSeperator;
		}
	}
}

std::ofstream * FilesManager::createFile(const std::string & fileName, FilesManager::FileType fileType)
{
	const std::string filePath = FilesManager::_workingDirectory + fileName + FilesManager::getExtension(fileType);
	return new std::ofstream(filePath, std::ios::out | std::ios::trunc);
}

std::ifstream * FilesManager::openFile(const std::string & fileName, FilesManager::FileType fileType)
{
	const std::string filePath = FilesManager::_workingDirectory + fileName + FilesManager::getExtension(fileType);
	return new std::ifstream(filePath);
}

const std::string & FilesManager::getExtension(FilesManager::FileType fileType)
{
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
			fileTypeExtension = &FilesManager::FILE_EXTENSION_GAME_SAVE;
		}
		break;

		case FilesManager::FILE_TYPE_SOLUTION:
		{
			fileTypeExtension = &FilesManager::FILE_EXTENSION_SOLUTION;
		}
		break;
	}

	return *fileTypeExtension;
}

std::vector<std::string> FilesManager::getFilesList(const FilesManager::FileType fileType)
{
	std::vector<std::string> result;

	const char * const tempFileName = "temp";
	const std::string & extension = FilesManager::getExtension(fileType);

	//Build command
	std::string command("dir /b ");
	if(FilesManager::_workingDirectory.length() > 0)
	{
		command += '\"' + FilesManager::_workingDirectory + '\"';
	}
	command += "*" + extension + " > " + tempFileName;

	//Execute command
	system(command.c_str());

	//Read file names from the temporary result file
	std::ifstream tempFile(tempFileName);
	std::string str;
	while(std::getline(tempFile, str))
	{
		result.push_back(str.substr(0, str.length() - extension.length()));
	}

	//Clean-up
	tempFile.close();
	remove(tempFileName);

	return result;
}

bool FilesManager::getScreenId(const std::string & fileName, FilesManager::FileType fileType, unsigned & id)
{
	std::ifstream * levelFile = FilesManager::openFile(fileName, fileType);
	std::string idLine;
	std::getline(*levelFile, idLine);

	levelFile->close();
	delete levelFile;

	return (sscanf(idLine.c_str(), "ScreenID = %u", &id) == 1);
}
