/*
 *  Earth.cpp
 *  Earthquake
 *
 *  Created by Robert Hodgin on 6/30/09.
 *  Copyright 2009 The Barbarian Group. All rights reserved.
 *
 */

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

#include "Roid.h"
#include "Resources.h"

using namespace ci;
using namespace ci::app;

Roid::Roid(): radius( 100 ), mMinMagToRender( 2 )
{
    init();
}

Roid::Roid(int radius, int x, int y, vec3 rotAxis, float rotSpeed):
radius( radius ), x(x), y(y){
    mRotSpeed = rotSpeed;
    mRotAxis = rotAxis;
    
    init();
}

void Roid::init()
{
    mLightDir = glm::normalize( vec3( 0.025f, 0.25f, 1.0f ) );

//    mLightDir = glm::normalize( vec3( 1.025f, 1.25f, 1.0f ) );
    
    // Load the textures for the Earth.
    auto fmt = gl::Texture2d::Format().wrap( GL_REPEAT ).mipmap().minFilter( GL_LINEAR_MIPMAP_LINEAR );
    mTexDiffuse = gl::Texture2d::create( loadImage( loadResource( RES_EARTHDIFFUSE ) ), fmt );
    mTexNormal = gl::Texture2d::create( loadImage( loadResource( RES_EARTHNORMAL ) ), fmt );
    mTexMask = gl::Texture2d::create( loadImage( loadResource( RES_EARTHMASK ) ), fmt );
    
    // Create the Earth mesh with a custom shader.
    auto earthShader = gl::GlslProg::create( loadResource( RES_PASSTHRU_VERT ), loadResource( RES_EARTH_FRAG ) );
  //  earthShader->uniform( "texDiffuse", 0 );
    earthShader->uniform( "texNormal", 1 );
    earthShader->uniform( "texMask", 2 );
    
    mEarth = gl::Batch::create( geom::Sphere().radius( radius ).subdivisions( 120 ), earthShader );
    
    //rotation = 0;
}


void Roid::update()
{
    // Rotate the roid by 0.2 degrees around the axis...
    mRotation *= rotate( toRadians( mRotSpeed ), normalize( mRotAxis ) );

    //gl::rotate(rotation);
	mEarth->getGlslProg()->uniform( "lightDir", mLightDir );
	
}

void Roid::draw()
{
    gl::pushModelMatrix();
    
    //std::cout<<"earth.draw"<<std::endl;
    
	gl::ScopedFaceCulling cull( true, GL_BACK );
	//gl::ScopedTextureBind tex0( mTexDiffuse, 0 );
	gl::ScopedTextureBind tex1( mTexNormal, 1 );
	gl::ScopedTextureBind tex2( mTexMask, 2 );

    //
    gl::translate(x,y);
    
    //apply rotation as matrix transform...
    gl::multModelMatrix( mRotation );
    mEarth->draw();
    gl::popModelMatrix();
}
