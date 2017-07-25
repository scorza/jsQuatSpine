#include "jsQuatSpine.h"
#include "common.h"

#define POSITIVE_X 0
#define POSITIVE_Y 1
#define POSITIVE_Z 2
#define NEGATIVE_X 3
#define NEGATIVE_Y 4
#define NEGATIVE_Z 5

#define kXYZ	0
#define kYZX	1
#define kZXY	2
#define kXZY	3
#define kYXZ	4
#define kZYX	5

MObject JSQuatSpine::aBoneAxis;
MObject JSQuatSpine::aRotateOrder;
MObject JSQuatSpine::aInputCurve;
MObject JSQuatSpine::aNumberBones;
MObject JSQuatSpine::aSpineLength;

MObject JSQuatSpine::aStretchRamp;
MObject JSQuatSpine::aSquashRamp;
MObject JSQuatSpine::aTwistRamp;

MObject JSQuatSpine::aStretchAmount;
MObject JSQuatSpine::aSquashAmount;
MObject JSQuatSpine::aExtraTwist;

MObject JSQuatSpine::aTopMatrix;
MObject JSQuatSpine::aBotMatrix;

MObject JSQuatSpine::aTopRotate;
MObject JSQuatSpine::aTopTranslate;
MObject JSQuatSpine::aBottomRotate;
MObject JSQuatSpine::aBottomTranslate;

MObject JSQuatSpine::aMidRotate;
MObject JSQuatSpine::aMidTranslate;
MObject JSQuatSpine::aMidScale;

const char* JSQuatSpine::kName = "jsQuatSpine";
MTypeId JSQuatSpine::kId(0x00000002);
const char* JSQuatSpine::aeCommand = "global proc AEjsQuatSpineTemplate( string $nodeName )\n{\n\teditorTemplate -beginScrollLayout;\n\n\t\teditorTemplate -beginLayout \"Quat Spine Node Attributes\" -collapse 0;\n\t\t\tAEaddRampControl ( $nodeName + \".stretchRamp\");\n\t\t\tAEaddRampControl ( $nodeName + \".squashRamp\");\n\t\t\tAEaddRampControl ( $nodeName + \".twistRamp\");\n\t\teditorTemplate -endLayout;\n\n\teditorTemplate -addExtraControls;\n\teditorTemplate -endScrollLayout;\n\n\tsetAttr ($nodeName + \".squashRamp[0].squashRamp_FloatValue\") 0.0;\n\tsetAttr ($nodeName + \".squashRamp[0].squashRamp_Position\") 0.0;\n\tsetAttr ($nodeName + \".squashRamp[0].squashRamp_Interp\") 2;\n\n\tsetAttr ($nodeName + \".squashRamp[1].squashRamp_FloatValue\") 1.0;\n\tsetAttr ($nodeName + \".squashRamp[1].squashRamp_Position\") 0.5;\n\tsetAttr ($nodeName + \".squashRamp[1].squashRamp_Interp\") 2;\n\t\n\tsetAttr ($nodeName + \".squashRamp[2].squashRamp_FloatValue\") 0.0;\n\tsetAttr ($nodeName + \".squashRamp[2].squashRamp_Position\") 1.0;\n\tsetAttr ($nodeName + \".squashRamp[2].squashRamp_Interp\") 2;\n\n\tsetAttr ($nodeName + \".stretchRamp[0].stretchRamp_FloatValue\") 0.0;\n\tsetAttr ($nodeName + \".stretchRamp[0].stretchRamp_Position\") 0.0;\n\tsetAttr ($nodeName + \".stretchRamp[0].stretchRamp_Interp\") 2;\n\n\tsetAttr ($nodeName + \".stretchRamp[1].stretchRamp_FloatValue\") 1.0;\n\tsetAttr ($nodeName + \".stretchRamp[1].stretchRamp_Position\") 0.5;\n\tsetAttr ($nodeName + \".stretchRamp[1].stretchRamp_Interp\") 2;\n\t\n\tsetAttr ($nodeName + \".stretchRamp[2].stretchRamp_FloatValue\") 0.0;\n\tsetAttr ($nodeName + \".stretchRamp[2].stretchRamp_Position\") 1.0;\n\tsetAttr ($nodeName + \".stretchRamp[2].stretchRamp_Interp\") 2;\n\n\tsetAttr ($nodeName + \".twistRamp[0].twistRamp_FloatValue\") 0.0;\n\tsetAttr ($nodeName + \".twistRamp[0].twistRamp_Position\") 0.0;\n\tsetAttr ($nodeName + \".twistRamp[0].twistRamp_Interp\") 1;\n\n\tsetAttr ($nodeName + \".twistRamp[1].twistRamp_FloatValue\") 1.0;\n\tsetAttr ($nodeName + \".twistRamp[1].twistRamp_Position\") 1.0;\n\tsetAttr ($nodeName + \".twistRamp[1].twistRamp_Interp\") 1;\n\n}\t\t\t";

JSQuatSpine::JSQuatSpine() :kSlerpFloor(0.0) {}
JSQuatSpine::~JSQuatSpine() {}

//void JSQuatSpine::postConstructor() 
//{
//	MObject oThis = thisMObject();
//	MFnDependencyNode fnDep(oThis);
//	MPlug squashRamp = fnDep.findPlug("squashRamp", false);
//	if (!squashRamp.isNull())
//	{
//		cout << "squashRamp found!" << endl;
//	}
//}

void* JSQuatSpine::creator() { return new JSQuatSpine; }

MStatus JSQuatSpine::initialize()
{
	MStatus status;

	MFnCompoundAttribute fnComp;
	MFnEnumAttribute fnEnum;
	MFnMatrixAttribute fnMatrix;
	MFnNumericAttribute fnNumeric;
	MRampAttribute fnRamp;
	MFnTypedAttribute fnTyped;

	// Enum attributes
	aBoneAxis = fnEnum.create("boneAxis", "boneAxis", POSITIVE_X);
	fnEnum.addField("Positive X", POSITIVE_X);
	fnEnum.addField("Positive Y", POSITIVE_Y);
	fnEnum.addField("Positive Z", POSITIVE_Z);
	fnEnum.addField("Negative X", NEGATIVE_X);
	fnEnum.addField("Negative Y", NEGATIVE_Y);
	fnEnum.addField("Negative Z", NEGATIVE_Z);
	fnEnum.setDefault(0);
	fnEnum.setKeyable(true);
	addAttribute(aBoneAxis);

	aRotateOrder = fnEnum.create("rotateOrder", "rotateOrder", kXYZ);
	fnEnum.addField("XYZ", kXYZ);
	fnEnum.addField("YZX", kYZX);
	fnEnum.addField("ZXY", kZXY);
	fnEnum.addField("XZY", kXZY);
	fnEnum.addField("YZX", kYZX);
	fnEnum.addField("ZYX", kZYX);
	fnEnum.setKeyable(true);
	fnEnum.setDefault(0);
	addAttribute(aRotateOrder);

	// Non angle attributes
	aInputCurve = fnTyped.create("inputCurve", "inputCurve", MFnData::kNurbsCurve);
	fnTyped.setKeyable(true);
	fnTyped.setStorable(true);
	addAttribute(aInputCurve);

	aNumberBones = fnNumeric.create("numberOfBones", "numberOfBones", MFnNumericData::kInt, 13);
	fnNumeric.setKeyable(true);
	addAttribute(aNumberBones);

	aSpineLength = fnNumeric.create("spineLength", "spineLength", MFnNumericData::kDouble, 12.0);
	fnNumeric.setKeyable(true);
	fnNumeric.setStorable(true);
	addAttribute(aSpineLength);

	// Ramp attributes
	aStretchRamp = fnRamp.createCurveRamp("stretchRamp", "stretchRamp");
	addAttribute(aStretchRamp);
	
	aSquashRamp = fnRamp.createCurveRamp("squashRamp", "squashRamp");
	addAttribute(aSquashRamp);
	
	aTwistRamp = fnRamp.createCurveRamp("twistRamp", "twistRamp");
	addAttribute(aTwistRamp);

	// Related attributes
	aStretchAmount = fnNumeric.create("stretchAmount", "stretchAmount", MFnNumericData::kDouble, 2.0);
	fnNumeric.setKeyable(true);
	fnNumeric.setStorable(true);
	addAttribute(aStretchAmount);

	aSquashAmount = fnNumeric.create("squashAmount", "squashAmount", MFnNumericData::kDouble, 2.0);
	fnNumeric.setKeyable(true);
	fnNumeric.setStorable(true);
	addAttribute(aSquashAmount);

	aExtraTwist = fnNumeric.create("twistMultiplier", "twistMultiplier", MFnNumericData::kDouble, 1.0);
	fnNumeric.setKeyable(true);
	fnNumeric.setStorable(true);
	fnNumeric.setMin(1.0);
	addAttribute(aExtraTwist);
	
	// Matrix input attributes
	aTopMatrix = fnMatrix.create("topMatrix", "topMatrix");
	fnMatrix.setKeyable(true);
	fnMatrix.setStorable(true);
	addAttribute(aTopMatrix);

	aBotMatrix = fnMatrix.create("bottomMatrix", "bottomMatrix");
	fnMatrix.setKeyable(true);
	fnMatrix.setStorable(true);
	addAttribute(aBotMatrix);

	// Top rotate output attribute
	aTopRotate = fnNumeric.create("topRotate", "topRotate", MFnNumericData::k3Double, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	fnNumeric.setStorable(false);
	fnNumeric.setWritable(false);
	status = addAttribute(aTopRotate);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// Top translate output attribute
	aTopTranslate = fnNumeric.create("topTranslate", "topTranslate", MFnNumericData::k3Double, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	fnNumeric.setStorable(false);
	fnNumeric.setWritable(false);
	status = addAttribute(aTopTranslate);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// Bottom rotate output attribute
	aBottomRotate = fnNumeric.create("bottomRotate", "bottomRotate", MFnNumericData::k3Double, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	fnNumeric.setStorable(false);
	fnNumeric.setWritable(false);
	status = addAttribute(aBottomRotate);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// Bottom rotate output attribute
	aBottomTranslate = fnNumeric.create("bottomTranslate", "bottomTranslate", MFnNumericData::k3Double, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	fnNumeric.setStorable(false);
	fnNumeric.setWritable(false);
	status = addAttribute(aBottomTranslate);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// Mid rotate output attribute
	aMidRotate = fnNumeric.create("midRotate", "midRotate", MFnNumericData::k3Double, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	fnNumeric.setStorable(false);
	fnNumeric.setWritable(false);
	fnNumeric.setArray(true);
	status = addAttribute(aMidRotate);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// Mid translate output attribute
	aMidTranslate = fnNumeric.create("midTranslate", "midTranslate", MFnNumericData::k3Double, 0.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	fnNumeric.setStorable(false);
	fnNumeric.setWritable(false);
	fnNumeric.setArray(true);
	status = addAttribute(aMidTranslate);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// Mid scale output attribute
	aMidScale = fnNumeric.create("midScale", "midScale", MFnNumericData::k3Double, 1.0, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	fnNumeric.setStorable(false);
	fnNumeric.setWritable(false);
	fnNumeric.setArray(true);
	status = addAttribute(aMidScale);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// attributeAffects()
	attributeAffects(aBoneAxis, aMidRotate);
	attributeAffects(aRotateOrder, aMidRotate);
	attributeAffects(aInputCurve, aMidRotate);
	attributeAffects(aNumberBones, aMidRotate);
	attributeAffects(aSpineLength, aMidRotate);
	attributeAffects(aStretchRamp, aMidRotate);
	attributeAffects(aSquashRamp, aMidRotate);
	attributeAffects(aTwistRamp, aMidRotate);
	attributeAffects(aStretchAmount, aMidRotate);
	attributeAffects(aSquashAmount, aMidRotate);
	attributeAffects(aExtraTwist, aMidRotate);
	attributeAffects(aTopMatrix, aMidRotate);
	attributeAffects(aBotMatrix, aMidRotate);


	attributeAffects(aBoneAxis, aMidTranslate);
	attributeAffects(aRotateOrder, aMidTranslate);
	attributeAffects(aInputCurve, aMidTranslate);
	attributeAffects(aNumberBones, aMidTranslate);
	attributeAffects(aSpineLength, aMidTranslate);
	attributeAffects(aStretchRamp, aMidTranslate);
	attributeAffects(aSquashRamp, aMidTranslate);
	attributeAffects(aTwistRamp, aMidTranslate);
	attributeAffects(aStretchAmount, aMidTranslate);
	attributeAffects(aSquashAmount, aMidTranslate);
	attributeAffects(aExtraTwist, aMidTranslate);
	attributeAffects(aTopMatrix, aMidTranslate);
	attributeAffects(aBotMatrix, aMidTranslate);

	attributeAffects(aBoneAxis, aMidScale);
	attributeAffects(aRotateOrder, aMidScale);
	attributeAffects(aInputCurve, aMidScale);
	attributeAffects(aNumberBones, aMidScale);
	attributeAffects(aSpineLength, aMidScale);
	attributeAffects(aStretchRamp, aMidScale);
	attributeAffects(aSquashRamp, aMidScale);
	attributeAffects(aTwistRamp, aMidScale);
	attributeAffects(aStretchAmount, aMidScale);
	attributeAffects(aSquashAmount, aMidScale);
	attributeAffects(aExtraTwist, aMidScale);
	attributeAffects(aTopMatrix, aMidScale);
	attributeAffects(aBotMatrix, aMidScale);
	
	attributeAffects(aBoneAxis, aTopRotate);
	attributeAffects(aRotateOrder, aTopRotate);
	attributeAffects(aInputCurve, aTopRotate);
	attributeAffects(aNumberBones, aTopRotate);
	attributeAffects(aSpineLength, aTopRotate);
	attributeAffects(aStretchRamp, aTopRotate);
	attributeAffects(aSquashRamp, aTopRotate);
	attributeAffects(aTwistRamp, aTopRotate);
	attributeAffects(aStretchAmount, aTopRotate);
	attributeAffects(aSquashAmount, aTopRotate);
	attributeAffects(aExtraTwist, aTopRotate);
	attributeAffects(aTopMatrix, aTopRotate);
	attributeAffects(aBotMatrix, aTopRotate);

	attributeAffects(aBoneAxis, aTopTranslate);
	attributeAffects(aRotateOrder, aTopTranslate);
	attributeAffects(aInputCurve, aTopTranslate);
	attributeAffects(aNumberBones, aTopTranslate);
	attributeAffects(aSpineLength, aTopTranslate);
	attributeAffects(aStretchRamp, aTopTranslate);
	attributeAffects(aSquashRamp, aTopTranslate);
	attributeAffects(aTwistRamp, aTopTranslate);
	attributeAffects(aStretchAmount, aTopTranslate);
	attributeAffects(aSquashAmount, aTopTranslate);
	attributeAffects(aExtraTwist, aTopTranslate);
	attributeAffects(aTopMatrix, aTopTranslate);
	attributeAffects(aBotMatrix, aTopTranslate);

	attributeAffects(aBoneAxis, aBottomRotate);
	attributeAffects(aRotateOrder, aBottomRotate);
	attributeAffects(aInputCurve, aBottomRotate);
	attributeAffects(aNumberBones, aBottomRotate);
	attributeAffects(aSpineLength, aBottomRotate);
	attributeAffects(aStretchRamp, aBottomRotate);
	attributeAffects(aSquashRamp, aBottomRotate);
	attributeAffects(aTwistRamp, aBottomRotate);
	attributeAffects(aStretchAmount, aBottomRotate);
	attributeAffects(aSquashAmount, aBottomRotate);
	attributeAffects(aExtraTwist, aBottomRotate);
	attributeAffects(aTopMatrix, aBottomRotate);
	attributeAffects(aBotMatrix, aBottomRotate);

	attributeAffects(aBoneAxis, aBottomTranslate);
	attributeAffects(aRotateOrder, aBottomTranslate);
	attributeAffects(aInputCurve, aBottomTranslate);
	attributeAffects(aNumberBones, aBottomTranslate);
	attributeAffects(aSpineLength, aBottomTranslate);
	attributeAffects(aStretchRamp, aBottomTranslate);
	attributeAffects(aSquashRamp, aBottomTranslate);
	attributeAffects(aTwistRamp, aBottomTranslate);
	attributeAffects(aStretchAmount, aBottomTranslate);
	attributeAffects(aSquashAmount, aBottomTranslate);
	attributeAffects(aExtraTwist, aBottomTranslate);
	attributeAffects(aTopMatrix, aBottomTranslate);
	attributeAffects(aBotMatrix, aBottomTranslate);

	return MS::kSuccess;
}

MStatus JSQuatSpine::compute(const MPlug &plug, MDataBlock &data)
{
	MStatus status;
	if (plug != aMidRotate && plug != aMidTranslate && plug != aMidScale &&
		plug !=aTopRotate && plug != aTopTranslate && plug != aBottomRotate && plug != aBottomTranslate)
	{
		return MS::kUnknownParameter;
	}
	// Get top/bottom output handles
	MDataHandle hTopRotate = data.outputValue(aTopRotate);
	MDataHandle hTopTranslate = data.outputValue(aTopTranslate);
	MDataHandle hBottomRotate = data.outputValue(aBottomRotate);
	MDataHandle hBottomTranslate = data.outputValue(aBottomTranslate);

	// Get mid joint output array handles
	MArrayDataHandle hArrayMidRotate = data.outputValue(aMidRotate);
	MArrayDataHandle hArrayMidTranslate = data.outputValue(aMidTranslate);
	MArrayDataHandle hArrayMidScale = data.outputValue(aMidScale);

	// Get input values
	unsigned int boneAxis = data.inputValue(aBoneAxis).asInt();
	MEulerRotation::RotationOrder euRotOrder = (MEulerRotation::RotationOrder)(data.inputValue(aRotateOrder).asShort());
	MTransformationMatrix::RotationOrder tRotOrder = (MTransformationMatrix::RotationOrder)(data.inputValue(aRotateOrder).asShort() + 1);
	MObject curve = data.inputValue(aInputCurve).asNurbsCurve();
	unsigned int numBones = data.inputValue(aNumberBones).asInt();
	double spineLength = data.inputValue(aSpineLength).asDouble();
	double stretchAmount = data.inputValue(aStretchAmount).asDouble();
	double squashAmount = data.inputValue(aSquashAmount).asDouble();
	double twistMultiplier = data.inputValue(aExtraTwist).asDouble();
	double twistDivisor = 1 / twistMultiplier;

	// Get ramp values
	MObject oThis = thisMObject();
	MRampAttribute stretchAttr(oThis, aStretchRamp, &status);
	MRampAttribute squashAttr(oThis, aSquashRamp, &status);
	MRampAttribute twistAttr(oThis, aTwistRamp, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	
	/////////////////////////////////////////////////////////////
	
	// Get input matrix values, get y-vectors and convert to get quaternions
	MMatrix topMatrix = data.inputValue(aTopMatrix).asMatrix();
	MMatrix botMatrix = data.inputValue(aBotMatrix).asMatrix();

	MVector topY(topMatrix[1][0], topMatrix[1][1], topMatrix[1][2]);
	MVector botY(botMatrix[1][0], botMatrix[1][1], botMatrix[1][2]);
	topY.normalize();
	botY.normalize();

	MQuaternion quatTop = MTransformationMatrix(topMatrix).rotation();
	MQuaternion quatBot = MTransformationMatrix(botMatrix).rotation();

	// Decompose to axis + angle and apply twist multiplier ???
	double angleTopStart, angleBotStart;
	MVector axisTopStart, axisBotStart;
	quatTop.getAxisAngle(axisTopStart, angleTopStart);
	quatBot.getAxisAngle(axisBotStart, angleBotStart);
	axisTopStart.normalize();
	axisBotStart.normalize();

	MQuaternion quatTopTwist(angleTopStart * twistMultiplier, axisTopStart);
	MQuaternion quatBotTwist(angleBotStart * twistMultiplier, axisBotStart);
	
	double alpha = acos(botY * topY);
	MVector cross = topY ^ botY;
	cross.normalize();

	MQuaternion quatCross(alpha, cross);
	MQuaternion quatAlign = quatTop * quatCross;
	MQuaternion quatAlignTwist = quatTopTwist * quatCross;

	//// START loop..........................................................
	for (int i = 0; i < numBones; ++i)
	{
		// Spine unit length
		double unit = static_cast<double>(i) / (numBones - 1);

		if (i == 0)
		{
			// Set bottom output
			MQuaternion botSlerp = slerp(quatBot, quatAlign, unit * twistMultiplier);
			MEulerRotation euBottomRot = quatBot.asEulerRotation();
			MEulerRotation euSlerpRot = botSlerp.asEulerRotation();

			MVector euBottomTrs = MTransformationMatrix(botMatrix).translation(MSpace::kWorld);

			hBottomRotate.set3Double(euBottomRot.x * 180 / M_PI, euSlerpRot.y * 180 / M_PI, euBottomRot.z * 180 / M_PI);
			hBottomTranslate.set3Double(euBottomTrs.x, euBottomTrs.y, euBottomTrs.z);

		}
		else if (i == numBones - 1)
		{
			// Set top output
			MQuaternion topSlerp = slerp(quatBot, quatAlign, unit * twistMultiplier);
			MEulerRotation euTopRot = quatTop.asEulerRotation();
			MEulerRotation euSlerpRot = topSlerp.asEulerRotation();

			
			MVector euTopTrs = MTransformationMatrix(topMatrix).translation(MSpace::kWorld);

			hTopRotate.set3Double(euTopRot.x * 180 / M_PI, euTopRot.y * twistMultiplier * 180 / M_PI, euTopRot.z * 180 / M_PI);
			hTopTranslate.set3Double(euTopTrs.x, euTopTrs.y, euTopTrs.z);

		}
		else if (i > 0 && i < numBones - 1) {
			// Get output datahandles for ith element
			status = hArrayMidRotate.jumpToElement(i - 1);
			status = hArrayMidTranslate.jumpToElement(i - 1);
			status = hArrayMidScale.jumpToElement(i - 1);
			MDataHandle hMidRotate = hArrayMidRotate.outputValue();
			MDataHandle hMidTranslate = hArrayMidTranslate.outputValue();
			MDataHandle hMidScale = hArrayMidScale.outputValue();

			// Get mid joint position from curve
			MPoint targetPt;
			MFnNurbsCurve fnCurve(curve);
			double currentLength = fnCurve.length();
			double targetParam = fnCurve.findParamFromLength(currentLength * unit);
			fnCurve.getPointAtParam(targetParam, targetPt, MSpace::kTransform);

			// Slerp between top align and bottom for temp slerp quaternion representing midTwist in isolation
			MQuaternion quatSlerp = slerp(quatBotTwist, quatAlign, unit * twistMultiplier);
			MMatrix mtxSlerp = quatSlerp.asMatrix();
			MVector ySlerp(mtxSlerp[1][0], mtxSlerp[1][1], mtxSlerp[1][2]);
			ySlerp.normalize();

			// Decompose to axis + angle and apply twist multiplier ???
			double midAngleTwist;
			MVector midAxisTwist;
			quatSlerp.getAxisAngle(midAxisTwist, midAngleTwist);
			midAxisTwist.normalize();
			MQuaternion quatSlerpTwist(midAngleTwist * twistMultiplier, midAxisTwist);

			// Get tangent vector and the tangent/yslerp cross product
			MVector tangent = fnCurve.tangent(targetParam, MSpace::kTransform);
			tangent.normalize();
			MVector midCross = tangent ^ ySlerp;
			midCross.normalize();
			double midAlpha = acos(tangent * ySlerp);
			MQuaternion quatMidCross(midAlpha, midCross);

			MQuaternion quatMidAlign = quatSlerp * quatMidCross.inverse();
			MEulerRotation euMidAlign = quatMidAlign.asEulerRotation();

			MQuaternion quatMidAlignTwist = quatSlerpTwist * quatMidCross.inverse();
			MEulerRotation euMidAlignTwist = quatMidAlignTwist.asEulerRotation();

			// Squash and stretch
			double axisStretch = currentLength / spineLength;
			double squashStretchBase = spineLength / currentLength;
			float rampPos = static_cast<float>(unit);
			float stretchValue;
			float squashValue;
			float twistValue;
			double power;

			stretchAttr.getValueAtPosition(rampPos, stretchValue, &status);
			squashAttr.getValueAtPosition(rampPos, squashValue, &status);

			if (currentLength > spineLength)
			{
				// Stretch
				power = static_cast<double>(stretchValue) * stretchAmount;
			}
			else if (currentLength <= spineLength)
			{
				// Squash
				power = static_cast<double>(squashValue) * squashAmount;
			}

			double scale = pow(squashStretchBase, power);

			// Set out rotate and translate
			hMidTranslate.set3Double(targetPt.x, targetPt.y, targetPt.z);
			hMidRotate.set3Double(euMidAlign.x * 180 / M_PI, euMidAlign.y * 180 / M_PI, euMidAlign.z * 180 / M_PI);
			hMidScale.set3Double(scale, axisStretch, scale);

		}

	}

	//// END loop.............................................................

	data.setClean(aMidRotate);
	data.setClean(aMidTranslate);
	data.setClean(aMidScale);
	data.setClean(aTopRotate);
	data.setClean(aTopTranslate);
	data.setClean(aBottomRotate);
	data.setClean(aBottomTranslate);

	return MS::kSuccess;
}
