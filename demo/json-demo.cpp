#include "nlohmann_json.hpp"

#include <iostream>
#include <string>

using json = nlohmann::json;

namespace {
	void testfunc1() {
		std::cout << "## NOT_FOUND_KEY test ##" << std::endl;

		json j;
		try {
			std::string not_found_value = j["not_found"].get<std::string>();
			std::cout << "not_found_value='" << not_found_value << "'" << std::endl;
		} catch(std::domain_error& e) {
			// access not existing key causes exception
			std::cout << "Error not_found_key:" << e.what() << std::endl;
		}
	}

	void testfunc2() {
		std::cout << "## json array  ##" << std::endl;

		json j = json::array();
		try {
			j["hoge"] = 10;
		} catch(std::domain_error& e) {
			// Using string key for array causes exception
			std::cout << "Error array with string index:" << e.what() << std::endl;
		}
	}

	void testfunc3() {
		std::cout << "## json setting  ##" << std::endl;

		json j;
		j["1"] = 1;
		j["2"] = 2;

		json k;
		k["3"] = 3;
		k["4"] = 4;

		j["k"] = k;

		std::cout << j.dump() << std::endl;
	}

	void testfunc4() {
		std::cout << "## json different type access  ##" << std::endl;

		json j;
		j["1"] = 1;
		j["2"] = 2;

		try {
			std::cout << "value[2] = " << j["2"].get<std::string>() << std::endl;
		} catch(std::domain_error& e) {
			std::cout << "Error different type access:" << e.what() << std::endl;
		}
	}

	void testfunc5() {
		std::cout << "## Non-initialize json stringify  ##" << std::endl;

		json j;
		std::cout << "Non initialize json dump:" << j.dump() << std::endl; // null

		std::cout << "Empty object dump:" << json::object().dump() << std::endl;
	}

	void testfunc6() {
		std::cout << "## Array test  ##" << std::endl;

		json a = {1, 2, 3.0, "hoge"};
		json j = a;
		std::cout << "Array:" << j.dump() << std::endl; // null
	}

	void testfunc7() {
		std::cout << "## null test  ##" << std::endl;

		json n = nullptr;
		std::cout << "null is object ?:" << n.is_object() << std::endl;
	}

	void testfunc8() {
		std::cout << "## Copy test  ##" << std::endl;

		json n = {{"foo", 1}, {"bar", 2}};
		json k = n;
		k["foo"] = 99;

		std::cout << "orig:" << n.dump() << std::endl;
		std::cout << "copy:" << k.dump() << std::endl;
	}

	void testfunc9() {
		std::cout << "## un exist key type  ##" << std::endl;

		json n = json::object();
		json k = n["hoge"];
		std::cout << "orig:" << k.is_string() << std::endl;
		std::cout << "orig:" << k.is_null() << std::endl;
	}
} // namespace

int main(void)
{
	std::string input("{\"foo\": \"bar\" }");
	json j;

	try {
		j = json::parse(input);
	} catch(std::invalid_argument& e) {
		std::cout << e.what() << std::endl;
		return 1;
	}
	std::cout << j.dump() << std::endl;

	testfunc1();
	testfunc2();
	testfunc3();
	testfunc4();
	testfunc5();
	testfunc6();
	testfunc7();
	testfunc8();
	testfunc9();

	return 0;
}