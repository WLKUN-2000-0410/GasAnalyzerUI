#include "SingletonShared.h"
#include "GlobalShare.h"
#include "CurveDataManager.h"
#include "DataProcessor.h"
#include "ModelFitter.h"
#include "ModelManager.h"
#include "ModelArchiveManager.h"
CSingletonShared::CSingletonShared()
{
}


CSingletonShared::~CSingletonShared()
{
}



void CSingletonShared::scanModeling(int nfuncode, int nTestMode, bool isSubBaseline, bool isSmooth, float fPeakPosition, std::string strTestName, float fCentration, float fMol, float fTotalPa, float fIteamPa, const std::string& modelName, const std::vector<std::string>& vecSeriesNames, const std::vector<double>& vecConOrPre)
{
	//其他重要的额外信息
	//1.需要判断曲线数量和浓度数量是否匹配
	if (vecSeriesNames.size() != vecConOrPre.size()) {
	//std::cerr << "错误: 传入的曲线名称数量与浓度/分压值数量不匹配!" << std::endl;
	revMessage(nfuncode);
	return;
}
	
	//2.初始化全局共享区
	GlobalShare::g_map_IndexBoolValue[0] = false; // 初始化成功标志为 false
	GlobalShare::g_map_IndexDouValue[0] = 0.0;		// 初始斜率
	GlobalShare::g_map_IndexDouValue[1] = 0.0;		// 初始截距
	GlobalShare::g_map_IndexDouValue[2] = 0.0;		// 初始特征峰位置
	GlobalShare::g_map_IndexStringValue[0] = "";	// 初始模型名称
	GlobalShare::g_map_IndexStringValue[1] = "";	// 初始气体名称
	GlobalShare::g_map_IndexStringValue[2] = "";	// 初始创建时间
	GlobalShare::g_map_IndexStringValue[3] = "";	// 初始模型类型
	GlobalShare::g_map_IndexVectorStrValue[0] = {};	// 初始参与计算的曲线名称列表


	//=============================1.根据曲线名字，提取曲线数据=============================
	//存储选中的曲线数据指针
	std::vector<const CurveData*> selectedCurves;

	// 获取曲线管理器的单例实例
	CurveDataManager& curveManager = CurveDataManager::GetInstance();

	// 遍历前端传入的所有曲线名称
	for (const std::string& name : vecSeriesNames)
	{
		// 根据名称查找曲线
		const CurveData* curve = curveManager.getCurveByName(name);

		// 检查是否找到了曲线
		if (curve != nullptr)
		{
			// 如果找到了，就将指向该曲线数据的指针存起来
			selectedCurves.push_back(curve);
			
		}
		else
		{
			// 如果没找到，打印一个错误信息
			revMessage(nfuncode);
			return;
		}
	}

	if (selectedCurves.size() != vecSeriesNames.size()) {
		//std::cerr << "错误: 部分曲线数据未能成功获取，建模中止。" << std::endl;
		revMessage(nfuncode);
		return;
	}

	//=================================2.扣基线===========================================

	
	//1.调用扣基线算法--得到峰强和浓度的vector
	//1.1接受参数-- 特征峰位置，是否扣基线，浓度
	bool isNeedSubBaseline = isSubBaseline;
	float PeakPosition = fPeakPosition;
	

	//1.2扣基线
	std::vector<DataPoint> modelingDataPoints;
	std::vector<CurveData> processedCurvesToArchive;
	if(isNeedSubBaseline==true)
	{
		//扣基线 -- 得到峰强
		//先写对一条曲线执行扣基线，得峰强得逻辑， 再改造多条曲线轮询扣基线，得峰强


		//1.用于存储扣基线处理后的结果，为下一步建模做准备
		
		// 遍历所有选中的曲线
		for (size_t i = 0; i < selectedCurves.size(); ++i)
		{
			const CurveData* currentCurve = selectedCurves[i];

			// 准备调用函数的输出参数
			std::vector<double> corrected_intensity; // 用于接收校正后的Y轴数据
			double optimal_fc = 0.0;                 // 用于接收找到的最优fc参数
			PeakInfo peak_info;                      // 用于接收找到的峰信息


			// 调用静态处理函数
			bool success = SpectrumProcessor::correctBaselineAndFindPeak(
				currentCurve->x_values,      // 光谱的X轴 (wavenumbers)
				currentCurve->y_values,      // 光谱的Y轴 (intensities)
				fPeakPosition,               // 目标峰位 (从UI传入)
				corrected_intensity,         // [输出] 校正后光谱
				optimal_fc,                  // [输出] 最优fc
				peak_info                    // [输出] 峰信息
			);

			
			if (success)
			{
				//std::cout << "  处理成功! 找到的峰高 (峰强): " << peak_info.height << std::endl;

				// 获取与当前曲线对应的浓度/分压值
				double concentration = vecConOrPre[i];

				// 使用峰强和浓度值构建 DataPoint 对象
				modelingDataPoints.emplace_back(concentration, peak_info.height, currentCurve->name);

				CurveData processedCurve;
				processedCurve.name = currentCurve->name + "_processed"; // 给处理后的曲线一个新名字
				processedCurve.x_values = currentCurve->x_values; // X轴保持不变
				processedCurve.y_values = corrected_intensity;    // Y轴是校正后的数据
				processedCurvesToArchive.push_back(processedCurve);
			}
			else
			{
				//std::cerr << "  处理失败! 未能从曲线 '" << currentCurve->name << "' 中提取峰信息。" << std::endl;
				revMessage(nfuncode);
				return;//中止建模过程，打印错误信息
			}
		}

	}
	else
	{
		// 遍历所有选中的曲线
		for (size_t i = 0; i < selectedCurves.size(); ++i)
		{
			const CurveData* currentCurve = selectedCurves[i];

			// 检查曲线数据是否有效
			if (currentCurve->x_values.empty()) {
			//	std::cerr << "错误: 曲线 '" << currentCurve->name << "' 数据为空，已跳过。" << std::endl;
				continue; // 跳过这条空曲线，继续处理下一条
			}

			// --- 查找与fPeakPosition最接近的点的索引 ---
			double min_diff = std::abs(currentCurve->x_values[0] - fPeakPosition);
			size_t closest_index = 0;

			for (size_t j = 1; j < currentCurve->x_values.size(); ++j) {
				double diff = std::abs(currentCurve->x_values[j] - fPeakPosition);
				if (diff < min_diff) {
					min_diff = diff;
					closest_index = j;
				}
			}

			// 使用找到的最近点的Y值作为“峰强”
			double peak_intensity = currentCurve->y_values[closest_index];

			/*std::cout << "  曲线 '" << currentCurve->name << "': "
				<< "在X=" << currentCurve->x_values[closest_index]
				<< "处 (最接近" << fPeakPosition << ") "
				<< "找到的峰强Y=" << peak_intensity << std::endl;*/

			// 获取与当前曲线对应的浓度/分压值
			double concentration = vecConOrPre[i];

			// 使用找到的峰强和浓度值构建 DataPoint 对象
			modelingDataPoints.emplace_back(concentration, peak_intensity, currentCurve->name);
		}
	}

	//=====================================3.建模=====================================
	
	
	// 创建一个模型管理器实例
	ModelManager& modelManager = ModelManager::GetInstance();
	// 从UI获取模型名称和气体名称 
	std::time_t now = std::time(0);
	bool modelCreated = false;
	if (nTestMode == 1)
	{
		// 检查是否有足够的数据点进行建模
		if (modelingDataPoints.size() < 2) {
			//std::cerr << "错误: 至少需要2个数据点才能进行浓度法建模。" << std::endl;
			revMessage(nfuncode);
			return;
		}

		// 调用模型管理器的 createNewModel 方法
		modelCreated = modelManager.createNewModel(
			ModelType::Concentration,    // 指定模型类型为浓度法
			modelName,                   // 模型的唯一名称
			strTestName,                 // 气体/样品名称
			fPeakPosition,               // 特征峰位置
			modelingDataPoints           // 包含 (浓度, 峰强) 的数据点向量
		);

		// 检查模型是否创建成功
		if (modelCreated) {

				const IGasModel* newModel = modelManager.getModel(modelName);
		
				const ConcentrationModel* concModel = static_cast<const ConcentrationModel*>(newModel);

				//将结果返回全局共享区
				GlobalShare::g_map_IndexBoolValue[0] = true; // 写入成功标志
				GlobalShare::g_map_IndexDouValue[0] = concModel->getSlope();				// 写入斜率
				GlobalShare::g_map_IndexDouValue[1] = concModel->getIntercept();			// 写入截距
				GlobalShare::g_map_IndexDouValue[2] = concModel->getCharacteristicPeak();   // 写入特征峰位置

				GlobalShare::g_map_IndexStringValue[0] = concModel->getModelName();			// 写入模型名称
				GlobalShare::g_map_IndexStringValue[1] = concModel->getGasName();			// 写入气体名称
				GlobalShare::g_map_IndexStringValue[2] = concModel->getCreationTimestamp(); // 写入模型创建时间
				GlobalShare::g_map_IndexStringValue[3] = concModel->getModelType();			// 写入模型类型

				GlobalShare::g_map_IndexVectorStrValue[0] = concModel->getParticipatingCurveNames();// 写入参与计算的曲线名称列表

			
		}
		else {
			
			revMessage(nfuncode);
			return;
			//std::cerr << "错误: 创建模型 '" << modelName << "' 失败。可能是名称重复或其他原因。" << std::endl;
		}

	}
	else {//分压法
		
		// 分压法拟合 y=kx，理论上1个点就够了
		if (modelingDataPoints.empty()) {
			//std::cerr << "错误: 分压法建模至少需要1个数据点。" << std::endl;
			
			revMessage(nfuncode);
			return;
		}

		modelCreated = modelManager.createNewModel(
			ModelType::PartialPressure,
			modelName, strTestName, fPeakPosition, modelingDataPoints
		);

		if (modelCreated) {
			const IGasModel* newModel = modelManager.getModel(modelName);
			const PartialPressureModel* ppModel = static_cast<const PartialPressureModel*>(newModel);
			//将结果返回全局共享区
			GlobalShare::g_map_IndexBoolValue[0] = true; // 写入成功标志
			GlobalShare::g_map_IndexDouValue[0] = ppModel->getSlope();					// 写入斜率
			GlobalShare::g_map_IndexDouValue[1] = 0.0;									// 写入截距，分压法没有截距，写0
			GlobalShare::g_map_IndexDouValue[2] = ppModel->getCharacteristicPeak();     // 写入特征峰位置
			GlobalShare::g_map_IndexStringValue[0] = ppModel->getModelName();			// 写入模型名称
			GlobalShare::g_map_IndexStringValue[1] = ppModel->getGasName();				// 写入气体名称
			GlobalShare::g_map_IndexStringValue[2] = ppModel->getCreationTimestamp();	// 写入模型创建时间
			GlobalShare::g_map_IndexStringValue[3] = ppModel->getModelType();			// 写入模型类型
			GlobalShare::g_map_IndexVectorStrValue[0] = ppModel->getParticipatingCurveNames();// 写入参与计算的曲线名称列表
		}
		else {
	
			revMessage(nfuncode);
			return;
			//std::cerr << "错误: 创建模型 '" << modelName << "' 失败。可能是名称重复或其他原因。" << std::endl;
		}

	}

	// --- 公共收尾步骤：归档 ---
	if (modelCreated) {
	
		// 准备原始曲线数据 (需要从 const CurveData* 转换为 CurveData)
		std::vector<CurveData> rawCurvesToArchive;
		for (const auto* curvePtr : selectedCurves) {
			if (curvePtr) rawCurvesToArchive.push_back(*curvePtr);
		}

		// 调用归档管理器
		if (nTestMode == 0) {
			bool archiveSuccess = ModelArchiveManager::archiveCurves(
				strTestName,                 // 气体/样品名称
				"分压法",            // 模型类型 ("浓度法" 或 "分压法")
				rawCurvesToArchive,      // 原始曲线数据
				processedCurvesToArchive // 处理后的曲线数据 (如果不扣基线，则为空)
			);
		}
		else {
			bool archiveSuccess = ModelArchiveManager::archiveCurves(
				strTestName,                 // 气体/样品名称
				"浓度法",            // 模型类型 ("浓度法" 或 "分压法")
				rawCurvesToArchive,      // 原始曲线数据
				processedCurvesToArchive // 处理后的曲线数据 (如果不扣基线，则为空)
			);
		}

	}
	else {
		//std::cerr << "错误: 创建模型 '" << modelName << "' 失败。" << std::endl;
		revMessage(nfuncode);
		return;
	}

	
	revMessage(nfuncode);
}

void CSingletonShared::scanVerify(int nfuncode, 
	const std::string& modelName,const std::string& curveName,
	bool isSubBaseline,float fTotalPa)
{						        
	// --- 0. 初始化全局区 ---
	GlobalShare::g_map_IndexBoolValue[0] = false; // 预设为失败
	GlobalShare::g_map_IndexDouValue.clear();

	// --- 1. 获取要使用的模型 ---
	ModelManager& modelManager = ModelManager::GetInstance();
	const IGasModel* modelToUse = modelManager.getModel(modelName);

	if (modelToUse == nullptr) {
		//std::cerr << "错误: 在ModelManager中未找到名为 '" << modelName << "' 的模型!" << std::endl;
		revMessage(nfuncode);
		return;
	}

	// --- 1.1 获取模型的特征峰位置 ---
	double characteristicPeak = modelToUse->getCharacteristicPeak();
	

	// --- 2. 获取待验证的曲线数据 ---
	CurveDataManager& curveManager = CurveDataManager::GetInstance();
	const CurveData* curveToVerify = curveManager.getCurveByName(curveName);

	if (curveToVerify == nullptr) {
		revMessage(nfuncode);
		return;
	}

	// --- 3. 使用从模型中获取的特征峰，提取峰强 ---

	double peak_intensity = 0.0; // 用于存储最终获得的峰强

	if (isSubBaseline) {
		std::vector<double> corrected_intensity;
		double optimal_fc = 0.0;
		PeakInfo peak_info;

		bool success = SpectrumProcessor::correctBaselineAndFindPeak(
			curveToVerify->x_values,
			curveToVerify->y_values,
			characteristicPeak, // <-- 使用从模型中获取的特征峰
			corrected_intensity,
			optimal_fc,
			peak_info
		);

		if (success) {
			peak_intensity = peak_info.height;
		}
		else {
			revMessage(nfuncode);
			return;
		}
	}
	else {
		// 不扣基线逻辑，同样使用 characteristicPeak
		if (curveToVerify->x_values.empty()) {
			revMessage(nfuncode);
			return;
		}
		double min_diff = std::abs(curveToVerify->x_values[0] - characteristicPeak);
		size_t closest_index = 0;
		for (size_t j = 1; j < curveToVerify->x_values.size(); ++j) {
			double diff = std::abs(curveToVerify->x_values[j] - characteristicPeak);
			if (diff < min_diff) {
				min_diff = diff;
				closest_index = j;
			}
		}
		peak_intensity = curveToVerify->y_values[closest_index];
	}
	// --- 4. 根据模型类型，计算浓度或分压 ---
	double calculated_concentration = 0.0;
	if (modelToUse->getModelType() == "浓度法")
	{
		// 浓度法逻辑
		// 对于浓度法，我们创建默认的上下文即可，总压参数会被忽略
		CalculationContext context;
		calculated_concentration = modelToUse->calculateConcentration(peak_intensity, context);

		GlobalShare::g_map_IndexBoolValue[0] = true; // 写入成功标志
		GlobalShare::g_map_IndexDouValue[0] = calculated_concentration; // 写入计算出的浓度值

	}
	else if (modelToUse->getModelType() == "分压法")
	{
		// 对于分压法，必须使用UI传入的总压
		CalculationContext context;
		context.totalPressure_Pa = fTotalPa;
		double concentration_ppm = modelToUse->calculateConcentration(peak_intensity, context);

		// 根据ppm浓度，反算 mol % 和 分压
		double concentration_mol = concentration_ppm / 10000.0;
		double partial_pressure_Pa = concentration_ppm * 1e-6 * fTotalPa;

		// 将三个结果都写入全局共享区
		GlobalShare::g_map_IndexBoolValue[0] = true;
		GlobalShare::g_map_IndexDouValue[0] = concentration_ppm;     // 结果1: ppm浓度
		GlobalShare::g_map_IndexDouValue[1] = concentration_mol;     // 结果2: mol %
		GlobalShare::g_map_IndexDouValue[2] = partial_pressure_Pa;   // 结果3: 分压
	}
	else
	{
		// 未知的模型类型，处理错误
		revMessage(nfuncode);
		return;
	}

	revMessage(nfuncode);
}

void CSingletonShared::singleScan(int nfuncode, float fStartWave, float fEndWave, float fTime, int nCount, std::string strReadMode, float fReadFrequency, float fRate, int nGain, std::string strScanMode, float fStepTime)
{
	revMessage(nfuncode);
}

void CSingletonShared::startScan(int nfuncode, float fStartWave, float fEndWave, float fTime, int nCount, std::string strReadMode, float fReadFrequency, float fRate, int nGain, std::string strScanMode, float fStepTime)
{
	revMessage(nfuncode);
}

void CSingletonShared::stop(int nfuncode)
{
	revMessage(nfuncode);
}

void CSingletonShared::setLaserPwd(int nfuncode, int nLaserPwd)
{

	revMessage(nfuncode);
}

void CSingletonShared::correctionData(int nfuncide, std::vector<double> waveVec, std::vector<double> pixVec)
{
}

void CSingletonShared::revMessage(int nfuncode)
{

	if (functionMap.find(nfuncode)!=functionMap.end())
	{
		functionMap[nfuncode]();
	}
	
}
