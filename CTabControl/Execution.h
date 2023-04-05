#pragma once

#include "Preprocess.h"
#include <filesystem>

class CCTabControlDlg;

class CCmdGeomClassification {
private:
	CCTabControlDlg* m_dlg{ nullptr };
public:
	CCmdGeomClassification(DirectoryOpt dirOpt, CutPlaneOpt hyperParamOpt, DataPreprocessing* pre=nullptr);
	bool Execute();
	void SetActiveDlg(CCTabControlDlg* dlg) {
		m_dlg = dlg;
	}
private:
	DirectoryOpt m_dirOpt;
	CutPlaneOpt m_CutPlaneOpt;
	DataPreprocessing* m_PreprocessData;
};