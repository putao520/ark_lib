#include "restclient-cpp/restclient.h"
#include "library_manager.h"
#include "file_until.h"
#include "spdlog/spdlog.h"
#include <fstream>
#include <iostream>
#include "text_encode.h"

#include <windows.h>
#include "PdbInfo.h"

using namespace std;
using namespace nlohmann;
string library_dir = "./library/";
string list_path = library_dir + "list.json";
string download_url = "http://127.0.0.1:8000";
vector<string> support_list = { "hal.dll", "ntoskrnl.exe" };
library_manager::library_manager() {
	// 校验列表文件,移出错误文件列表
	content = load();
	string system_dir = systemDir();
	// 有效文件列表
	for (auto& n : support_list) {
		if (content.contains(n)) {
			string file_path = system_dir + n;
			PdbInfo pdn_info(file_path.c_str());
			if (pdn_info.getSigned() != content[n]) {
				content.erase(n);
			}
		}
	}
	// 根据 json 信息检查内容
	for (json::iterator it = content.begin(); it != content.end(); ) {
		const auto& n = it.key();
		FileUntil fu(library_dir + n);
		if (!fu.existing())
			it = content.erase(it);
		else
			it++;
	}
	
}

library_manager::~library_manager() {
	if (!content.is_null())
		flush();
}

library_manager* library_manager::update() {
	download_list.clear();
	for (auto& n : support_list) {
		// 目前 list 有缺失
		if (!content.contains(n)) {
			insert(n, false);
		}
	}
	this->download(this->download_list);
	flush();
	return this;
}

bool library_manager::remove(const std::string& key) {
	if (content.contains(key)) {
		content.erase(key);
		return true;
	}
	return false;
}

std::string library_manager::insert(const std::string& key, bool download) {
	string system_dir = systemDir();
	string file_path = system_dir + key;
	PdbInfo pdn_info(file_path.c_str());
	string sign = pdn_info.getSigned();
	if (content[key] != sign) {
		content[key] = sign;
		// 下载库
		if (download) {
			this->download(key);
		}
		else {
			download_list.push_back(key);
		}
	}
	return sign;
}

std::string library_manager::get(const std::string& key) {
	return content[key];
}

void library_manager::flush() {
	ofstream out(FileUntil::toAbsolute(list_path.c_str()));
	if (!out.is_open())
		return;
	out << content;
	out.close();
}

json library_manager::load() {
	json result = json::object({});
	fstream file(FileUntil::toAbsolute(list_path.c_str()), ios::in);
	if (file.is_open()) {
		try {
			file >> result;
		}
		catch (detail::exception&) {
		}
	}
	return result;
}

std::string library_manager::systemDir() {
	// 获得系统文件夹
	char system_dir_str[MAX_PATH] = { 0 };
	GetSystemDirectory(system_dir_str, MAX_PATH);
	string system_dir = system_dir_str;
	return system_dir + "\\";
}

bool library_manager::download(const string& name) {
	std::vector<std::string> temp_list;
	temp_list.push_back(name);
	return download(temp_list) > 0;
}

int library_manager::download(const std::vector<std::string>& download_list) {
	json body = json::object({ {"item", json::array({})} });
	auto item = body["item"];
	for (auto& name : download_list) {
		item.push_back(fileInfo(name));
	}
	body["item"] = item;
	// string info = body.dump();
	auto rep = RestClient::post(download_url + "/generator/upgrade", "application/json", body.dump());
	if (rep.code != 200)
		return false;
	// 根据返回结果下载库文件
	try {
		json result = json::parse(rep.body);
		if (!result.contains("data"))
			return false;
		if (result["code"] != 0)
			return false;
		json data = result["data"];
		// string info = data.dump();
		for (auto& [sni, url] : data.items()) {
			auto r = RestClient::get(url);
			if (r.code != 200)
				continue;
			FileUntil file(library_dir + sni + ".js");
			file.writeString(TextEncode::string_to_utf8(r.body));
		}
		return true;
	}
	catch (detail::exception& e) {
		spdlog::error("library download: {} ->error:{}", body.dump(), e.what());
		std::abort();
	}
	return false;
}

// 文件名获得符号元信息
nlohmann::json library_manager::fileInfo(const std::string& name) {
	string system_dir = systemDir();
	string file_path = system_dir + name;
	PdbInfo pdn_info(file_path.c_str());
	return json{ 
		{"id",pdn_info.getSigned()},
		{"name",pdn_info.getPdbName()},
		{"file",name}
	};
}