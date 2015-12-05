#include "ParticleSystem.h"

#include "cinder/app/App.h"


#include "cinder/Utilities.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

using namespace ci;
using namespace ci::app;

/*
   BINNED PARTCILE SYSTEM controller
 
   ALL CREDIT for this code goes to KYLE MCDONALD and his original OF version
   posted here http://www.openframeworks.cc/forum/viewtopic.php?f=12&t=2860
*/

// BPS stands for Binned Particle System
class bps {
 public:	
	void setup(int kParticles, vector<Roid> *roids);
	void update();
	void draw();
	
	void keyDown( KeyEvent event );
	void mouseDown( MouseEvent event);
	void mouseUp( MouseEvent event);
	void mouseDrag( MouseEvent event );
	
	vec2 mouse;
	
	float timeStep;
	float lineOpacity, pointOpacity;
	float particleNeighborhood, particleRepulsion;
	float centerAttraction;
	
	int mKParticles;
	ParticleSystem particleSystem;
	bool isMousePressed, slowMotion;
    
    vector<Roid> *mRoids;
};

void bps::setup(int kParticles, vector<Roid> *roids){
	// this number describes how many bins are used
	// on my machine, 2 is the ideal number (2^2 = 4x4 pixel bins)
	// if this number is too high, binning is not effective
	// because the screen is not subdivided enough. if
	// it's too low, the bins take up so much memory as to
	// become inefficient.
	int binPower = 4;
    
    mRoids = roids;
	
	particleSystem.setup(getWindowWidth(), getWindowHeight(), binPower);
	
	//mKParticles = kParticles;
	float padding = 0;
	float maxVelocity = .5;
	for(int i = 0; i < kParticles * 1024; i++) {
		float x = Rand::randFloat(padding, getWindowWidth() - padding);
		float y = Rand::randFloat(padding, getWindowHeight() - padding);
		float xv = Rand::randFloat(-maxVelocity, maxVelocity);
		float yv = Rand::randFloat(-maxVelocity, maxVelocity);
		Particle particle(x, y, xv, yv);
		particleSystem.add(particle);
	}
	
	timeStep = 1;
	lineOpacity = 0.12f;
	pointOpacity = 0.5f;
	slowMotion = false;
	particleNeighborhood = 14;
	particleRepulsion = .5;
	centerAttraction = .05;
}

void bps::update(){
	particleSystem.setTimeStep(timeStep);
}

void bps::draw()
{
    
	//gl::clear();
	gl::enableAdditiveBlending();
    gl::color(1.0f, 1.0f, 1.0f, lineOpacity);
	
	particleSystem.setupForces();
    
	// apply per-particle forces
    gl::begin(GL_LINES);
	for(int i = 0; i < particleSystem.size(); i++) {
		Particle& cur = particleSystem[i];
		// global force on other particles
		particleSystem.addRepulsionForce(cur, particleNeighborhood, particleRepulsion);
		// forces on this particle
		cur.bounceOffWalls(0, 0, getWindowWidth(), getWindowHeight());
		cur.addDampingForce();
	}
    gl::end();
    
    //add repulsion forces for each assteroid
    for (auto roid : *mRoids)
    {
        cout<<"roid.x"<<roid.x<<endl;
        cout<<"roid.y"<<roid.y<<endl;
        particleSystem.addRepulsionForce(roid.x+getWindowWidth()/2, roid.y+getWindowHeight()/2, roid.radius, 10);
    }
    
	// single global forces
	particleSystem.addAttractionForce(getWindowWidth()/2, getWindowHeight()/2, getWindowWidth(), centerAttraction);
    
	if(isMousePressed)
		particleSystem.addRepulsionForce(mouse.x, mouse.y, 100, 10);
	particleSystem.update();
    gl::color(1.0f, 1.0f, 1.0f, pointOpacity);
	particleSystem.draw();
	gl::disableAlphaBlending();
	
    gl::color(1.0f, 1.0f, 1.0f);
	//gl::drawString( toString( kParticles ) + "k particles", vec2(32.0f, 32.0f));
	//gl::drawString( toString((int) getAverageFps()) + " fps", vec2(32.0f, 52.0f));
}


void bps::keyDown( KeyEvent event )
{
	if( event.getChar() == 's' ) {
		slowMotion = !slowMotion;
		if(slowMotion)
			timeStep = .05;
		else
			timeStep = 1;
	}
}

void bps::mouseDown( MouseEvent event )
{
	isMousePressed = true;
	mouse = vec2(event.getPos());
}

void bps::mouseUp( MouseEvent event )
{
	isMousePressed = false;
}

void bps::mouseDrag( MouseEvent event )
{
	mouse = vec2(event.getPos());
}



// This line tells Cinder to actually create the application
//CINDER_APP_NATIVE( bpsApp, RendererGl )
