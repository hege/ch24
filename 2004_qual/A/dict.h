
#include <stdlib.h>
#include <ctype.h>


class dictNode
{
public:
	dictNode	*subNodes[26];
	bool		isnil;
	bool		hasval;
	int			depth;

	dictNode(int depth) 
	: depth(depth)
	{ 
		isnil = true;
		hasval = false;
	};

	void addWord(const char* szWord)
	{
		if ( depth == strlen(szWord) ) {
			hasval = true;
		} else {
			if ( isnil ) {
				for ( int i = 0; i < 26; subNodes[i++] = new dictNode(depth+1) );
				isnil = false;
			}
			subNodes[tolower(szWord[depth]) - 'a']->addWord(szWord);
		}
	};

	bool getWord(char *szWord)
	{
		if ( depth == strlen(szWord) )
		{
			return hasval;
		} else {
			if ( isnil )
				return false;
			else {
				if ( szWord[depth] == '*' ) {
					for ( int i = 0; i < 26; i++ ) {
						if ( subNodes[i]->getWord(szWord) ) {
							szWord[depth] = i + 'a';
							return true;
						}
					}
					return false;
				} else {
					return subNodes[tolower(szWord[depth]) - 'a']->getWord(szWord);
				}
			}
		}
	};
};

