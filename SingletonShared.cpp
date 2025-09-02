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
	//������Ҫ�Ķ�����Ϣ
	//1.��Ҫ�ж�����������Ũ�������Ƿ�ƥ��
	if (vecSeriesNames.size() != vecConOrPre.size()) {
	//std::cerr << "����: �������������������Ũ��/��ѹֵ������ƥ��!" << std::endl;
	revMessage(nfuncode);
	return;
}
	
	//2.��ʼ��ȫ�ֹ�����
	GlobalShare::g_map_IndexBoolValue[0] = false; // ��ʼ���ɹ���־Ϊ false
	GlobalShare::g_map_IndexDouValue[0] = 0.0;		// ��ʼб��
	GlobalShare::g_map_IndexDouValue[1] = 0.0;		// ��ʼ�ؾ�
	GlobalShare::g_map_IndexDouValue[2] = 0.0;		// ��ʼ������λ��
	GlobalShare::g_map_IndexStringValue[0] = "";	// ��ʼģ������
	GlobalShare::g_map_IndexStringValue[1] = "";	// ��ʼ��������
	GlobalShare::g_map_IndexStringValue[2] = "";	// ��ʼ����ʱ��
	GlobalShare::g_map_IndexStringValue[3] = "";	// ��ʼģ������
	GlobalShare::g_map_IndexVectorStrValue[0] = {};	// ��ʼ�����������������б�


	//=============================1.�����������֣���ȡ��������=============================
	//�洢ѡ�е���������ָ��
	std::vector<const CurveData*> selectedCurves;

	// ��ȡ���߹������ĵ���ʵ��
	CurveDataManager& curveManager = CurveDataManager::GetInstance();

	// ����ǰ�˴����������������
	for (const std::string& name : vecSeriesNames)
	{
		// �������Ʋ�������
		const CurveData* curve = curveManager.getCurveByName(name);

		// ����Ƿ��ҵ�������
		if (curve != nullptr)
		{
			// ����ҵ��ˣ��ͽ�ָ����������ݵ�ָ�������
			selectedCurves.push_back(curve);
			
		}
		else
		{
			// ���û�ҵ�����ӡһ��������Ϣ
			revMessage(nfuncode);
			return;
		}
	}

	if (selectedCurves.size() != vecSeriesNames.size()) {
		//std::cerr << "����: ������������δ�ܳɹ���ȡ����ģ��ֹ��" << std::endl;
		revMessage(nfuncode);
		return;
	}

	//=================================2.�ۻ���===========================================

	
	//1.���ÿۻ����㷨--�õ���ǿ��Ũ�ȵ�vector
	//1.1���ܲ���-- ������λ�ã��Ƿ�ۻ��ߣ�Ũ��
	bool isNeedSubBaseline = isSubBaseline;
	float PeakPosition = fPeakPosition;
	

	//1.2�ۻ���
	std::vector<DataPoint> modelingDataPoints;
	std::vector<CurveData> processedCurvesToArchive;
	if(isNeedSubBaseline==true)
	{
		//�ۻ��� -- �õ���ǿ
		//��д��һ������ִ�пۻ��ߣ��÷�ǿ���߼��� �ٸ������������ѯ�ۻ��ߣ��÷�ǿ


		//1.���ڴ洢�ۻ��ߴ����Ľ����Ϊ��һ����ģ��׼��
		
		// ��������ѡ�е�����
		for (size_t i = 0; i < selectedCurves.size(); ++i)
		{
			const CurveData* currentCurve = selectedCurves[i];

			// ׼�����ú������������
			std::vector<double> corrected_intensity; // ���ڽ���У�����Y������
			double optimal_fc = 0.0;                 // ���ڽ����ҵ�������fc����
			PeakInfo peak_info;                      // ���ڽ����ҵ��ķ���Ϣ


			// ���þ�̬������
			bool success = SpectrumProcessor::correctBaselineAndFindPeak(
				currentCurve->x_values,      // ���׵�X�� (wavenumbers)
				currentCurve->y_values,      // ���׵�Y�� (intensities)
				fPeakPosition,               // Ŀ���λ (��UI����)
				corrected_intensity,         // [���] У�������
				optimal_fc,                  // [���] ����fc
				peak_info                    // [���] ����Ϣ
			);

			
			if (success)
			{
				//std::cout << "  ����ɹ�! �ҵ��ķ�� (��ǿ): " << peak_info.height << std::endl;

				// ��ȡ�뵱ǰ���߶�Ӧ��Ũ��/��ѹֵ
				double concentration = vecConOrPre[i];

				// ʹ�÷�ǿ��Ũ��ֵ���� DataPoint ����
				modelingDataPoints.emplace_back(concentration, peak_info.height, currentCurve->name);

				CurveData processedCurve;
				processedCurve.name = currentCurve->name + "_processed"; // ������������һ��������
				processedCurve.x_values = currentCurve->x_values; // X�ᱣ�ֲ���
				processedCurve.y_values = corrected_intensity;    // Y����У���������
				processedCurvesToArchive.push_back(processedCurve);
			}
			else
			{
				//std::cerr << "  ����ʧ��! δ�ܴ����� '" << currentCurve->name << "' ����ȡ����Ϣ��" << std::endl;
				revMessage(nfuncode);
				return;//��ֹ��ģ���̣���ӡ������Ϣ
			}
		}

	}
	else
	{
		// ��������ѡ�е�����
		for (size_t i = 0; i < selectedCurves.size(); ++i)
		{
			const CurveData* currentCurve = selectedCurves[i];

			// ������������Ƿ���Ч
			if (currentCurve->x_values.empty()) {
			//	std::cerr << "����: ���� '" << currentCurve->name << "' ����Ϊ�գ���������" << std::endl;
				continue; // �������������ߣ�����������һ��
			}

			// --- ������fPeakPosition��ӽ��ĵ������ ---
			double min_diff = std::abs(currentCurve->x_values[0] - fPeakPosition);
			size_t closest_index = 0;

			for (size_t j = 1; j < currentCurve->x_values.size(); ++j) {
				double diff = std::abs(currentCurve->x_values[j] - fPeakPosition);
				if (diff < min_diff) {
					min_diff = diff;
					closest_index = j;
				}
			}

			// ʹ���ҵ���������Yֵ��Ϊ����ǿ��
			double peak_intensity = currentCurve->y_values[closest_index];

			/*std::cout << "  ���� '" << currentCurve->name << "': "
				<< "��X=" << currentCurve->x_values[closest_index]
				<< "�� (��ӽ�" << fPeakPosition << ") "
				<< "�ҵ��ķ�ǿY=" << peak_intensity << std::endl;*/

			// ��ȡ�뵱ǰ���߶�Ӧ��Ũ��/��ѹֵ
			double concentration = vecConOrPre[i];

			// ʹ���ҵ��ķ�ǿ��Ũ��ֵ���� DataPoint ����
			modelingDataPoints.emplace_back(concentration, peak_intensity, currentCurve->name);
		}
	}

	//=====================================3.��ģ=====================================
	
	
	// ����һ��ģ�͹�����ʵ��
	ModelManager& modelManager = ModelManager::GetInstance();
	// ��UI��ȡģ�����ƺ��������� 
	std::time_t now = std::time(0);
	bool modelCreated = false;
	if (nTestMode == 1)
	{
		// ����Ƿ����㹻�����ݵ���н�ģ
		if (modelingDataPoints.size() < 2) {
			//std::cerr << "����: ������Ҫ2�����ݵ���ܽ���Ũ�ȷ���ģ��" << std::endl;
			revMessage(nfuncode);
			return;
		}

		// ����ģ�͹������� createNewModel ����
		modelCreated = modelManager.createNewModel(
			ModelType::Concentration,    // ָ��ģ������ΪŨ�ȷ�
			modelName,                   // ģ�͵�Ψһ����
			strTestName,                 // ����/��Ʒ����
			fPeakPosition,               // ������λ��
			modelingDataPoints           // ���� (Ũ��, ��ǿ) �����ݵ�����
		);

		// ���ģ���Ƿ񴴽��ɹ�
		if (modelCreated) {

				const IGasModel* newModel = modelManager.getModel(modelName);
		
				const ConcentrationModel* concModel = static_cast<const ConcentrationModel*>(newModel);

				//���������ȫ�ֹ�����
				GlobalShare::g_map_IndexBoolValue[0] = true; // д��ɹ���־
				GlobalShare::g_map_IndexDouValue[0] = concModel->getSlope();				// д��б��
				GlobalShare::g_map_IndexDouValue[1] = concModel->getIntercept();			// д��ؾ�
				GlobalShare::g_map_IndexDouValue[2] = concModel->getCharacteristicPeak();   // д��������λ��

				GlobalShare::g_map_IndexStringValue[0] = concModel->getModelName();			// д��ģ������
				GlobalShare::g_map_IndexStringValue[1] = concModel->getGasName();			// д����������
				GlobalShare::g_map_IndexStringValue[2] = concModel->getCreationTimestamp(); // д��ģ�ʹ���ʱ��
				GlobalShare::g_map_IndexStringValue[3] = concModel->getModelType();			// д��ģ������

				GlobalShare::g_map_IndexVectorStrValue[0] = concModel->getParticipatingCurveNames();// д������������������б�

			
		}
		else {
			
			revMessage(nfuncode);
			return;
			//std::cerr << "����: ����ģ�� '" << modelName << "' ʧ�ܡ������������ظ�������ԭ��" << std::endl;
		}

	}
	else {//��ѹ��
		
		// ��ѹ����� y=kx��������1����͹���
		if (modelingDataPoints.empty()) {
			//std::cerr << "����: ��ѹ����ģ������Ҫ1�����ݵ㡣" << std::endl;
			
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
			//���������ȫ�ֹ�����
			GlobalShare::g_map_IndexBoolValue[0] = true; // д��ɹ���־
			GlobalShare::g_map_IndexDouValue[0] = ppModel->getSlope();					// д��б��
			GlobalShare::g_map_IndexDouValue[1] = 0.0;									// д��ؾ࣬��ѹ��û�нؾ࣬д0
			GlobalShare::g_map_IndexDouValue[2] = ppModel->getCharacteristicPeak();     // д��������λ��
			GlobalShare::g_map_IndexStringValue[0] = ppModel->getModelName();			// д��ģ������
			GlobalShare::g_map_IndexStringValue[1] = ppModel->getGasName();				// д����������
			GlobalShare::g_map_IndexStringValue[2] = ppModel->getCreationTimestamp();	// д��ģ�ʹ���ʱ��
			GlobalShare::g_map_IndexStringValue[3] = ppModel->getModelType();			// д��ģ������
			GlobalShare::g_map_IndexVectorStrValue[0] = ppModel->getParticipatingCurveNames();// д������������������б�
		}
		else {
	
			revMessage(nfuncode);
			return;
			//std::cerr << "����: ����ģ�� '" << modelName << "' ʧ�ܡ������������ظ�������ԭ��" << std::endl;
		}

	}

	// --- ������β���裺�鵵 ---
	if (modelCreated) {
	
		// ׼��ԭʼ�������� (��Ҫ�� const CurveData* ת��Ϊ CurveData)
		std::vector<CurveData> rawCurvesToArchive;
		for (const auto* curvePtr : selectedCurves) {
			if (curvePtr) rawCurvesToArchive.push_back(*curvePtr);
		}

		// ���ù鵵������
		if (nTestMode == 0) {
			bool archiveSuccess = ModelArchiveManager::archiveCurves(
				strTestName,                 // ����/��Ʒ����
				"��ѹ��",            // ģ������ ("Ũ�ȷ�" �� "��ѹ��")
				rawCurvesToArchive,      // ԭʼ��������
				processedCurvesToArchive // �������������� (������ۻ��ߣ���Ϊ��)
			);
		}
		else {
			bool archiveSuccess = ModelArchiveManager::archiveCurves(
				strTestName,                 // ����/��Ʒ����
				"Ũ�ȷ�",            // ģ������ ("Ũ�ȷ�" �� "��ѹ��")
				rawCurvesToArchive,      // ԭʼ��������
				processedCurvesToArchive // �������������� (������ۻ��ߣ���Ϊ��)
			);
		}

	}
	else {
		//std::cerr << "����: ����ģ�� '" << modelName << "' ʧ�ܡ�" << std::endl;
		revMessage(nfuncode);
		return;
	}

	
	revMessage(nfuncode);
}

void CSingletonShared::scanVerify(int nfuncode, 
	const std::string& modelName,const std::string& curveName,
	bool isSubBaseline,float fTotalPa)
{						        
	// --- 0. ��ʼ��ȫ���� ---
	GlobalShare::g_map_IndexBoolValue[0] = false; // Ԥ��Ϊʧ��
	GlobalShare::g_map_IndexDouValue.clear();

	// --- 1. ��ȡҪʹ�õ�ģ�� ---
	ModelManager& modelManager = ModelManager::GetInstance();
	const IGasModel* modelToUse = modelManager.getModel(modelName);

	if (modelToUse == nullptr) {
		//std::cerr << "����: ��ModelManager��δ�ҵ���Ϊ '" << modelName << "' ��ģ��!" << std::endl;
		revMessage(nfuncode);
		return;
	}

	// --- 1.1 ��ȡģ�͵�������λ�� ---
	double characteristicPeak = modelToUse->getCharacteristicPeak();
	

	// --- 2. ��ȡ����֤���������� ---
	CurveDataManager& curveManager = CurveDataManager::GetInstance();
	const CurveData* curveToVerify = curveManager.getCurveByName(curveName);

	if (curveToVerify == nullptr) {
		revMessage(nfuncode);
		return;
	}

	// --- 3. ʹ�ô�ģ���л�ȡ�������壬��ȡ��ǿ ---

	double peak_intensity = 0.0; // ���ڴ洢���ջ�õķ�ǿ

	if (isSubBaseline) {
		std::vector<double> corrected_intensity;
		double optimal_fc = 0.0;
		PeakInfo peak_info;

		bool success = SpectrumProcessor::correctBaselineAndFindPeak(
			curveToVerify->x_values,
			curveToVerify->y_values,
			characteristicPeak, // <-- ʹ�ô�ģ���л�ȡ��������
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
		// ���ۻ����߼���ͬ��ʹ�� characteristicPeak
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
	// --- 4. ����ģ�����ͣ�����Ũ�Ȼ��ѹ ---
	double calculated_concentration = 0.0;
	if (modelToUse->getModelType() == "Ũ�ȷ�")
	{
		// Ũ�ȷ��߼�
		// ����Ũ�ȷ������Ǵ���Ĭ�ϵ������ļ��ɣ���ѹ�����ᱻ����
		CalculationContext context;
		calculated_concentration = modelToUse->calculateConcentration(peak_intensity, context);

		GlobalShare::g_map_IndexBoolValue[0] = true; // д��ɹ���־
		GlobalShare::g_map_IndexDouValue[0] = calculated_concentration; // д��������Ũ��ֵ

	}
	else if (modelToUse->getModelType() == "��ѹ��")
	{
		// ���ڷ�ѹ��������ʹ��UI�������ѹ
		CalculationContext context;
		context.totalPressure_Pa = fTotalPa;
		double concentration_ppm = modelToUse->calculateConcentration(peak_intensity, context);

		// ����ppmŨ�ȣ����� mol % �� ��ѹ
		double concentration_mol = concentration_ppm / 10000.0;
		double partial_pressure_Pa = concentration_ppm * 1e-6 * fTotalPa;

		// �����������д��ȫ�ֹ�����
		GlobalShare::g_map_IndexBoolValue[0] = true;
		GlobalShare::g_map_IndexDouValue[0] = concentration_ppm;     // ���1: ppmŨ��
		GlobalShare::g_map_IndexDouValue[1] = concentration_mol;     // ���2: mol %
		GlobalShare::g_map_IndexDouValue[2] = partial_pressure_Pa;   // ���3: ��ѹ
	}
	else
	{
		// δ֪��ģ�����ͣ��������
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
