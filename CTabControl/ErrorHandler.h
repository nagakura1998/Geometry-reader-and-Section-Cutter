#pragma once
#include "stdafx.h"

class MLError {
private:
	CString m_message;
public:
	MLError(CString err) : m_message(err) {}

	CString GetMessage() {
		return m_message;
	}
};

class ErrorHandler
{
public:
	static void CheckPathExist(CString paramName, CString path) {
		if (path.IsEmpty()) {
			throw MLError(paramName + _T(" must not empty!"));
		}
		else if (!PathFileExists(path)) {
			throw MLError(paramName + _T(" directory ") + path + _T(" is not exist!"));
		}
	}

	template <typename T>
	static void CheckValueInRange(CString paramName,  T value, T lowerRange, T upperRange) {
		if (value < lowerRange || value > upperRange) {
			CString message;
			if constexpr (std::is_same_v<T, int>) {
				message.Format(paramName + _T(" must between %d and %d"), lowerRange, upperRange);
			}
			else {
				message.Format(paramName + _T(" must between %f and %f"), lowerRange, upperRange);
			}
			throw MLError(message);
		}
	}

	template <typename T>
	static void CheckValueHigher(CString paramName, T value, T lowerRange)
	{
		if (value <= lowerRange) {
			CString message;
			if constexpr (std::is_same_v<T, int>) {
				message.Format(paramName + _T(" must higher than %d"), lowerRange);
			}
			else {
				message.Format(paramName + _T(" must higher than %f"), lowerRange);
			}
			throw MLError(message);
		}
	}

	template <typename T>
	static void CheckValueLower(CString paramName, T value, T upperRange)
	{
		if (value >= upperRange) {
			CString message;
			if constexpr (std::is_same_v<T, int>) {
				message.Format(paramName + _T(" must lower than %d"), upperRange);
			}
			else {
				message.Format(paramName + _T(" must lower than %f"), upperRange);
			}
			throw MLError(message);
		}
	}
};