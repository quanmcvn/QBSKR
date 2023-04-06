#ifndef HEADER_QBSKR_UTIL_LINE_ITERATOR_HPP
#define HEADER_QBSKR_UTIL_LINE_ITERATOR_HPP

#include <string>

/** 
 *  Splits a given string at newlines.
 *  A newline at the end of string is ignored, others are returned as empty.
 *  Newlines aren't included in the returned string.
 *  Usage:
 *  	LineIterator it("Hello\nWorld");
 *  	while (it.next())
 *  		std::cout << it.get() << std::endl;
*/
class LineIterator final {
private:
	std::string::const_iterator first;
	const std::string::const_iterator last;
	std::string::const_iterator line_end;

public:
	LineIterator(const std::string& string);

	/// @return false when no character left, true otherwise
	bool next();

	/// @return the current line
	std::string get();
};

#endif