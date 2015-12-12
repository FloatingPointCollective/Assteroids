#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

#include "Roid.h"
#include "POV.h"
#include "bps.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class AssteroidsApp : public App {
  public:
    static void prepareSettings( Settings *settings );
    void addRoid(int radius, int x, int y, vec3 rAxis, float rSpeed);

	void setup() override;
	void mouseDown( MouseEvent event ) override;
    void mouseUp( MouseEvent event ) override;
    void mouseDrag( MouseEvent event ) override;
    void keyDown( KeyEvent event ) override;
	void update() override;
	void draw() override;
    vector<Roid> roids;

    
    POV     mPov;
    //Roid    mRoid, mRoid2, mRoid3;
    bps     mBPS;
    
};

void AssteroidsApp::prepareSettings( Settings *settings )
{
    
    settings->setWindowSize( 1280, 800 );
    settings->disableFrameRate();
    settings->setResizable( true );
    settings->setFullScreen( true );
    
}

void AssteroidsApp::setup()
{
    
    hideCursor();
    
    addRoid(130, -200, -180, randVec3(), randFloat(-1,1)/2);
    addRoid(120, 100, -150, randVec3(), randFloat(-1,1)/2);
    //addRoid(60, 0, -50, randVec3(), randFloat(-1,1)/2);
    
    //addRoid(100, 0, 0, randVec3(), randFloat(-1,1)/2);

  //  mRoid = Roid(50, 50, 50, randVec3(), randFloat(-1,1)/2);
    
    //mRoid2 = Roid(50,-100,-100, randVec3(), randFloat(-1,1)/2);
    //mRoid3 = Roid(50,-80,30, randVec3(), randFloat(-1,1)/2);
    
    // Create the camera controller.
    mPov = POV( this, ci::vec3( 0.0f, 0.0f, 1000.0f ), ci::vec3( 0.0f, 0.0f, 0.0f ) );

    mBPS.setup(2, & roids);
    
}

void AssteroidsApp::addRoid(int radius, int x, int y, vec3 rAxis, float rSpeed){
    Roid roid = Roid(radius, x, y, rAxis, rSpeed);
    roids.push_back(roid);
}

void AssteroidsApp::update()
{
    mBPS.update();

    mPov.update();
    
    for (auto &roid : roids)
    {
        roid.update();
    }
    
   // mRoid.update();
  //  mRoid2.update();
  //  mRoid3.update();
}

void AssteroidsApp::draw()
{
    
    gl::clear( Color( 0, 0, 0 ) );

    //gl::ScopedDepth       depth( true );
    gl::ScopedColor       color( 1, 1, 1 );
    
    // Draw roid.
    for (auto &roid : roids)
    {
        roid.draw();
    }
    
   // mRoid.draw();
   // mRoid2.draw();
   // mRoid3.draw();

    //draw particle system
    gl::translate(-getWindowWidth()/2,-getWindowHeight()/2);
    mBPS.draw();
    
}

void AssteroidsApp::mouseDown( MouseEvent event )
{
    mBPS.mouseDown(event);
}

void AssteroidsApp::mouseUp( MouseEvent event )
{
    mBPS.mouseUp(event);
}

void AssteroidsApp::mouseDrag( MouseEvent event )
{
    mBPS.mouseDrag(event);
}

void AssteroidsApp::keyDown( KeyEvent event ) {
    if( event.getChar() == '=' ){
        mPov.adjustDist(5);
    } else if( event.getChar() == '-' ){
        mPov.adjustDist(-5);
    }
}

//CINDER_APP( AssteroidsApp, RendererGl )
CINDER_APP( AssteroidsApp, RendererGl( RendererGl::Options().msaa( 16 ) ), &AssteroidsApp::prepareSettings )

