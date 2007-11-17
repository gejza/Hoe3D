//////////////////////////////////////////////////////////////////////////////////////////
//	BITSET.h
//	class declaration for set of bits to represent many true/falses
//	You may use this code however you wish, but if you do, please credit me and
//	provide a link to my website in a readme file or similar
//	Downloaded from: www.paulsprojects.net
//	Created:	8th August 2002
//////////////////////////////////////////////////////////////////////////////////////////	

#ifndef BITSET_H
#define BITSET_H

class BITSET
{
public:
	BITSET() : numBytes(0), bits(NULL)
	{}
	~BITSET()
	{
		if(bits)
			delete [] bits;
		bits=NULL;
	}

	bool Init(int numberOfBits);
	void ClearAll();
	void SetAll();

	void Clear(int bitNumber);
	void Set(int bitNumber);

	unsigned char IsSet(int bitNumber);

protected:
	int numBytes;	//size of bits array
	unsigned char * bits;
};

#endif

