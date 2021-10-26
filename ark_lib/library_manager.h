#pragma once
#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>
/**
* ���� md5 Ϊ�����ļ�Ψһʶ���
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
	std::string insert(const std::string&, bool=true);		// �����ļ� signed
	std::string get(const std::string&);			// �����ļ� signed
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