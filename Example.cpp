#include <iostream>
#include <string>

#include "Calc.h"

void main() {
	while (std::cin) {
		std::string str;
		std::getline(std::cin, str);

		if (str.empty())
			continue;

		CalcError error = { 0 };

		auto ret = calc(str.c_str(), nullptr, nullptr, &error);

		if (error.message)
			std::cout << "Error: " << error.message << std::endl;
		else
			std::cout << "Result: " << ret << std::endl;
	}
}
