// The sample robotarm model.  You should build a file
// very similar to this for when you make your model.
#pragma warning (disable : 4305)
#pragma warning (disable : 4244)
#pragma warning(disable : 4786)

#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include "particleSystem.h"


#include <FL/gl.h>
#include <stdlib.h>

#define M_DEFAULT 2.0f
#define M_OFFSET 3.0f
#define P_OFFSET 0.3f
#define MAX_VEL 200
#define MIN_STEP 0.1

#define COLOR_RED		1.0f, 0.0f, 0.0f
#define COLOR_GREEN		0.0f, 1.0f, 0.0f
#define COLOR_BLUE		0.0f, 0.0f, 1.0f
#define COLOR_SKIN		1.0f, 0.8f, 0.6f

#ifndef M_PI
#define M_PI 3.141592653589793238462643383279502
#endif



// This is a list of the controls for the RobotArm
// We'll use these constants to access the values 
// of the controls from the user interface.
enum RobotArmControls
{ 
	//TEST,
	XPOS,
	YPOS,
	ZPOS,
	//HEIGHT,
	//ROTATE,
	//LIGHT_X,
	//LIGHT_Y,
	//LIGHT_Z,
	//LIGHT_INTENSITY,
	UPPER_BODY_X,
	UPPER_BODY_Y,
	UPPER_BODY_Z,
	LOWER_BODY_X,
	LOWER_BODY_Y,
	LOWER_BODY_Z,
	LEFT_UPPER_ARM_X,
	LEFT_UPPER_ARM_Y,
	LEFT_UPPER_ARM_Z,
	RIGHT_UPPER_ARM_X,
	RIGHT_UPPER_ARM_Y,
	RIGHT_UPPER_ARM_Z,
	LEFT_LOWER_ARM_X,
	RIGHT_LOWER_ARM_X,
	LEFT_HAND_X,
	LEFT_HAND_Z,
	LEFT_LINK_MOTION,
	LEFT_THUMB,
	RIGHT_HAND_X,
	RIGHT_HAND_Z,
	RIGHT_LINK_MOTION,
	RIGHT_THUMB,
	LEFT_UPPER_LEG_X,
	LEFT_UPPER_LEG_Y,
	LEFT_UPPER_LEG_Z,
	RIGHT_UPPER_LEG_X,
	RIGHT_UPPER_LEG_Y,
	RIGHT_UPPER_LEG_Z,
	LEFT_LOWER_LEG_X,
	RIGHT_LOWER_LEG_X,
	HEAD_X,
	HEAD_Y,
	HEAD_Z,
	LEFT_FOOT_X,
	RIGHT_FOOT_X,
	//ANIMATE,
	//LIGHT_CHANGE,
	GUN,
	HAT,
	CIGAR,
	NOSE_SIZE,
	EYE_COLOR,
	EYE_SIZE,
	EYE_BALL_SIZE,
	EYE_DIST,
	FRAME_ALL,
	LEFT_HAND_CONTRAINT_X,
	LEFT_HAND_CONTRAINT_Y,
	LEFT_HAND_CONTRAINT_Z,
	PARTICLE_COUNT,
	//META_ARM,
	//META_ARM_SIZE,
	NUMCONTROLS

	/*
    BASE_ROTATION=0, LOWER_TILT, UPPER_TILT, CLAW_ROTATION,
        BASE_LENGTH, LOWER_LENGTH, UPPER_LENGTH, PARTICLE_COUNT, NUMCONTROLS, 
		*/
};

void drawHead(float noseScale, int eyeColor, float eyeScale, float eyeBallScale, float eyeDist);
void drawHat(double innerR, double outerR);
void drawSemiSphere(double r);
void ground(float h);
void base(float h);
void rotation_base(float h);
void lower_arm(float h);
void upper_arm(float h);
void claw(float h);
void y_box(float h);

// To make a RobotArm, we inherit off of ModelerView
class RobotArm : public ModelerView 
{
public:
    RobotArm(int x, int y, int w, int h, char *label) 
        : ModelerView(x,y,w,h,label) {}
    virtual void draw();
};

// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView* createRobotArm(int x, int y, int w, int h, char *label)
{ 
    return new RobotArm(x,y,w,h,label); 
}

// We'll be getting the instance of the application a lot; 
// might as well have it as a macro.
#define VAL(x) (ModelerApplication::Instance()->GetControlValue(x))





// We are going to override (is that the right word?) the draw()
// method of ModelerView to draw out RobotArm
void RobotArm::draw()
{
	/* pick up the slider values */	
	/*
	float theta = VAL( BASE_ROTATION );
	float phi = VAL( LOWER_TILT );
	float psi = VAL( UPPER_TILT );
	float cr = VAL( CLAW_ROTATION );
	float h1 = VAL( BASE_LENGTH );
	float h2 = VAL( LOWER_LENGTH );
	float h3 = VAL( UPPER_LENGTH );
	*/
	float pc = VAL( PARTICLE_COUNT );
	

    // This call takes care of a lot of the nasty projection 
    // matrix stuff
    ModelerView::draw();

	static GLfloat lmodel_ambient[] = {0.4,0.4,0.4,1.0};

	// define the model
	setAmbientColor(0.1, 0.1, 0.1);
	setDiffuseColor(COLOR_BLUE);
	glPushMatrix();
	glTranslated(VAL(XPOS), VAL(YPOS), VAL(ZPOS));

	// center of the body
	glPushMatrix();
	glScaled(2, 1, 1);
	drawSphere(1);
	glScaled(0.5, 1, 1);

	// Upper Body		
	glPushMatrix();
	glRotated(VAL(UPPER_BODY_X), 1.0, 0.0, 0.0);
	glRotated(VAL(UPPER_BODY_Y), 0.0, 1.0, 0.0);
	glRotated(VAL(UPPER_BODY_Z), 0.0, 0.0, 1.0);
	glRotated(-90, 1.0, 0.0, 0.0);
	glScaled(2, 1.4, 1);
	drawCylinder(4, 1, 1.2);        //    , "./images/shirt.bmp");
	glScaled(0.5, 1 / 1.4, 1);
	// Left upper arm pivot
	setDiffuseColor(COLOR_SKIN);
	glPushMatrix();
	glTranslated(2.2, 0, 3.2);
	drawSphere(0.8);
	// Left upper arm
	glRotated(90, 1.0, 0.0, 0.0);
	glRotated(VAL(LEFT_UPPER_ARM_X) - VAL(LEFT_LINK_MOTION) * 70 / 90, 1.0, 0.0, 0.0);
	glRotated(VAL(LEFT_UPPER_ARM_Y) - VAL(LEFT_LINK_MOTION) * 35 / 90, 0.0, 1.0, 0.0);
	glRotated(VAL(LEFT_UPPER_ARM_Z) + VAL(LEFT_LINK_MOTION) * 25 / 90, 0.0, 0.0, 1.0);
	glRotated(90, 1.0, 0.0, 0.0);
	/*
	if (VAL(META_ARM)){
		glScaled(0.2, 0.2, 0.5);
		glRotated(-90, 0.0, 1.0, 0.0);
		drawMetaarm(VAL(META_ARM_SIZE));
		glRotated(90, 0.0, 1.0, 0.0);
		glScaled(5, 5, 2);
	}*/
	//else{
		drawCylinder(4.5, 0.8, 0.6);
	//}
	// Left lower arm pivot
	glTranslated(0, 0, 4.5);
	drawSphere(0.6);
	// Left lower arm
	glRotated(-90, 1.0, 0.0, 0.0);
	glRotated(VAL(LEFT_LOWER_ARM_X) - VAL(LEFT_LINK_MOTION) * 130 / 90, 1.0, 0.0, 0.0);
	glRotated(90, 1.0, 0.0, 0.0);
	drawCylinder(3.5, 0.6, 0.4);
	// Left hand pivot
	glTranslated(0, 0, 3.5);
	drawSphere(0.4);
	// Left hand
	glRotated(VAL(LEFT_HAND_X), 1.0, 0.0, 0.0);
	glRotated(VAL(LEFT_HAND_Z), 0.0, 1.0, 0.0);
	glTranslated(-0.2, -0.5, 0);
	drawBox(0.4, 1, 1.2);
	// Gun	
	if (VAL(GUN)){
		setDiffuseColor(0.55, 0.52, 0.41);
		glPushMatrix();
		glTranslated(-0.4, 0, 0.2);
		//drawGun();
		glPopMatrix();
		setDiffuseColor(COLOR_SKIN);
	}
	// Left thumb
	glPushMatrix();
	glTranslated(0, 1, 0);
	glTranslated(0.2, 0, 0);
	glRotated(VAL(LEFT_THUMB), 1.0, 0.0, 0.0);
	glTranslated(-0.2, 0, 0);
	drawBox(0.4, 0.4, 0.6);
	glPopMatrix();
	glPopMatrix();

	// Right upper arm pivot
	glPushMatrix();
	glTranslated(-2.2, 0, 3.2);
	drawSphere(0.8);
	// Right upper arm
	glRotated(90, 1.0, 0.0, 0.0);
	glRotated(VAL(RIGHT_UPPER_ARM_X) - VAL(RIGHT_LINK_MOTION) * 70 / 90 , 1.0, 0.0, 0.0);
	glRotated(VAL(RIGHT_UPPER_ARM_Y) + VAL(RIGHT_LINK_MOTION) * 35 / 90, 0.0, 1.0, 0.0);
	glRotated(VAL(RIGHT_UPPER_ARM_Z) - VAL(RIGHT_LINK_MOTION) * 25 / 90, 0.0, 0.0, 1.0);
	glRotated(90, 1.0, 0.0, 0.0);
	/*
	if (VAL(META_ARM)){
		glScaled(0.2, 0.2, 0.5);
		glRotated(-90, 0.0, 1.0, 0.0);
		drawMetaarm(VAL(META_ARM_SIZE));
		glRotated(90, 0.0, 1.0, 0.0);
		glScaled(5, 5, 2);
	}*/
	//else{
		drawCylinder(4.5, 0.8, 0.6);
	//}
	// Right lower arm pivot
	glTranslated(0, 0, 4.5);
	drawSphere(0.6);
	// Right lower arm
	glRotated(-90, 1.0, 0.0, 0.0);
	glRotated(VAL(RIGHT_LOWER_ARM_X) - VAL(RIGHT_LINK_MOTION) * 130 / 90, 1.0, 0.0, 0.0);
	glRotated(90, 1.0, 0.0, 0.0);
	drawCylinder(3.5, 0.6, 0.4);
	// Right hand pivot
	glTranslated(0, 0, 3.5);
	drawSphere(0.4);
	// Right hand
	glRotated(VAL(RIGHT_HAND_X), 1.0, 0.0, 0.0);
	glRotated(VAL(RIGHT_HAND_Z), 0.0, 1.0, 0.0);
	glTranslated(-0.2, -0.5, 0);
	drawBox(0.4, 1, 1.2);
	// Right thumb
	glTranslated(0, 1, 0);
	glTranslated(0.2, 0, 0);
	glRotated(VAL(RIGHT_THUMB), 1.0, 0.0, 0.0);
	glTranslated(-0.2, 0, 0);
	drawBox(0.4, 0.4, 0.6);
	glPopMatrix();

	// Neck pivot
	glPushMatrix();
	glTranslated(0, 0, 4);
	drawSphere(0.7);
	// Neck
	glRotated(90, 1, 0, 0);
	glRotated(VAL(HEAD_X), 1, 0, 0);
	glRotated(VAL(HEAD_Y), 0, 1, 0);
	glRotated(VAL(HEAD_Z), 0, 0, 1);
	glRotated(-90, 1, 0, 0);
	drawCylinder(1, 0.7, 0.7);
	// head
	glTranslated(0, 0, 1.5);
	glScaled(1, 1, 1.5);
	drawHead(VAL(NOSE_SIZE), VAL(EYE_COLOR), VAL(EYE_SIZE), VAL(EYE_BALL_SIZE), VAL(EYE_DIST));
	glScaled(1, 1, 1.0 / 1.5);
	// hat
	if (VAL(HAT)){
		setDiffuseColor(0.76, 0.65, 0.19);
		glPushMatrix();
		glTranslated(0, 0, 1.8);
		glRotated(90, 1.0, 0.0, 0.0);
		drawHat(1, 2);
		glPopMatrix();
		setDiffuseColor(COLOR_SKIN);
	}
	// cigar
	if (VAL(CIGAR)){
		setDiffuseColor(0.90, 0.40, 0.02);
		glPushMatrix();
		glTranslated(0, 0, -0.2);
		glRotated(90, 1.0, 0.0, 0.0);
		drawCylinder(1.5, 0.1, 0.15);
		glPopMatrix();
		setDiffuseColor(COLOR_SKIN);
	}
	setDiffuseColor(COLOR_SKIN);
	glPopMatrix();
	glPopMatrix();

	// Lower Body
	setDiffuseColor(COLOR_BLUE);
	glPushMatrix();
	glRotated(VAL(LOWER_BODY_X), 1.0, 0.0, 0.0);
	glRotated(VAL(LOWER_BODY_Y), 0.0, 1.0, 0.0);
	glRotated(VAL(LOWER_BODY_Z), 0.0, 0.0, 1.0);
	glRotated(90, 1.0, 0.0, 0.0);
	glScaled(2, 1.4, 1);
	drawCylinder(3, 1, 1.2);      // "./images/trouser.bmp");
	glScaled(0.5, 1 / 1.4, 1);
	// Left upper leg pivot
	setDiffuseColor(COLOR_SKIN);
	glPushMatrix();
	glTranslated(1, 0, 3);
	drawSphere(1.0);
	// Left upper leg
	glRotated(-90, 1.0, 0.0, 0.0);
	glRotated(VAL(LEFT_UPPER_LEG_X), 1.0, 0.0, 0.0);
	glRotated(VAL(LEFT_UPPER_LEG_Y), 0.0, 1.0, 0.0);
	glRotated(VAL(LEFT_UPPER_LEG_Z), 0.0, 0.0, 1.0);
	glRotated(90, 1.0, 0.0, 0.0);
	drawCylinder(4, 1, 0.8);
	// Left lower leg pivot
	glTranslated(0, 0, 4);
	drawSphere(0.8);
	// Left lower leg						
	glRotated(VAL(LEFT_LOWER_LEG_X), 1.0, 0.0, 0.0);
	drawCylinder(4, 0.8, 0.6);
	// Left foot pivot
	glTranslated(0, 0, 4);
	drawSphere(0.6);
	// Left foot
	setDiffuseColor(COLOR_RED);
	glRotated(VAL(LEFT_FOOT_X), 1.0, 0.0, 0.0);
	glTranslated(-0.6, -0.7, 0);
	drawBox(1.2, 2.6, 0.6);
	setDiffuseColor(COLOR_SKIN);
	glPopMatrix();

	// Right upper leg pivot
	glPushMatrix();
	glTranslated(-1, 0, 3);
	drawSphere(1.0);
	// Right upper leg
	glRotated(-90, 1.0, 0.0, 0.0);
	glRotated(VAL(RIGHT_UPPER_LEG_X), 1.0, 0.0, 0.0);
	glRotated(VAL(RIGHT_UPPER_LEG_Y), 0.0, 1.0, 0.0);
	glRotated(VAL(RIGHT_UPPER_LEG_Z), 0.0, 0.0, 1.0);
	glRotated(90, 1.0, 0.0, 0.0);
	drawCylinder(4, 1, 0.8);
	// Right lower leg pivot
	glTranslated(0, 0, 4);
	drawSphere(0.8);
	// Right lower leg						
	glRotated(VAL(RIGHT_LOWER_LEG_X), 1.0, 0.0, 0.0);
	drawCylinder(4, 0.8, 0.6);
	// Right foot pivot
	glTranslated(0, 0, 4);
	drawSphere(0.6);
	//Right foot
	setDiffuseColor(COLOR_RED);
	glRotated(VAL(RIGHT_FOOT_X), 1.0, 0.0, 0.0);
	glTranslated(-0.6, -0.7, 0);
	drawBox(1.2, 2.6, 0.6);
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	
	
	/*
	ground(-0.2);

	base(0.8);

    glTranslatef( 0.0, 0.8, 0.0 );			// move to the top of the base
    glRotatef( theta, 0.0, 1.0, 0.0 );		// turn the whole assembly around the y-axis. 
	rotation_base(h1);						// draw the rotation base

    glTranslatef( 0.0, h1, 0.0 );			// move to the top of the base
    glRotatef( phi, 0.0, 0.0, 1.0 );		// rotate around the z-axis for the lower arm
	glTranslatef( -0.1, 0.0, 0.4 );
	lower_arm(h2);							// draw the lower arm

    glTranslatef( 0.0, h2, 0.0 );			// move to the top of the lower arm
    glRotatef( psi, 0.0, 0.0, 1.0 );		// rotate  around z-axis for the upper arm
	upper_arm(h3);							// draw the upper arm

	glTranslatef( 0.0, h3, 0.0 );
	glRotatef( cr, 0.0, 0.0, 1.0 );
	claw(1.0);
	*/
	//*** DON'T FORGET TO PUT THIS IN YOUR OWN CODE **/
	endDraw();
}

void drawHead(float noseScale, int eyeColor, float eyeScale, float eyeBallScale, float eyeDist)
{
	glPushMatrix();

	setDiffuseColor(0.0f, 0.8f, 0.8f);
	//up front
	drawTriangle(0.8, -0.4, 0, -0.8, -0.4, 0, -0.6, -0.2, 1.6);
	drawTriangle(0.8, -0.4, 0, -0.6, -0.2, 1.6, 0.6, -0.2, 1.6);

	//down front
	drawTriangle(0.8, -0.4, 0, -0.8, -0.4, 0, 0.4, -0.2, -0.8);
	drawTriangle(-0.8, -0.4, 0, -0.4, -0.2, -0.8, 0.4, -0.2, -0.8);

	//top
	setDiffuseColor(0.98f, 0.96f, 0.2f);
	drawTriangle(0.6, -0.2, 1.6, -0.6, -0.2, 1.6, -0.6, 0.2, 1.6);
	drawTriangle(0.6, -0.2, 1.6, 0.6, 0.2, 1.6, -0.6, 0.2, 1.6);
	setDiffuseColor(0.0f, 0.8f, 0.8f);

	//up back
	setDiffuseColor(0.98f, 0.96f, 0.2f);
	drawTriangle(0.8, 0.4, 0, -0.8, 0.4, 0, -0.6, 0.2, 1.6);
	drawTriangle(0.8, 0.4, 0, -0.6, 0.2, 1.6, 0.6, 0.2, 1.6);
	setDiffuseColor(0.0f, 0.8f, 0.8f);

	//down back
	drawTriangle(0.8, 0.4, 0, -0.8, 0.4, 0, 0.4, 0.2, -0.8);
	drawTriangle(-0.8, 0.4, 0, -0.4, 0.2, -0.8, 0.4, 0.2, -0.8);

	//down
	drawTriangle(0.4, -0.2, -0.8, -0.4, -0.2, -0.8, -0.4, 0.2, -0.8);
	drawTriangle(0.4, -0.2, -0.8, 0.4, 0.2, -0.8, -0.4, 0.2, -0.8);

	//up left
	drawTriangle(-0.8, -0.4, 0, -0.8, 0.4, 0, -0.6, -0.2, 1.6);
	setDiffuseColor(0.98f, 0.96f, 0.2f);
	drawTriangle(-0.6, 0.2, 1.6, -0.8, 0.4, 0, -0.6, -0.2, 1.6);
	setDiffuseColor(0.0f, 0.8f, 0.8f);

	//up right
	drawTriangle(0.8, -0.4, 0, 0.8, 0.4, 0, 0.6, -0.2, 1.6);
	setDiffuseColor(0.98f, 0.86f, 0.2f);
	drawTriangle(0.6, 0.2, 1.6, 0.8, 0.4, 0, 0.6, -0.2, 1.6);
	setDiffuseColor(0.0f, 0.8f, 0.8f);

	//down left
	drawTriangle(-0.8, -0.4, 0, -0.8, 0.4, 0, -0.4, -0.2, -0.8);
	drawTriangle(-0.4, 0.2, -0.8, -0.8, 0.4, 0, -0.4, -0.2, -0.8);

	//down right
	drawTriangle(0.8, -0.4, 0, 0.8, 0.4, 0, 0.4, -0.2, -0.8);
	drawTriangle(0.4, 0.2, -0.8, 0.8, 0.4, 0, 0.4, -0.2, -0.8);

	//nose
	glPushMatrix();
	glScaled(noseScale, noseScale, noseScale);
	drawTriangle(0, -0.375, 0.6, -0.2, -0.4, 0, 0, -0.6, 0.2);
	drawTriangle(0, -0.375, 0.6, 0.2, -0.4, 0, 0, -0.6, 0.2);
	drawTriangle(-0.2, -0.4, 0, 0.2, 0.4, 0, 0, -0.6, 0.2);
	glPopMatrix();

	// eye1
	glPushMatrix();
	glTranslated(-eyeDist, -0.2, 0.8);
	//glPushMatrix();
	//glScaled(eyeBallScale, eyeBallScale, eyeBallScale);
	drawSphere(0.3);
	//glPopMatrix();
	switch (eyeColor){
	case 0:
		// black
		setDiffuseColor(0.0f, 0.0f, 0.0f);
		break;
	case 1:
		// blue
		setDiffuseColor(COLOR_BLUE);
		break;
	case 2:
		// red
		setDiffuseColor(COLOR_RED);
		break;
	}
	glTranslated(0.02, -0.28, 0);
	glPushMatrix();
	glScaled(eyeScale, eyeScale, eyeScale);
	drawSphere(0.05);
	glPopMatrix();
	setDiffuseColor(0.0f, 0.8f, 0.8f);
	glPopMatrix();


	// eye2
	glPushMatrix();
	glTranslated(eyeDist, -0.2, 0.8);   //0.3
	//glPushMatrix();
	//glScaled(eyeBallScale, eyeBallScale, eyeBallScale);
	drawSphere(0.3);
	//glPopMatrix();
	switch (eyeColor){
	case 0:
		// black
		setDiffuseColor(0.0f, 0.0f, 0.0f);
		break;
	case 1:
		// blue
		setDiffuseColor(COLOR_BLUE);
		break;
	case 2:
		// red
		setDiffuseColor(COLOR_RED);
		break;
	}
	glTranslated(-0.02, -0.28, 0);
	glPushMatrix();
	glScaled(eyeScale, eyeScale, eyeScale);
	drawSphere(0.05);
	glPopMatrix();
	setDiffuseColor(0.0f, 0.8f, 0.8f);
	glPopMatrix();

	glPopMatrix();
	//glEnd();
}

void drawSemiSphere(double r){
	double clipEq[4] = { 0, 1, 0, 0 };

	glEnable(GL_CLIP_PLANE0);
	glClipPlane(GL_CLIP_PLANE0, clipEq);

	drawSphere(r);


	glDisable(GL_CLIP_PLANE0);
}


void drawHat(double innerR, double outerR){
	glPushMatrix();
	drawSemiSphere(innerR);
	glRotated(-90, 1.0, 0.0, 0.0);
	drawCylinder(0.1, outerR, outerR);
	glPopMatrix();
}



void ground(float h) 
{
	glDisable(GL_LIGHTING);
	glColor3f(0.65,0.45,0.2);
	glPushMatrix();
	glScalef(30,0,30);
	y_box(h);
	glPopMatrix();
	glEnable(GL_LIGHTING);
}

void base(float h) {
	setDiffuseColor( 0.25, 0.25, 0.25 );
	setAmbientColor( 0.25, 0.25, 0.25 );
	glPushMatrix();
		glPushMatrix();
			glTranslatef(1.0, h / 2.0, 0.75);
			drawCylinder(0.25, h / 2.0, h / 2.0);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(1.0, h / 2.0, -1.0);
			drawCylinder(0.25, h / 2.0, h / 2.0);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-1.0, h / 2.0, 0.75);
			drawCylinder(0.25, h / 2.0, h / 2.0);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-1.0, h / 2.0, -1.0);
			drawCylinder(0.25, h / 2.0, h / 2.0);
		glPopMatrix();
	glScalef(4.0f, h, 4.0f);
	y_box(1.0f);
	glPopMatrix();
}

void rotation_base(float h) {
	setDiffuseColor( 0.85, 0.75, 0.25 );
	setAmbientColor( 0.95, 0.75, 0.25 );
	glPushMatrix();
		glPushMatrix();
			glScalef(4.0, h, 4.0);
			y_box(1.0f); // the rotation base
		glPopMatrix();
		setDiffuseColor( 0.15, 0.15, 0.65 );
		setAmbientColor( 0.15, 0.15, 0.65 );
		glPushMatrix();
			glTranslatef(-0.5, h, -0.6);
			glScalef(2.0, h, 1.6);
			y_box(1.0f); // the console
		glPopMatrix();
		setDiffuseColor( 0.65, 0.65, 0.65 );
		setAmbientColor( 0.65, 0.65, 0.65 );
		glPushMatrix();
			glTranslatef( 0.5, h, 0.6 );
			glRotatef( -90.0, 1.0, 0.0, 0.0 );
			drawCylinder( h, 0.05, 0.05 ); // the pipe
		glPopMatrix();
	glPopMatrix();
}

void lower_arm(float h) {					// draw the lower arm
	setDiffuseColor( 0.85, 0.75, 0.25 );
	setAmbientColor( 0.95, 0.75, 0.25 );
	y_box(h);
}

void upper_arm(float h) {					// draw the upper arm
	setDiffuseColor( 0.85, 0.75, 0.25 );
	setAmbientColor( 0.95, 0.75, 0.25 );
	glPushMatrix();
	glScalef( 1.0, 1.0, 0.7 );
	y_box(h);
	glPopMatrix();
}

void claw(float h) {
	setDiffuseColor( 0.25, 0.25, 0.85 );
	setAmbientColor( 0.25, 0.25, 0.85 );

	glBegin( GL_TRIANGLES );

	glNormal3d( 0.0, 0.0, 1.0);		// +z side
	glVertex3d( 0.5, 0.0, 0.5);
	glVertex3d(-0.5, 0.0, 0.5);
	glVertex3d( 0.5,   h, 0.5);

	glNormal3d( 0.0, 0.0, -1.0);	// -z side
	glVertex3d( 0.5, 0.0, -0.5);
	glVertex3d(-0.5, 0.0, -0.5);
	glVertex3d( 0.5,   h, -0.5);

	glEnd();

	glBegin( GL_QUADS );

	glNormal3d( 1.0,  0.0,  0.0);	// +x side
	glVertex3d( 0.5, 0.0,-0.5);
	glVertex3d( 0.5, 0.0, 0.5);
	glVertex3d( 0.5,   h, 0.5);
	glVertex3d( 0.5,   h,-0.5);

	glNormal3d( 0.0,-1.0, 0.0);		// -y side
	glVertex3d( 0.5, 0.0, 0.5);
	glVertex3d( 0.5, 0.0,-0.5);
	glVertex3d(-0.5, 0.0,-0.5);
	glVertex3d(-0.5, 0.0, 0.5);

	glEnd();
}

void y_box(float h) {

	glBegin( GL_QUADS );

	glNormal3d( 1.0 ,0.0, 0.0);			// +x side
	glVertex3d( 0.25,0.0, 0.25);
	glVertex3d( 0.25,0.0,-0.25);
	glVertex3d( 0.25,  h,-0.25);
	glVertex3d( 0.25,  h, 0.25);

	glNormal3d( 0.0 ,0.0, -1.0);		// -z side
	glVertex3d( 0.25,0.0,-0.25);
	glVertex3d(-0.25,0.0,-0.25);
	glVertex3d(-0.25,  h,-0.25);
	glVertex3d( 0.25,  h,-0.25);

	glNormal3d(-1.0, 0.0, 0.0);			// -x side
	glVertex3d(-0.25,0.0,-0.25);
	glVertex3d(-0.25,0.0, 0.25);
	glVertex3d(-0.25,  h, 0.25);
	glVertex3d(-0.25,  h,-0.25);

	glNormal3d( 0.0, 0.0, 1.0);			// +z side
	glVertex3d(-0.25,0.0, 0.25);
	glVertex3d( 0.25,0.0, 0.25);
	glVertex3d( 0.25,  h, 0.25);
	glVertex3d(-0.25,  h, 0.25);

	glNormal3d( 0.0, 1.0, 0.0);			// top (+y)
	glVertex3d( 0.25,  h, 0.25);
	glVertex3d( 0.25,  h,-0.25);
	glVertex3d(-0.25,  h,-0.25);
	glVertex3d(-0.25,  h, 0.25);

	glNormal3d( 0.0,-1.0, 0.0);			// bottom (-y)
	glVertex3d( 0.25,0.0, 0.25);
	glVertex3d(-0.25,0.0, 0.25);
	glVertex3d(-0.25,0.0,-0.25);
	glVertex3d( 0.25,0.0,-0.25);

	glEnd();
}

int main()
{
    ModelerControl controls[NUMCONTROLS ];
	
	//controls[TEST] = ModelerControl("Test", 0, 1, 0.1f, 1);

	controls[XPOS] = ModelerControl("X Position", -5, 5, 0.1f, 0);
	controls[YPOS] = ModelerControl("Y Position", 0, 5, 0.1f, 0);
	controls[ZPOS] = ModelerControl("Z Position", -5, 5, 0.1f, 0);
	//controls[HEIGHT] = ModelerControl("Height", 1, 2.5, 0.1f, 1);
	//controls[ROTATE] = ModelerControl("Rotate", -135, 135, 1, 0);
	//controls[LIGHT_INTENSITY] = ModelerControl("Light Intensity", 0, 5, 0.1f, 1);
	//controls[LIGHT_X] = ModelerControl("Light X", -30, 30, 1, 4);
	//controls[LIGHT_Y] = ModelerControl("Light Y", -30, 30, 1, 2);
	//controls[LIGHT_Z] = ModelerControl("Light Z", -30, 30, 1, 4);
	controls[UPPER_BODY_X] = ModelerControl("Upper Body X", -90, 90, 1, 0);
	controls[UPPER_BODY_Y] = ModelerControl("Upper Body Y", -90, 90, 1, 0);
	controls[UPPER_BODY_Z] = ModelerControl("Upper Body Z", -45, 45, 1, 0);
	controls[LOWER_BODY_X] = ModelerControl("Lower Body X", -90, 90, 1, 0);
	controls[LOWER_BODY_Y] = ModelerControl("Lower Body Y", -90, 90, 1, 0);
	controls[LOWER_BODY_Z] = ModelerControl("Lower Body Z", -45, 45, 1, 0);
	controls[LEFT_UPPER_ARM_X] = ModelerControl("Left Upper Arm X", -180, 45, 1, -15); // -15
	controls[LEFT_UPPER_ARM_Y] = ModelerControl("Left Upper Arm Y", -180, 90, 1, 0);
	controls[LEFT_UPPER_ARM_Z] = ModelerControl("Left Upper Arm Z", 0, 180, 1, 15);  //15
	controls[RIGHT_UPPER_ARM_X] = ModelerControl("Right Upper Arm X", -180, 45, 1, -15); //-15
	controls[RIGHT_UPPER_ARM_Y] = ModelerControl("Right Upper Arm Y", -90, 180, 1, 0);
	controls[RIGHT_UPPER_ARM_Z] = ModelerControl("Right Upper Arm Z", -180, 0, 1, -15);//15
	controls[LEFT_LOWER_ARM_X] = ModelerControl("Left Lower Arm X", -140, 0, 1, 0);
	controls[RIGHT_LOWER_ARM_X] = ModelerControl("Right Lower Arm X", -140, 0, 1, 0);
	controls[LEFT_HAND_X] = ModelerControl("Left Hand X", -30, 30, 1, 0);
	controls[LEFT_HAND_Z] = ModelerControl("Left Hand Z", -60, 60, 1, 0);
	controls[LEFT_LINK_MOTION] = ModelerControl("Left Link Motion", 0, 90, 1, 0);
	controls[RIGHT_HAND_X] = ModelerControl("Right Hand X", -30, 30, 1, 0);
	controls[RIGHT_HAND_Z] = ModelerControl("Right Hand Z", -60, 60, 1, 0);
	controls[RIGHT_LINK_MOTION] = ModelerControl("Right Link Motion", 0, 90, 1, 0);
	controls[LEFT_UPPER_LEG_X] = ModelerControl("Left Upper Leg X", -90, 45, 1, 0);
	controls[LEFT_UPPER_LEG_Y] = ModelerControl("Left Upper Leg Y", -45, 90, 1, 0);
	controls[LEFT_UPPER_LEG_Z] = ModelerControl("Left Upper Leg Z", 0, 45, 1, 0);
	controls[RIGHT_UPPER_LEG_X] = ModelerControl("Right Upper Leg X", -90, 45, 1, 0);
	controls[RIGHT_UPPER_LEG_Y] = ModelerControl("Right Upper Leg Y", -90, 45, 1, 0);
	controls[RIGHT_UPPER_LEG_Z] = ModelerControl("Right Upper Leg Z", -45, 0, 1, 0);
	controls[LEFT_LOWER_LEG_X] = ModelerControl("Left Lower Leg X", 0, 160, 1, 0);
	controls[RIGHT_LOWER_LEG_X] = ModelerControl("Right Lower Leg X", 0, 160, 1, 0);
	controls[HEAD_X] = ModelerControl("Head X", -60, 60, 1, 0);
	controls[HEAD_Y] = ModelerControl("Head Y", -90, 90, 1, 0);
	controls[HEAD_Z] = ModelerControl("Head Z", -45, 45, 1, 0);
	controls[LEFT_FOOT_X] = ModelerControl("Left Foot X", -40, 40, 1, 0);
	controls[RIGHT_FOOT_X] = ModelerControl("Right Foot X", -40, 40, 1, 0);
	controls[LEFT_HAND_X] = ModelerControl("Left Hand X", -35, 35, 1, 0);
	controls[LEFT_HAND_Z] = ModelerControl("Left Hand Z", -60, 60, 1, 0);
	controls[RIGHT_HAND_X] = ModelerControl("Right Hand X", -35, 35, 1, 0);
	controls[RIGHT_HAND_Z] = ModelerControl("Right Hand Z", -60, 60, 1, 0);
	controls[LEFT_THUMB] = ModelerControl("Left Thumb", -90, 0, 1, 0);
	controls[RIGHT_THUMB] = ModelerControl("Right Thumb", -90, 0, 1, 0);
	controls[GUN] = ModelerControl("Gun?", 0, 1, 1, 0);
	controls[HAT] = ModelerControl("Hat?", 0, 1, 1, 0);
	controls[CIGAR] = ModelerControl("Cigar", 0, 1, 1, 0);
	controls[NOSE_SIZE] = ModelerControl("Nose Size", 1, 1.5, 0.1, 1);
	controls[EYE_COLOR] = ModelerControl("Eye Color", 0, 2, 1, 0);
	controls[EYE_SIZE] = ModelerControl("Eye Size", 1, 2, 0.1f, 1);
	controls[EYE_BALL_SIZE] = ModelerControl("Eye Ball Size", 1, 1.5, 0.1f, 1);
	controls[EYE_DIST] = ModelerControl("Eye Distance", 0.2, 0.5, 0.05, 0.3f);
	controls[FRAME_ALL] = ModelerControl("Frame All?", 0, 1, 1, 0);
	controls[LEFT_HAND_CONTRAINT_X] = ModelerControl("Left Hand Constraint X", 0, 1, 1, 0);
	controls[LEFT_HAND_CONTRAINT_Y] = ModelerControl("Left Hand Constraint Y", 0, 1, 1, 0);
	controls[LEFT_HAND_CONTRAINT_Z] = ModelerControl("Left Hand Constraint Z", 0, 1, 1, 0);
	//controls[META_ARM] = ModelerControl("Meta Arm?", 0, 1, 1, 0);
	//controls[META_ARM_SIZE] = ModelerControl("Meta Arm Size", 6, 25, 0.1f, 8.5);

	//controls[ANIMATE] = ModelerControl("Animate", 0, 1, 1, 0);
	//controls[LIGHT_CHANGE] = ModelerControl("Light Change", 0, 1, 1, 0);

	/*
	controls[BASE_ROTATION] = ModelerControl("base rotation (theta)", -180.0, 180.0, 0.1, 0.0 );
    controls[LOWER_TILT] = ModelerControl("lower arm tilt (phi)", 15.0, 95.0, 0.1, 55.0 );
    controls[UPPER_TILT] = ModelerControl("upper arm tilt (psi)", 0.0, 135.0, 0.1, 30.0 );
	controls[CLAW_ROTATION] = ModelerControl("claw rotation (cr)", -30.0, 180.0, 0.1, 0.0 );
    controls[BASE_LENGTH] = ModelerControl("base height (h1)", 0.5, 10.0, 0.1, 0.8 );
    controls[LOWER_LENGTH] = ModelerControl("lower arm length (h2)", 1, 10.0, 0.1, 3.0 );
    controls[UPPER_LENGTH] = ModelerControl("upper arm length (h3)", 1, 10.0, 0.1, 2.5 );
	*/
    controls[PARTICLE_COUNT] = ModelerControl("particle count (pc)", 0.0, 5.0, 0.1, 5.0 );
    


	// You should create a ParticleSystem object ps here and then
	// call ModelerApplication::Instance()->SetParticleSystem(ps)
	// to hook it up to the animator interface.
	ParticleSystem *ps = new ParticleSystem();
	// do some more particle system setup

	ModelerApplication::Instance()->SetParticleSystem(ps);
    ModelerApplication::Instance()->Init(&createRobotArm, controls, NUMCONTROLS);

    return ModelerApplication::Instance()->Run();
}
