#pragma once
#include <string>
#include <fstream> 
#include <vector>
#include <unordered_map>
#include "Options.h"
#include <Xlsx/Chart.h>
#include <Xlsx/Chartsheet.h>
#include <Xlsx/Workbook.h>
#include <XLSXColors/XLSXColorLib.h>
#include "ErrorHandler.h"

class CCTabControlDlg;

enum FILETYPE {
	KEYWORD = 0,
	NASTRAN = 1,
	STL = 2,
};

struct GeomRange {
	float max_x = -FLT_MAX;
	float min_x = FLT_MAX;
	float max_y = -FLT_MAX;
	float min_y = FLT_MAX;
	float max_z = -FLT_MAX;
	float min_z = FLT_MAX;
};

class GeometryReader {
public:
	std::unordered_map<int, std::vector<int>>m_elems;
	std::unordered_map<int, Vector3f>m_nodes;
	std::unordered_map<int, std::vector<int>>m_bodies;
	std::unordered_map<int, GeomRange> m_elemRange;
	std::string m_filePath;
public:
	GeometryReader(std::string filePath) : m_filePath(filePath) {};

	virtual void Read() = 0;

	std::vector<std::string> Tokenizestring(std::string s, std::string delimiter = ",");
};

class DataWriter {
public:
	static void WriteSectionData(std::string path, const std::vector<std::vector<float>>& data);

	static bool WriteSectionDataXlsx(std::string path, std::string section, int X_index, int Y_index, const std::vector<std::vector<Vector3f>>& data);
};

class KeyFileReader :public GeometryReader {
public:
	KeyFileReader(std::string filePath) :GeometryReader(filePath) {}
	void Read();
};

class STLReader :public GeometryReader {	
private:
	float m_fTol;
public:
	STLReader(std::string filePath, float fTol = 0.001):GeometryReader(filePath), m_fTol(fTol) {}
	void Read();
};

class ReaderFactory {
private:
public:
	static GeometryReader* CreateReader(std::string filePath, FILETYPE e) {
		switch (e)
		{
		case KEYWORD:
			return new KeyFileReader(filePath);
			break;
		case NASTRAN:
			break;
		case STL:
			return new STLReader(filePath);
			break;
		default:
			break;
		}
		return nullptr;
	}
};

class SectionDataGenerator {
public:
	GeometryReader* m_reader;
	CutPlaneOpt m_opt;
	std::vector<std::vector<Vector3f>>intersect_x_order;
	std::vector<std::vector<Vector3f>>intersect_y_order;
	std::vector<std::vector<Vector3f>>intersect_z_order;
public:
	SectionDataGenerator(GeometryReader* reader, CutPlaneOpt opt) :m_reader(reader), m_opt(opt) {}
	~SectionDataGenerator() { delete m_reader; }

	std::vector<std::vector<Vector3f>> GetPointOnSectionX(float pos_x);

	std::vector<std::vector<Vector3f>> GetPointOnSectionY(float pos_y);

	std::vector<std::vector<Vector3f>> GetPointOnSectionZ(float pos_z);

	std::vector<Vector3f> GetOrderList(std::vector<std::vector<Vector3f>>& intersectLineData);

	void Generate();
};

class DataPreprocessing {
private:
	std::shared_ptr<GeometryReader>m_reader;
	CCTabControlDlg* m_dlg{ nullptr };
public:
	DataPreprocessing(std::string Path);

	void SetActiveDlg(CCTabControlDlg* dlg) {
		m_dlg = dlg;
	}

	bool Preprocessing();

	GeometryReader* GetReader() {
		return m_reader.get();
	}
};