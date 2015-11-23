#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "Earth.h"
#include "POV.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class AssteroidsApp : public App {
  public:
    static void prepareSettings( Settings *settings );

	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
    
    POV               mPov;
    Earth             mEarth;
    
};

void AssteroidsApp::prepareSettings( Settings *settings )
{
    settings->setWindowSize( 1024, 768 );
    settings->disableFrameRate();
    settings->setResizable( true );
    settings->setFullScreen( false );
}

void AssteroidsApp::setup()
{
    
    // Create the camera controller.
    mPov = POV( this, ci::vec3( 0.0f, 0.0f, 1000.0f ), ci::vec3( 0.0f, 0.0f, 0.0f ) );

    
}

void AssteroidsApp::mouseDown( MouseEvent event )
{
}

void AssteroidsApp::update()
{
    mPov.update();
    mEarth.update();
}

void AssteroidsApp::draw()
{
    
    gl::clear( Color( 0, 0, 0 ) );

    gl::ScopedDepth       depth( true );
    gl::ScopedColor       color( 1, 1, 1 );

    
    // Draw roid.
    mEarth.draw();
    
}

//CINDER_APP( AssteroidsApp, RendererGl )
CINDER_APP( AssteroidsApp, RendererGl( RendererGl::Options().msaa( 16 ) ), &AssteroidsApp::prepareSettings )

