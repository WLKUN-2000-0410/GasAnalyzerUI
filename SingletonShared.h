#pragma once
#include <functional>
#include <map>
#include <vector>
#include <string>
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
		, std::string strTestName, float fCentration, float fMol, float fTotalPa, float fIteamPa, 
		const std::string& modelName,const std::vector<std::string>& vecSeriesNames,const std::vector<double>& vecConOrPre);

	/*void scanVerify(int nfuncode, int nTestMode, bool isSubBaseline, bool isSmooth, float fPeakPosition
		, std::string strTestName, float fCentration, float fMol, float fTotalPa, float fIteamPa);*/
	
	void scanVerify(
		int nfuncode,                      // �ص�����ID 
		const std::string& modelName,      // Ҫʹ���ĸ�ģ�͵�����
		const std::string& curveName,      // Ҫ��֤�������ߵ�����
		bool isSubBaseline,                // �Ƿ�ۻ��� 
		float fTotalPa                     // ��ѹ (����, ��ѹ������Ũ��ʱ����)
	);
	void singleScan(int nfuncode, float fStartWave,float fEndWave,float fTime,int nCount,std::string strReadMode,float fReadFrequency,float fRate,int nGain,std::string strScanMode,float fStepTime);

	void startScan(int nfuncode, float fStartWave,float fEndWave,float fTime,int nCount, std::string strReadMode, float fReadFrequency, float fRate, int nGain, std::string strScanMode, float fStepTime);

	void stop(int nfuncode);

	void setLaserPwd(int nfuncode,int nLaserPwd);

	void correctionData(int nfuncide, std::vector<double> waveVec, std::vector<double> pixVec);

	void setFuntionCallbackMap(int nfuncode,std::function<void()> callback) {
		functionMap[nfuncode] = callback;	
	}

	void revMessage(int nfuncode);

};

