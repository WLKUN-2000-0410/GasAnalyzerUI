#pragma once
#include <map>
#include <vector>
enum  BussiNumEnum
{
	BUSS_MODELING = 1,
	BUSS_VERIFY = 2
};
class GlobalShare
{
public:
	static int                g_SerialNm;            //Î¨Ò»Á÷Ë®ºÅ
	static bool               b_TestMode;
	static bool               b_startService;

	static  std::map<int, bool>			g_map_IndexBoolValue;
	static  std::map<int, std::string>	g_map_IndexStringValue;
	static  std::map<int, int>			g_map_IndexIntValue;
	static  std::map<int, float>		g_map_IndexFloatValue;
	static  std::map<int, double>		g_map_IndexDouValue;


	static  std::map<int, std::vector<double>>			g_map_IndexVectorDouValue;
	static  std::map<int, std::vector<std::string>>		g_map_IndexVectorStrValue;
	static  std::map<int, std::vector<int>>				g_map_IndexVectorIntValue;
	static  std::map<int, std::vector<bool>>			g_map_IndexVectorBoolValue;
	static  std::map<int, std::vector<float>>			g_map_IndexVectorFloatValue;
	
	
};

