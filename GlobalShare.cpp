#include "GlobalShare.h"

// --- �ڴ˴����� GlobalShare.h �����������о�̬��Ա���� ---

// Ϊ���������ṩһ����ʼֵ
int                GlobalShare::g_SerialNm = 0;
bool               GlobalShare::b_TestMode = false;
bool               GlobalShare::b_startService = false;

// Ϊ���ӵ�STL���������ṩ����
std::map<int, bool>			        GlobalShare::g_map_IndexBoolValue;
std::map<int, std::string>	        GlobalShare::g_map_IndexStringValue;
std::map<int, int>			        GlobalShare::g_map_IndexIntValue;
std::map<int, float>		        GlobalShare::g_map_IndexFloatValue;
std::map<int, double>		        GlobalShare::g_map_IndexDouValue;

std::map<int, std::vector<double>>			GlobalShare::g_map_IndexVectorDouValue;
std::map<int, std::vector<std::string>>		GlobalShare::g_map_IndexVectorStrValue;
std::map<int, std::vector<int>>				GlobalShare::g_map_IndexVectorIntValue;
std::map<int, std::vector<bool>>			GlobalShare::g_map_IndexVectorBoolValue;
std::map<int, std::vector<float>>			GlobalShare::g_map_IndexVectorFloatValue;