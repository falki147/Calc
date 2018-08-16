#include <Calc.h>
#include <stdio.h>

int main() {
	char buffer[512];
	
	while (fgets(buffer, sizeof(buffer), stdin) && buffer[0] != '\n') {
		CalcError error = { 0 };
		double ret = calc(buffer, NULL, NULL, &error);
		
		if (error.message)
			printf("Error: %s\n", error.message);
		else
			printf("Result: %f\n", ret);
	}
}
