#ifndef FILES_MANAGER_H_
#define FILES_MANAGER_H_

#include <string>
#include <vector>

class FilesManager
{
private:
	FilesManager();
	~FilesManager();

	static std::string _levelsPath;

	static const std::string FILE_EXTENSION_LEVEL;
	static const std::string FILE_EXTENSION_GAME_SAVE;
	static const std::string FILE_EXTENSION_SOLUTION;

public:

	enum FileType
	{
		FILE_TYPE_LEVEL,
		FILE_TYPE_SAVE,
		FILE_TYPE_SOLUTION
	};

	static bool getScreenId(const std::string & filePath, unsigned & id);

	static std::vector<std::string> getFilesList(const FilesManager::FileType fileType);
	static void setLevelsPath(const char * const newPath);

	static std::ofstream * createSaveFile(const std::string fileName);
	static std::ofstream * createSolutionFile(const std::string fileName);
};

#endif /* FILES_MANAGER_H_ */
