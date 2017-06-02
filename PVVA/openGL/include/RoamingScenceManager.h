#ifndef ROAMINGSCENCEMANAGER_H
#define ROAMINGSCENCEMANAGER_H
#include <fstream>
using namespace std;
class Vector3D;
class Position3D;
class RoamingScenceManager
{
public:
	RoamingScenceManager();
	~RoamingScenceManager();

	void init();
	void render();
	void executeRotateOperation(int x, int y);
	void executeScaleOperation(float factor);
	void executeTranslateOperation(int x, int y);
	void getInitPos(int x, int y);

	void drawString(const char* str);
	void selectFont(int size, int charset, const char* face);

	void save(ofstream & saveFile) const;
	void open(ifstream & openFile);

	void setView(int NumView);

private:
	//辅助坐标系三根轴
	Vector3D *AuxX;
	Vector3D *AuxY;
	Vector3D *AuxZ;

	//旋转后观察点方向与视线向上方向
	Vector3D*NewEye;
	Vector3D*NewUp;
	Vector3D *NewView;

	Position3D *OldMouse;
	Position3D *Mouse;

	Vector3D *TempTranslateVec;
	float TempscaleFactor;

	// ASCII字符总共只有0到127，一共128种字符
	const int MAX_CHAR = 128;

	int ID_COORDINATY;
	void init_CoordinaryDisplayList();
	void axis(double length); // 坐标轴
};

#endif // ROAMINGSCENCEMANAGER_H
