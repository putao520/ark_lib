#pragma once
#include <string>
#include <functional>
#include <optional>
#include "v8vm.h"

class v8job{
public:
	v8job();
	v8job(const v8job&);
	v8job(v8job&&);
	v8job& operator=(const v8job& job);
	v8job(const std::string& id);
	v8job& setScript(const std::string&);
	const std::string& getResult();

	v8job& setOnReturn(std::optional<std::function<void(std::string)>>);
	void run(v8vm& vm);
private:
	std::string id;
	std::string script;
	std::string result;

	std::optional<std::function<void(std::string)>> onReturn;
};