#pragma once
#include <utility>
#include <iostream>


class Log {
public:
	Log(const char* logger_name) {
		m_Name = logger_name;
	}

	~Log() {
		this->Warn("Destroying log: ", m_Name);
	}

	template <typename... Args> 
	void Info(Args&&... args) {
		log_impl("\033[94m[", m_Name, " INFO]: ", std::forward<Args>(args)...);
	}

	template <typename... Args> 
	void Warn(Args&&... args) {
		log_impl("\033[93m[", m_Name, " WARN]: ", std::forward<Args>(args)...);
	}

	template <typename... Args> 
	void Error(Args&&... args) {
		log_impl("\033[41m\033[97m[", m_Name, " ERROR]: ", std::forward<Args>(args)...);
	}

private:	
	const char* m_Name = "Logger";

	void log_impl() {
		std::cout << "\033[0m"<< std::endl;
	}

	template <typename T, typename... Args>
	void log_impl(T&& first_arg, Args&&... remaining_args) {

		std::cout << std::forward<T>(first_arg);
		log_impl(std::forward<Args>(remaining_args)...);
	}
};
