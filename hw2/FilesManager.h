#ifndef FILES_MANAGER_H_
#define FILES_MANAGER_H_

#include <string>
#include <vector>

class FilesManager
{
private:
	FilesManager();
	~FilesManager();

public:

	static std::vector<std::string> getFileNames();
	static bool getScreenId(const std::string & filePath, unsigned & id);
};

#endif /* FILES_MANAGER_H_ */
