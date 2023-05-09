#include "util/crappy_writer.hpp"

#include "util/log.hpp"

CrappyWriter::~CrappyWriter()
{
	if (!lists.empty()) {
		log_warning << m_filename << ": Not all lists were closed in Writer" << std::endl;
	}
	os.close();
}

CrappyWriter::CrappyWriter(const std::string& filename) :
	m_filename(filename),
	lists(),
	os(filename),
	indent_depth(0)
{}

void CrappyWriter::write_list(const std::string& listname)
{
	indent();
	++indent_depth;
	os << listname << "-start\n";
	lists.push_back(listname);
}

void CrappyWriter::end_list(const std::string& listname)
{
	if (lists.empty()) {
		log_warning << m_filename << ": Trying to end list '" << listname << "' but it was not opened" << std::endl;
		return;
	}

	if (lists.back() != listname) {
		log_warning << m_filename << ": Trying to end list '" << listname << "' but last list opened was '" << lists.back() << "'" << std::endl;
		return;
	}
	--indent_depth;
	indent();
	os << listname << "-end\n";
	lists.pop_back();
}

void CrappyWriter::write(const std::string& name, int value)
{
	indent();
	os << name << " " << value << "\n";
}

void CrappyWriter::write(const std::string& name, float value)
{
	indent();
	os << name << " " << value << "\n";
}

void CrappyWriter::write(const std::string& name, const std::string& value)
{
	indent();
	os << name << " " << value << "\n";
}

void CrappyWriter::write(const std::string& name, const std::vector<int>& values)
{
	indent();
	os << name;
	for (const auto& i : values ) os << " " << i;
	os << "\n";
}

void CrappyWriter::indent()
{
	for (int i = 0; i < indent_depth; ++ i) {
		os << "\t";
	}
}