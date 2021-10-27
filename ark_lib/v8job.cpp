#include "v8job.h"

v8job::v8job(const std::string& id): id(id), script(""), result("") {
}

v8job::v8job() : id(""), script(""), result("") {
}

v8job::v8job(const v8job& self): id(self.id), script(self.script), result(self.result), onReturn(self.onReturn) {
}

v8job::v8job(v8job&& self) : id(self.id), script(self.script), result(self.result), onReturn(self.onReturn) {
}
v8job& v8job::operator=(const v8job& job) {
	this->id = job.id;
	this->script = job.script;
	this->result = job.result;
	this->onReturn = job.onReturn;
	return *this;
}

v8job& v8job::setScript(const std::string& _script) {
	this->script = _script;
	return *this;
}

const std::string& v8job::getResult() {
	return this->result;
}

v8job& v8job::setOnReturn(std::optional<std::function<void(std::string)>> fn) {
	if (fn)
		this->onReturn = fn;
	return *this;
}

void v8job::run(v8vm& vm){
	auto r = vm.script(this->script.c_str()).exec<std::string>();
	if (this->onReturn) {
		(*this->onReturn)(r);
	}
	this->result = r;
}