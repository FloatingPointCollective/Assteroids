#include "ParticleSystem.h"

#include "cinder/app/App.h"


#include "cinder/Utilities.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "cinder/Perlin.h"

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
    vec2 getField(vec2 position);
	
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
    
    Perlin				mPerlin;
    float				mAnimationCounter;

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
	float maxVelocity = .05;
	for(int i = 0; i < kParticles * 1024; i++) {
		float x = Rand::randFloat(padding, getWindowWidth() - padding);
		float y = Rand::randFloat(padding, getWindowHeight() - padding);
		float xv = Rand::randFloat(-maxVelocity, maxVelocity);
		float yv = Rand::randFloat(-maxVelocity, maxVelocity);
		Particle particle(x, y, xv, yv);
		particleSystem.add(particle);
	}
	
	timeStep = 1;
	lineOpacity = 0.1f;
	pointOpacity = 0.5f;
	slowMotion = false;
	particleNeighborhood = 10;
	particleRepulsion = .5;
	centerAttraction = .05;
    
    
    mAnimationCounter = 0.0f;

}

void bps::update(){
	particleSystem.setTimeStep(timeStep);
    // Move ahead in time, which becomes the z-axis of our 3D noise.
    mAnimationCounter += 10.0f;
}

void bps::draw()
{
    
	//gl::clear();
    
    //gl::color( 0, 0, 0, .5 );
    //gl::drawSolidRect( Rectf( 0,0, getWindowWidth(), getWindowHeight()));
    
    //gl::enableAlphaBlending();
    //gl::color(ColorA( 0, 0, 0, 0.9f));
    //gl::drawSolidRect(getWindowBounds());
    
	gl::enableAdditiveBlending();
    gl::color(1.0f, 1.0f, 1.0f, lineOpacity);
	
	particleSystem.setupForces();
    vec2 pos;
    
    // forces on this particle
    int padding = 200;
    int left = padding;
    int top = padding;
    int right = getWindowWidth()-padding;
    int bottom = getWindowHeight()-padding;

    
	// apply per-particle forces
    gl::begin(GL_LINES);
	for(int i = 0; i < particleSystem.size(); i++) {
		Particle& cur = particleSystem[i];
		// global force on other particles
		particleSystem.addRepulsionForce(cur, particleNeighborhood, particleRepulsion);
        
        //apply noise field force to the particle
        /*pos.x = cur.x;
        pos.y = cur.y;
        cur.applyForce(getField(pos));*/
        
        // Add some perlin noise to the velocity.
        vec3 deriv = mPerlin.dfBm( vec3( cur.x, cur.y, mAnimationCounter ) * 0.001f );
        //particle.mZ = deriv.z;
       // vec2 deriv2 = normalize( vec2( deriv.x, deriv.y ) );
        int softness = 50;
        vec2 deriv2 = vec2( (deriv.x+1)/softness, deriv.y/softness );
        cur.applyForce(deriv2);
        //particle.mVelocity += deriv2 * mSpeed;
        
        cur.loopAround(padding, padding, getWindowWidth()-padding, getWindowHeight()-padding);
        
        cur.addDampingForce();

	}
    gl::end();
    
    //add repulsion forces for each assteroid
    for (auto roid : *mRoids)
    {
       // cout<<"roid.x"<<roid.x<<endl;
       // cout<<"roid.y"<<roid.y<<endl;
        particleSystem.addRepulsionForce(roid.x+getWindowWidth()/2, roid.y+getWindowHeight()/2, roid.radius+20, 1);
    }
    
    //add repulsive force for 4 corners
    int size = getWindowWidth()/10;
    int power = 1;
    /*particleSystem.addRepulsionForce(left,top,size, power);
    particleSystem.addRepulsionForce(right,top,size, power);
    particleSystem.addRepulsionForce(left,bottom,size, power);
    particleSystem.addRepulsionForce(right,bottom,size, power);*/
    
	// single global forces
	//particleSystem.addAttractionForce(getWindowWidth()/2, getWindowHeight()/2, getWindowWidth(), centerAttraction);
    
	if(isMousePressed)
		particleSystem.addAttractionForce(mouse.x, mouse.y, 200, .5);
    
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
