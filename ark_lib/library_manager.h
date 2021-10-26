#pragma once
#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>
/**
* 本类 md5 为符号文件唯一识别符
* 
* 
{
	'name':'signed',
	'name':'signed',
	'name':'signed',
	...
}
* 
*/
class library_manager {
public:
	library_manager();
	~library_manager();
	library_manager* update();
	bool remove(const std::string&);
	std::string insert(const std::string&, bool=true);		// 返回文件 signed
	std::string get(const std::string&);			// 返回文件 signed
private:
	nlohmann::json content;

	void flush();
	nlohmann::json load();
	bool download(const std::string& name);
	int download(const std::vector<std::string>& download_list);
	std::string systemDir();
	nlohmann::json fileInfo(const std::string& name);
	std::unordered_map<std::string, std::string> file_list;
	std::vector<std::string> download_list;
};