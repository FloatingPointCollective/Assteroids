#pragma once
#include "cinder/Vector.h"
#include "cinder/Camera.h"
#include "cinder/app/App.h"


using namespace ci;

class POV {
public:
	POV() {}
	POV( ci::app::App *aApp, ci::vec3 aEye, ci::vec3 aCenter );

	void update();

	void adjustAngle( float aAngleDelta, float aPitch );
	void adjustDist( float aDist );
	void setEye( ci::vec3 v );
	void setCenter( ci::vec3 v );

public:
	CameraPersp  mCam;
	vec3         mEye;
	vec3         mCenter;
	vec3         mEyeNormal;

	float            mAngle, mAngleDest;
	float            mDist, mDistDest;

private:
	ci::app::App    *mApp;

	float            mAspectRatio;
	float            mFOV;

};
