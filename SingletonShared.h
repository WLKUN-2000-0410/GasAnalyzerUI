#pragma once
#include <functional>
#include <map>

class CSingletonShared
{
private:
	CSingletonShared();
public:
	~CSingletonShared();
	static CSingletonShared&  GetInstance()
	{
		static CSingletonShared instance;
		return instance;
	};

public:
	std::string meg;
	std::map<int , std::function<void()>> functionMap;
	
	//
	//void setFuntionCallback(std::function<void()> callback) {
	//	callbackfunc = callback;
	//}
	//void clearFuntionCallback() {
	//	callbackfunc = nullptr;
	//}
	void scanModeling(int nfuncode,int nTestMode, bool isSubBaseline, bool isSmooth, float fPeakPosition
		, std::string strTestName, float fCentration, float fMol, float fTotalPa, float fIteamPa);

	void scanVerify(int nfuncode, int nTestMode, bool isSubBaseline, bool isSmooth, float fPeakPosition
		, std::string strTestName, float fCentration, float fMol, float fTotalPa, float fIteamPa);

	void setFuntionCallbackMap(int nfuncode,std::function<void()> callback) {
		functionMap[nfuncode] = callback;	
	}

	
	
	void revMessage(int nfuncode);

};

