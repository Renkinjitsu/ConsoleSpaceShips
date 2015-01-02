#ifndef FILES_MANAGER_H_
#define FILES_MANAGER_H_

#include <string>
#include <vector>

class FilesManager
{
public:

	enum FileType
	{
		FILE_TYPE_LEVEL,
		FILE_TYPE_SAVE,
		FILE_TYPE_SOLUTION
	};

private:
	FilesManager();
	~FilesManager();

	static std::string _workingDirectory;

	static const std::string FILE_EXTENSION_LEVEL;
	static const std::string FILE_EXTENSION_GAME_SAVE;
	static const std::string FILE_EXTENSION_SOLUTION;

	static const std::string & getExtension(FilesManager::FileType fileType);

public:

	static bool getScreenId(const std::string & fileName, FilesManager::FileType fileType, unsigned & id);

	static std::vector<std::string> getFilesList(const FilesManager::FileType fileType);
	static void changeDirectory(const char * const newPath);

	static std::ofstream * createFile(const std::string & fileName, FilesManager::FileType fileType);
	static std::ifstream * openFile(const std::string & fileName, FilesManager::FileType fileType);
};

#endif /* FILES_MANAGER_H_ */
