#include <limits.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (putchar(bytes[i]) == EOF)
			return false;
	return true;
}

static int printInt(int numToPrint, size_t maxrem) {
	int remainder = numToPrint;
	int remainder_div_ten = remainder/10;
	int divisor = 1;
	int quotient;
	int written = 0;
	size_t len = 0;
	while(divisor <= remainder_div_ten) {
		divisor *= 10;
		len++;
	}
	if (maxrem < len) {
		// TODO: Set errno to EOVERFLOW.
		return -1;
	}
	while(divisor) {
		quotient = remainder/divisor;
		putchar(quotient + '0');
		remainder %= divisor;
		divisor /= 10;
		written++;
	}
	return written;
}

int printf(const char* restrict format, ...) {
	va_list parameters;
	va_start(parameters, format);

	int written = 0;

	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;

		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, amount))
				return -1;
			format += amount;
			written += amount;
			continue;
		}

		const char* format_begun_at = format++;

		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(&c, sizeof(c)))
				return -1;
			written++;
		} else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(str, len))
				return -1;
			written += len;
		} else if (*format == 'd') {
			format++;
			const int num = va_arg(parameters, int);

			int remainder = num;
			int remainder_div_ten = remainder/10;
			int divisor = 1;
			size_t len = 1;

			while(divisor <= remainder_div_ten) {
				divisor *= 10;
				len++;
			}

			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}

			char numHolder[33] = "000000000000000000000000000000000";
			print(itoa(num, numHolder, 10), len);

			written += len;

		} else if (*format == 'X') {
			format++;
			const int num = va_arg(parameters, int);

			int remainder = num;
			int remainder_div_ten = remainder/16;
			int divisor = 1;
			size_t len = 1;

			while(divisor <= remainder_div_ten) {
				divisor *= 16;
				len++;
			}

			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}

			char numHolder[33] = "000000000000000000000000000000000";
			print(itoa(num, numHolder, 16), len);

			written += len;
		} else {
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, len))
				return -1;
			written += len;
			format += len;
		}
	}

	va_end(parameters);
	return written;
}
