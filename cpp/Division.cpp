// Divide two integers, accurate to an integer value, without using division operator

#include <iostream>

int divide(int a, int b) {

	if (a == 0) {
		return 0;
	}

	if (b == 0) {
		throw std::runtime_error("Invalid denominator");
	}

	int retval = 1;

	int sign = 1;
	if ((a < 0 && b > 0) || (a > 0 && b < 0)) {
		sign = -1;
	}

	if (a < 0) {
		a = -1 * a;
	}

	if (b < 0) {
		b = -1 * b;
	}
	
	if (a == b) {
		return 1;
	}

	if (a < b) {
		return 0;
	}


	int start = 1;
	int end = a;
	int prod = 1;

	while (true) {
		retval = (start + end) >> 1;
		
		prod = b*retval;
		
		if (prod == a) {
			break;
		}
		else if (prod < a) {
			if ((a - prod) < b) {
				break;
			}

			start = retval;
		}
		else {
			if ((prod - a) < b) {
				retval--;
				break;
			}

			end = retval;
		}
	}

	retval = sign * retval;

	return retval;
}

int main(int argc, char **argv) {
	std::cout << "23/23 = " << divide(23, 23) << std::endl;
	std::cout << "23/4 = " << divide(23, 4) << std::endl;
	std::cout << "23/6 = " << divide(23, 6) << std::endl;
	std::cout << "25/4 = " << divide(25, 4) << std::endl;
	std::cout << "24/6 = " << divide(24, 6) << std::endl;
	std::cout << "35/5 = " << divide(35, 5) << std::endl;
	std::cout << "35/-7 = " << divide(35, -7) << std::endl;
	std::cout << "-35/7 = " << divide(-35, 7) << std::endl;
	std::cout << "-35/-7 = " << divide(-35, -7) << std::endl;
}

