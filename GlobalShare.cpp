#include "GlobalShare.h"

// --- 在此处定义 GlobalShare.h 中声明的所有静态成员变量 ---

// 为基础类型提供一个初始值
int                GlobalShare::g_SerialNm = 0;
bool               GlobalShare::b_TestMode = false;
bool               GlobalShare::b_startService = false;

// 为复杂的STL容器类型提供定义
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