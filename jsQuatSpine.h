#pragma once
#include "common.h"

class JSQuatSpine : public MPxNode
{
public:
	JSQuatSpine();
	virtual ~JSQuatSpine();
	//virtual void postConstructor();
	static void* creator();
	static MStatus initialize();

	virtual MStatus compute(const MPlug &plug, MDataBlock &data);

	static MObject aBoneAxis;
	static MObject aRotateOrder;
	static MObject aInputCurve;
	static MObject aNumberBones;
	static MObject aSpineLength;

	static MObject aStretchRamp;
	static MObject aSquashRamp;
	static MObject aTwistRamp;

	static MObject aStretchAmount;
	static MObject aSquashAmount;
	static MObject aExtraTwist;
	static MObject aControlClusterOffset;

	static MObject aTopMatrix;
	static MObject aBotMatrix;

	static MObject aTopRotate;
	static MObject aTopTranslate;
	static MObject aBottomRotate;
	static MObject aBottomTranslate;

	static MObject aMidRotate;
	static MObject aMidTranslate;
	static MObject aMidScale;

	static const char* kName;
	static MTypeId kId;

	// AETEMPLATE
	static const char* aeCommand;

private:
	double kSlerpFloor;
};
