#ifndef HEADER_QBSKR_UTIL_CRAPPY_WRITER_HPP
#define HEADER_QBSKR_UTIL_CRAPPY_WRITER_HPP

#include <fstream>
#include <vector>
#include <string>

class CrappyWriter final {
private:
	CrappyWriter() = delete;

public:
	~CrappyWriter();

private:
	CrappyWriter(const CrappyWriter&) = delete;
	CrappyWriter& operator=(const CrappyWriter&) = delete;

private:
	std::string m_filename;
	std::vector<std::string> lists;
	std::ofstream os;
	int indent_depth;

public:
	CrappyWriter(const std::string& filename);

	void write_list(const std::string& listname);
	void end_list(const std::string& listname);

	void write(const std::string& name, int value);
	void write(const std::string& name, float value);
	void write(const std::string& name, const std::string& value);
	void write(const std::string& name, const std::vector<int>& values);

private:
	void indent();
};

#endif