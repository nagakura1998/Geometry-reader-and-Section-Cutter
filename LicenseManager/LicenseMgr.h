#pragma once
#include <chrono>

class __declspec(dllexport) LicenseManager {
private:
	int PERIOD = 365;
	struct tm m_BeginPeriod{ 0, 0, 12 };
public:
	LicenseManager(int startDay, int startMonth, int startYear) {
		// Set up the date structure
		m_BeginPeriod.tm_year = startYear - 1900;
		m_BeginPeriod.tm_mon = startMonth - 1;  // note: zero indexed
		m_BeginPeriod.tm_mday = startDay;       // note: not zero indexed
	}

	int DayLeft() {
		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
		time_t tt = std::chrono::system_clock::to_time_t(now);

		tm utc_tm;
		gmtime_s(&utc_tm , &tt);
		tm local_tm;
		localtime_s(&local_tm, &tt);
		
		DatePlusDays(&m_BeginPeriod, PERIOD);
		time_t endPeriod = mktime(&m_BeginPeriod);
		int days = (int)(mktime(&m_BeginPeriod)-tt) / (24 * 60 * 60);
		return days;
	}

	void DatePlusDays(struct tm* date, int days)
	{
		const time_t ONE_DAY = 24 * 60 * 60;

		// Seconds since start of epoch
		time_t date_seconds = mktime(date) + (days * ONE_DAY);

		// Update caller's date
		// Use localtime because mktime converts to UTC so may change date
		localtime_s(date, &date_seconds);
	}

};