#ifndef _MACRO_HPP_
#define _MACRO_HPP_

#include <string>
#include <vector>

typedef struct MNT
{
	std::string name;
	int numArg;
	int index;
}MNT;

class Macro
{
public:
	Macro(const std::string& inputFile, const std::string& outputFile);
	virtual ~Macro();
	std::vector<std::string> GetLineVector();

private:
	std::vector<MNT> nameTable;
	std::vector< std::vector<std::string> > defTable;
	std::string inputFile;
	std::string outputFile;
	std::vector<std::string> lineVector;






};

#endif //_MACRO_HPP_