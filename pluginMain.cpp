#include "jsQuatSpine.h"
#include "common.h"
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>

MStatus initializePlugin(MObject mObj)
{
	MStatus status;

	MGlobal::executeCommand(JSQuatSpine::aeCommand);

	MFnPlugin fnPlugin(mObj, "James Sumner III", "1.0", "2016");

	status = fnPlugin.registerNode(JSQuatSpine::kName,
								   JSQuatSpine::kId,
								   JSQuatSpine::creator,
								   JSQuatSpine::initialize,
								   MPxNode::kDependNode);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return status;
}

MStatus uninitializePlugin(MObject mObj)
{
	MStatus status;
	MFnPlugin fnPlugin(mObj);

	status = fnPlugin.deregisterNode(JSQuatSpine::kId);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	return status;
}