#pragma once

class CMyOpenGLCtrl : public COpenGLCtrl
{
public:
	// 6 file STL cho robot RRP 3-DOF
	STRUCT_OBJECTGLLIST m_objLink0;          // BASE
	STRUCT_OBJECTGLLIST m_objLink1;          // LINK1 beam (thanh ngang cam)
	STRUCT_OBJECTGLLIST m_objRail;           // RAIL (cột xanh dương)
	STRUCT_OBJECTGLLIST m_objCarriage;       // CARRIAGE (bộ trượt xanh lá)
	STRUCT_OBJECTGLLIST m_objLink3;          // LINK3 (thanh tím)
	STRUCT_OBJECTGLLIST m_objEndEffector;    // END-EFFECTOR (vàng)
	
	STRUCT_OBJECTGLLIST m_objTrajectory;
	STRUCT_OBJECTGLLIST m_objWorkSpace;

	GL_LIGHT m_glLight;
	CRobot* m_pRobot;

	void DrawInOpenGL();
	void LoadRobotModel();
	void DrawTrajectory();
	void DrawWorkSpace();

private:
	void LoadModel(const CString& sfilename, STRUCT_OBJECTGLLIST* pObjectDraw);
};