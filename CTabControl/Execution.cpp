#include "stdafx.h"
#include "Execution.h"
#include "CTabControlDlg.h"
#include "ModelView.h"

std::string GetCurrentDirectory()
{
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	return std::string(buffer).substr(0, pos);
}

CCmdGeomClassification::CCmdGeomClassification(DirectoryOpt dirOpt, CutPlaneOpt cutPlaneOpt, DataPreprocessing* pre)
	: m_dirOpt(dirOpt), m_CutPlaneOpt(cutPlaneOpt), m_PreprocessData(pre)
{

}

std::string CStr2Str(CString str)
{
	CT2CA val(str);
	std::string kast(val);
	return kast;
}

bool CCmdGeomClassification::Execute()
{
	CString curPath = (CString)GetCurrentDirectory().c_str();

	std::string filePath = CStr2Str(m_dirOpt.keywordFile);
	if (!m_PreprocessData) {
		m_PreprocessData = new DataPreprocessing(filePath);
		m_PreprocessData->SetActiveDlg(m_dlg);
		m_PreprocessData->Preprocessing();
	}

	m_dlg->m_log.AddString(_T("Generating section Data..."));
	m_dlg->m_log.UpdateWindow();
	
	auto secGen = new SectionDataGenerator(m_PreprocessData->GetReader(), m_CutPlaneOpt);
	secGen->Generate();

	m_dlg->m_log.AddString(_T("Generating section Data: Done!"));
	m_dlg->m_log.UpdateWindow();

	std::vector <Vertex> sectionVertices;
	std::vector <GLuint> sectionIndexes;
	int nCount = 0;
	if (m_CutPlaneOpt.bCutByX) {
		for (int i = 0; i < secGen->intersect_x_order.size(); i++) {
			for (int j = 0; j < secGen->intersect_x_order[i].size() - 1; j++) {
				sectionVertices.push_back(Vertex{ glm::vec3(
								secGen->intersect_x_order[i][j].vec[0],
								secGen->intersect_x_order[i][j].vec[1],
								secGen->intersect_x_order[i][j].vec[2]) });
				sectionIndexes.push_back(nCount++);
				sectionVertices.push_back(Vertex{ glm::vec3(
								secGen->intersect_x_order[i][j + 1].vec[0],
								secGen->intersect_x_order[i][j + 1].vec[1],
								secGen->intersect_x_order[i][j + 1].vec[2]) });
				sectionIndexes.push_back(nCount);
			}
		}
	}
	
	if (m_CutPlaneOpt.bCutByY) {
		for (int i = 0; i < secGen->intersect_y_order.size(); i++) {
			for (int j = 0; j < secGen->intersect_y_order[i].size() - 1; j++) {
				sectionVertices.push_back(Vertex{ glm::vec3(
								secGen->intersect_y_order[i][j].vec[0],
								secGen->intersect_y_order[i][j].vec[1],
								secGen->intersect_y_order[i][j].vec[2]) });
				sectionIndexes.push_back(nCount++);
				sectionVertices.push_back(Vertex{ glm::vec3(
								secGen->intersect_y_order[i][j + 1].vec[0],
								secGen->intersect_y_order[i][j + 1].vec[1],
								secGen->intersect_y_order[i][j + 1].vec[2]) });
				sectionIndexes.push_back(nCount);
			}
		}
	}
	
	if (m_CutPlaneOpt.bCutByZ) {
		for (int i = 0; i < secGen->intersect_z_order.size(); i++) {
			for (int j = 0; j < secGen->intersect_z_order[i].size() - 1; j++) {
				sectionVertices.push_back(Vertex{ glm::vec3(
								secGen->intersect_z_order[i][j].vec[0],
								secGen->intersect_z_order[i][j].vec[1],
								secGen->intersect_z_order[i][j].vec[2]) });
				sectionIndexes.push_back(nCount++);
				sectionVertices.push_back(Vertex{ glm::vec3(
								secGen->intersect_z_order[i][j + 1].vec[0],
								secGen->intersect_z_order[i][j + 1].vec[1],
								secGen->intersect_z_order[i][j + 1].vec[2]) });
				sectionIndexes.push_back(nCount);
			}
		}
	}
	
	auto view = CModelView::CreateModelView();
	view->m_sectionVertices = std::move(sectionVertices);
	view->m_sectionIndexes = std::move(sectionIndexes);
	
	m_dlg->m_log.AddString(_T("Output Section Data to Logging Directory..."));
	m_dlg->m_log.UpdateWindow();
	std::string secDir = CStr2Str(m_dirOpt.logDir);
	if (m_CutPlaneOpt.bCutByX) {
		if (!DataWriter::WriteSectionDataXlsx(secDir, "X", 1, 2, secGen->intersect_x_order)) {
			m_dlg->m_log.AddString(_T("Output Section X Data to Logging Directory: Save XLSX failed!"));
		}
		else {
			m_dlg->m_log.AddString(_T("Output Section X Data to Logging Directory: Done!"));
		}
	}
	if (m_CutPlaneOpt.bCutByY) {
		if (!DataWriter::WriteSectionDataXlsx(secDir, "Y", 0, 2, secGen->intersect_y_order)) {
			m_dlg->m_log.AddString(_T("Output Section Y Data to Logging Directory: Save XLSX failed!"));
		}
		else {
			m_dlg->m_log.AddString(_T("Output Section Y Data to Logging Directory: Done!"));
		}
	}
	if (m_CutPlaneOpt.bCutByZ) {
		if (!DataWriter::WriteSectionDataXlsx(secDir, "Z", 0, 1, secGen->intersect_y_order)) {
			m_dlg->m_log.AddString(_T("Output Section Z Data to Logging Directory: Save XLSX failed!"));
		}
		else {
			m_dlg->m_log.AddString(_T("Output Section Z Data to Logging Directory: Done!"));
		}
	}
	
	m_dlg->m_log.UpdateWindow();

	return true;

}
