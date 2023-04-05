#include "stdafx.h"
#include "Preprocess.h"
#include <iostream>
#include "CTabControlDlg.h"

std::vector<std::string> GeometryReader::Tokenizestring(std::string s, std::string delimiter) {
	size_t pos = 0;
	std::string token;
	std::vector<std::string>vecResultStr;

	if ((pos = s.find("\n")) != std::string::npos) {
		s.erase(pos, pos + 2);
	}

	while ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);
		if (!token.empty())
			vecResultStr.push_back(token);
		s.erase(0, pos + delimiter.length());
	}
	vecResultStr.push_back(s);
	return vecResultStr;
}

void KeyFileReader::Read() {
	std::string myText;
	std::ifstream MyReadFile(m_filePath);

	bool readElem = false;
	bool readNode = false;
	while (getline(MyReadFile, myText)) {
		if (myText.find("ELEMENT_SHELL") != -1) {
			readElem = true;
			continue;
		}
		if (myText.find("NODE") != -1) {
			readNode = true;
			continue;
		}
		if (readElem && (myText.find("*") != -1 || myText.find("$") != -1)) {
			readElem = false;
			readNode = false;
		}
		if (readNode) {
			int nodeID = stoi(myText.substr(0, 8));
			int cmp = 0;
			for (int idx = 8; idx < myText.length(); idx += 16) {
				m_nodes[nodeID].vec[cmp++] = stof(myText.substr(idx, 16));
			}

		}
		else if (readElem) {
			int elemID = stoi(myText.substr(0, 8));
			int bodyID = stoi(myText.substr(8, 16));
			m_bodies[bodyID].push_back(elemID);
			for (int idx = 16; idx < myText.length(); idx += 8) {
				m_elems[elemID].push_back(stoi(myText.substr(idx, 8)));
			}
		}
	}
}

void STLReader::Read() {
	std::vector<Vector3f> vertices;
	std::vector<std::vector<int>>faces;
	std::map<int, std::vector<int>>bodies;
	std::vector<float> vFacetNormal;
	std::vector<Vector3f> vFacetVertices;
	bool bInsideLoop = false;
	int iInsideLoopCounter = 0;
	int iVertexCounter = 0;
	int iCurBody = 0;

	std::string myText;
	std::ifstream MyReadFile(m_filePath);

	bool readElem = false;
	bool readNode = false;
	int line = 1;
	while (getline(MyReadFile, myText)) {
		auto data = Tokenizestring(myText, " ");
		if (data.size() == 0) {
			line++;
			continue;
		}
		
		if (data[0] == "solid") {
			iCurBody++;
			line++;
			continue;
		}
		if (data[0] == "facet" && data[1] == "normal") {
			if (data.size() != 5 || bInsideLoop || iInsideLoopCounter) {
				CString error; error.Format(_T("This mesh is corrupted at line %d"), line);
				throw MLError(error);
			}
			for (int i = 2; i < 5; i++) {
				vFacetNormal.push_back(stof(data[i]));
			}
			line++;
			continue;
		}
		if (myText.find("outer loop") != -1) {
			if (bInsideLoop || iInsideLoopCounter || vFacetNormal.size() == 0) {
				CString error; error.Format(_T("This mesh is corrupted at line %d"), line);
				throw MLError(error);
			}
			bInsideLoop = true;
			line++;
			continue;
		}
		if (myText.find("endloop") != -1) {
			if (iInsideLoopCounter < 3 || iInsideLoopCounter > 4 || !bInsideLoop || vFacetNormal.size() == 0) {
				CString error; error.Format(_T("This mesh is corrupted at line %d"), line);
				throw MLError(error);
			}
			bInsideLoop = false;
			line++;
			continue;
		}
		if (data[0] == "vertex") {
			if (!bInsideLoop && data.size() != 4) {
				CString error; error.Format(_T("This mesh is corrupted"), line);
				throw MLError(error);
			}
			Vector3f buff;
			for (int i = 0; i < 3; i++) {
				buff.vec[i] = (stof(data[i+1]));
			}
			vFacetVertices.push_back(buff);
			iInsideLoopCounter++;
			iVertexCounter++;
			line++;
			continue;
		}
		if (myText.find("endfacet") != -1) {
			if (vFacetNormal.size() == 0 || bInsideLoop || iInsideLoopCounter < 3 || iInsideLoopCounter > 4) {
				CString error; error.Format(_T("This mesh is corrupted at line %d"), line);
				throw MLError(error);
			}
			vertices.insert(vertices.end(), vFacetVertices.begin(), vFacetVertices.end());
			Vector3f a = vFacetVertices[1] - vFacetVertices[0];
			Vector3f b = vFacetVertices[2] - vFacetVertices[0];
			Vector3f normal = a.cross(b);
			normal.Normalize();
			float dot = normal.vec[0] * vFacetNormal[0] + normal.vec[1] * vFacetNormal[1]
				+ normal.vec[2] * vFacetNormal[2];
			std::vector<int>tmp;
			if (dot > 0) {
				for (int vertexIdx = iVertexCounter - iInsideLoopCounter; vertexIdx < iVertexCounter; vertexIdx++)
					tmp.push_back(vertexIdx);
			}
			else {
				for (int vertexIdx = iVertexCounter - 1; vertexIdx < iVertexCounter-iInsideLoopCounter-1; vertexIdx--)
					tmp.push_back(vertexIdx);
			}
			if (iCurBody != 0)
				bodies[iCurBody].push_back(faces.size());
			faces.push_back(tmp);
			vFacetNormal.clear();
			iInsideLoopCounter = 0;
			vFacetVertices.clear();
			line++;
			continue;
		}
		line++;
	}
	
	std::unordered_map<int, int>mapNewNode;
	for (int i = 0; i < vertices.size(); i++) {
		int iExistIdx = -1;
		for (auto itr = m_nodes.begin(); itr != m_nodes.end(); itr++) {
			if ((vertices[i] - itr->second).Length() < m_fTol) {
				iExistIdx = itr->first;
				break;
			}
		}
		if (iExistIdx == -1) {
			mapNewNode[i] = m_nodes.size();
			m_nodes[m_nodes.size()] = vertices[i];
		}
		else {
			mapNewNode[i] = iExistIdx;
		}
	}
	std::map<int, int>mapNewElem;
	for (int i = 0; i < faces.size(); i++) {
		std::vector<int>tmp;
		for (int j = 0; j < faces[i].size(); j++) {
			tmp.push_back(mapNewNode[faces[i][j]]);
		}
		mapNewElem[i] = m_elems.size();
		m_elems[m_elems.size()] = tmp;
	}
	for (auto itr = bodies.begin(); itr != bodies.end(); itr++) {
		for (int i = 0; i < itr->second.size(); i++) {
			m_bodies[itr->first].push_back(mapNewElem[itr->second[i]]);
		}
	}
}

bool DataWriter::WriteSectionDataXlsx(std::string path, std::string section, int X_index, int Y_index, const std::vector<std::vector<Vector3f>>& data)
{
	SimpleXlsx::CWorkbook book("me");
	for (int i = 0; i < data.size(); i++) {
		SimpleXlsx::CWorksheet& sheet = book.AddSheet(section + "_" + std::to_string(i));
		sheet.BeginRow();
		sheet.AddCell("X");
		sheet.AddCell("Y");
		sheet.AddCell("Z");
		sheet.EndRow();

		for (auto& coord : data[i]) {
			sheet.BeginRow();
			sheet.AddCell(coord.vec[0]);
			sheet.AddCell(coord.vec[1]);
			sheet.AddCell(coord.vec[2]);
			sheet.EndRow();
		}
		SimpleXlsx::CChart::Series ser;
		// adding chart to the workbook the reference to a newly created object is returned
		SimpleXlsx::CChart& chart = book.AddChart(sheet, SimpleXlsx::DrawingPoint(7, 3), SimpleXlsx::DrawingPoint(14, 20));

		// for scatter charts it is obligatory to specify both category (X axis) and values (Y axis) sequences
		ser.catSheet = &sheet;
		ser.catAxisFrom = SimpleXlsx::CellCoord(2, X_index);
		ser.catAxisTo = SimpleXlsx::CellCoord(data[i].size() + 1, X_index);

		ser.valSheet = &sheet;
		ser.valAxisFrom = SimpleXlsx::CellCoord(2, Y_index);
		ser.valAxisTo = SimpleXlsx::CellCoord(data[i].size() + 1, Y_index);

		ser.JoinType = SimpleXlsx::CChart::Series::joinSmooth;  // determines whether series will be a smoothed or straight-lined curve
		ser.Marker.Type = SimpleXlsx::CChart::Series::symNone;   // if true add diamond marks in each node of the sequence set

		// add series into the chart (you can add as many series as you wish into the same chart)
		//ser.title = "Log Line";
		chart.AddSeries(ser);

		ser.JoinType = SimpleXlsx::CChart::Series::joinNone;
		ser.Marker.Type = SimpleXlsx::CChart::Series::symCircle;
		ser.Marker.Size = 3;
		ser.Marker.LineWidth = 1.;

		// add series into the chart (you can add as many series as you wish into the same chart)
		//ser.title = "Log Mark";
		chart.AddSeries(ser);
	}
	if (book.Save(path + "/" + section + ".xlsx"))
		return true;
	else
		return false;
}

void DataWriter::WriteSectionData(std::string path, const std::vector<std::vector<float>>& data) {
	std::ofstream myfile;
	myfile.open(path);

	myfile << "X,Y,Z\n";
	for (int i = 0; i < data.size(); i++) {
		myfile << std::to_string(data[i][0]) << "," << std::to_string(data[i][1]) << "," << std::to_string(data[i][2]) << "\n";
	}
	myfile.close();
}

std::vector<std::vector<Vector3f>> SectionDataGenerator::GetPointOnSectionX(float pos_x) {
	std::vector<std::vector<Vector3f>>intersectLine;
	for (auto& elemID : m_reader->m_bodies[1]) {
		if (m_reader->m_elemRange[elemID].min_x > pos_x || pos_x > m_reader->m_elemRange[elemID].max_x)
			continue;

		std::vector<Vector3f>nodes_line;
		for (int i = 0; i < m_reader->m_elems[elemID].size(); i++) {
			auto coord1 = m_reader->m_nodes[m_reader->m_elems[elemID][i % m_reader->m_elems[elemID].size()]];
			auto coord2 = m_reader->m_nodes[m_reader->m_elems[elemID][(i + 1) % m_reader->m_elems[elemID].size()]];
			Vector3f u1;
			u1 = coord2 - coord1;
			if (u1.vec[0] == 0)
				continue;
			float t = (pos_x - coord1.vec[0]) / u1.vec[0];
			Vector3f buff;
			buff.vec[0] = pos_x;
			buff.vec[1] = coord1.vec[1] + u1.vec[1] * t;
			buff.vec[2] = coord1.vec[2] + u1.vec[2] * t;

			float a = (coord1 - coord2).Length();
			float b = (coord1 - buff).Length();
			float c = (coord2 - buff).Length();
			if (abs(b + c - a) < 1e-4) {
				nodes_line.push_back(buff);
			}
		}
		intersectLine.push_back(nodes_line);
	}
	return intersectLine;
}

std::vector<std::vector<Vector3f>> SectionDataGenerator::GetPointOnSectionY(float pos_y) {
	std::vector<std::vector<Vector3f>>intersectLine;
	for (auto& elemID : m_reader->m_bodies[1]) {
		if (m_reader->m_elemRange[elemID].min_y > pos_y || pos_y > m_reader->m_elemRange[elemID].max_y)
			continue;

		std::vector<Vector3f>nodes_line;
		for (int i = 0; i < m_reader->m_elems[elemID].size(); i++) {
			auto coord1 = m_reader->m_nodes[m_reader->m_elems[elemID][i % m_reader->m_elems[elemID].size()]];
			auto coord2 = m_reader->m_nodes[m_reader->m_elems[elemID][(i + 1) % m_reader->m_elems[elemID].size()]];
			Vector3f u1;
			u1 = coord2 - coord1;
			if (u1.vec[1] == 0)
				continue;
			float t = (pos_y - coord1.vec[1]) / u1.vec[1];
			Vector3f buff;
			buff.vec[0] = coord1.vec[0] + u1.vec[0] * t;
			buff.vec[1] = pos_y;
			buff.vec[2] = coord1.vec[2] + u1.vec[2] * t;

			float a = (coord1 - coord2).Length();
			float b = (coord1 - buff).Length();
			float c = (coord2 - buff).Length();
			if (abs(b + c - a) < 1e-4) {
				nodes_line.push_back(buff);
			}
		}
		intersectLine.push_back(nodes_line);
	}
	return intersectLine;
}

std::vector<std::vector<Vector3f>> SectionDataGenerator::GetPointOnSectionZ(float pos_z) {
	std::vector<std::vector<Vector3f>>intersectLine;
	for (auto& elemID : m_reader->m_bodies[1]) {
		if (m_reader->m_elemRange[elemID].min_z > pos_z || pos_z > m_reader->m_elemRange[elemID].max_z)
			continue;

		std::vector<Vector3f>nodes_line;
		for (int i = 0; i < m_reader->m_elems[elemID].size(); i++) {
			auto coord1 = m_reader->m_nodes[m_reader->m_elems[elemID][i % m_reader->m_elems[elemID].size()]];
			auto coord2 = m_reader->m_nodes[m_reader->m_elems[elemID][(i + 1) % m_reader->m_elems[elemID].size()]];
			Vector3f u1;
			u1 = coord2 - coord1;
			if (u1.vec[2] == 0)
				continue;
			float t = (pos_z - coord1.vec[2]) / u1.vec[2];
			Vector3f buff;
			buff.vec[0] = coord1.vec[0] + u1.vec[0] * t;
			buff.vec[1] = coord1.vec[1] + u1.vec[1] * t;
			buff.vec[2] = pos_z;

			float a = (coord1 - coord2).Length();
			float b = (coord1 - buff).Length();
			float c = (coord2 - buff).Length();
			if (abs(b + c - a) < 1e-4) {
				nodes_line.push_back(buff);
			}
		}
		intersectLine.push_back(nodes_line);
	}
	return intersectLine;
}

std::vector<Vector3f> SectionDataGenerator::GetOrderList(std::vector<std::vector<Vector3f>>& intersectLineData) {
	std::vector<Vector3f>intersectPointsInOrder;
	intersectPointsInOrder.push_back(intersectLineData[0][0]);
	intersectPointsInOrder.push_back(intersectLineData[0][1]);
	intersectLineData.erase(intersectLineData.begin());
	while (intersectLineData.size() > 0) {
		auto head = intersectPointsInOrder[0];
		auto tail = intersectPointsInOrder[intersectPointsInOrder.size() - 1];
		for (int i = 0; i < intersectLineData.size(); i++) {
			if ((head - intersectLineData[i][0]).Length() < 1e-4) {
				intersectPointsInOrder.insert(intersectPointsInOrder.begin(), intersectLineData[i][1]);
				intersectLineData.erase(intersectLineData.begin() + i);
				break;
			}
			else if ((head - intersectLineData[i][1]).Length() < 1e-4) {
				intersectPointsInOrder.insert(intersectPointsInOrder.begin(), intersectLineData[i][0]);
				intersectLineData.erase(intersectLineData.begin() + i);
				break;
			}
			else if ((tail - intersectLineData[i][0]).Length() < 1e-4) {
				intersectPointsInOrder.push_back(intersectLineData[i][1]);
				intersectLineData.erase(intersectLineData.begin() + i);
				break;
			}
			else if ((tail- intersectLineData[i][1]).Length() < 1e-4) {
				intersectPointsInOrder.push_back(intersectLineData[i][0]);
				intersectLineData.erase(intersectLineData.begin() + i);
				break;
			}
		}
	}
	return intersectPointsInOrder;
}

void SectionDataGenerator::Generate()
{
	GeomRange range;
	
	for (auto& elemID : m_reader->m_bodies[1]) {
		GeomRange tmp;
		for (auto& nodeID : m_reader->m_elems[elemID]) {
			if (m_reader->m_nodes[nodeID].vec[0] > tmp.max_x)
				tmp.max_x = m_reader->m_nodes[nodeID].vec[0];
			if (m_reader->m_nodes[nodeID].vec[0] < tmp.min_x)
				tmp.min_x = m_reader->m_nodes[nodeID].vec[0];
			if (m_reader->m_nodes[nodeID].vec[1] > tmp.max_y)
				tmp.max_y = m_reader->m_nodes[nodeID].vec[1];
			if (m_reader->m_nodes[nodeID].vec[1] < tmp.min_y)
				tmp.min_y = m_reader->m_nodes[nodeID].vec[1];
			if (m_reader->m_nodes[nodeID].vec[2] > tmp.max_z)
				tmp.max_z = m_reader->m_nodes[nodeID].vec[2];
			if (m_reader->m_nodes[nodeID].vec[2] < tmp.min_z)
				tmp.min_z = m_reader->m_nodes[nodeID].vec[2];

			if (m_reader->m_nodes[nodeID].vec[0] > range.max_x)
				range.max_x = m_reader->m_nodes[nodeID].vec[0];
			if (m_reader->m_nodes[nodeID].vec[0] < range.min_x)
				range.min_x = m_reader->m_nodes[nodeID].vec[0];
			if (m_reader->m_nodes[nodeID].vec[1] > range.max_y)
				range.max_y = m_reader->m_nodes[nodeID].vec[1];
			if (m_reader->m_nodes[nodeID].vec[1] < range.min_y)
				range.min_y = m_reader->m_nodes[nodeID].vec[1];
			if (m_reader->m_nodes[nodeID].vec[2] > range.max_z)
				range.max_z = m_reader->m_nodes[nodeID].vec[2];
			if (m_reader->m_nodes[nodeID].vec[2] < range.min_z)
				range.min_z = m_reader->m_nodes[nodeID].vec[2];
		}
		m_reader->m_elemRange[elemID] = tmp;
	}
	
	
	
	if (m_opt.bCutByX) {
		std::vector<std::vector<Vector3f>>intersect_x_div;
		intersect_x_div = GetPointOnSectionX(m_opt.X);
		intersect_x_order.push_back(GetOrderList(intersect_x_div));
	}
	if (m_opt.bCutByY) {
		std::vector<std::vector<Vector3f>>intersect_y_div;
		intersect_y_div = GetPointOnSectionY(m_opt.Y);
		intersect_y_order.push_back(GetOrderList(intersect_y_div));
	}
	if (m_opt.bCutByZ) {
		std::vector<std::vector<Vector3f>>intersect_z_div;
		intersect_z_div = GetPointOnSectionZ(m_opt.Z);
		intersect_z_order.push_back(GetOrderList(intersect_z_div));
	}
	
}

DataPreprocessing::DataPreprocessing(std::string Path) {
	m_reader = std::shared_ptr<GeometryReader>(ReaderFactory::CreateReader(Path, STL));
}

bool DataPreprocessing::Preprocessing() {
	if (!m_reader) {
		return false;
	}
	m_dlg->m_log.AddString(_T("Reading Geometry file..."));
	m_dlg->m_log.UpdateWindow();

	m_reader->Read();

	m_dlg->m_log.AddString(_T("Reading Geometry file: Done!"));
	m_dlg->m_log.UpdateWindow();

	return true;
}