#include "SingletonShared.h"
#include "GlobalShare.h"

CSingletonShared::CSingletonShared()
{
}


CSingletonShared::~CSingletonShared()
{
}



void CSingletonShared::scanModeling(int nfuncode, int nTestMode, bool isSubBaseline, bool isSmooth, float fPeakPosition, std::string strTestName, float fCentration, float fMol, float fTotalPa, float fIteamPa)
{

	
	revMessage(nfuncode);
}

void CSingletonShared::scanVerify(int nfuncode, int nTestMode, bool isSubBaseline, bool isSmooth, float fPeakPosition, std::string strTestName, float fCentration, float fMol, float fTotalPa, float fIteamPa)
{




	revMessage(nfuncode);
}

void CSingletonShared::revMessage(int nfuncode)
{

	if (functionMap.find(nfuncode)!=functionMap.end())
	{
		functionMap[nfuncode]();
	}
	
}
