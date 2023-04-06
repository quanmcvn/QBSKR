#include "util/line_iterator.hpp"

LineIterator::LineIterator(const std::string& string) :
	first(string.begin()),
	last(string.end()),
	line_end(string.begin())
{}

bool LineIterator::next()
{
	// end of text
	if (line_end == last) return false;
	// a newline at the end of string is ignored
	if (line_end + 1 == last && *first == '\n') return false;
	
	// not set it rightaway since the constructor set first == line_end (== string.begin())
	if (first != line_end) first = line_end + 1;
	do {
		++ line_end;
	} while (line_end != last && *line_end != '\n');
	return true;
}

std::string LineIterator::get()
{
	return std::string(first, line_end);
}