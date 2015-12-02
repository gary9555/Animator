#pragma warning(disable : 4786)

#include "particleSystem.h"

#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <vector>
#include <FL/gl.h>
#include "modelerdraw.h"

/***************
 * Constructors
 ***************/

ParticleSystem::ParticleSystem(float gravity, float friction)
{
	// TODO
	srand(time(0));
	forces.push_back(new Gravity(Vec3f(0, -gravity, 0)));
	forces.push_back(new Friction(friction));
}





/*************
 * Destructor
 *************/

ParticleSystem::~ParticleSystem() 
{
	// TODO
	particles.clear();
	forces.clear();
}


/******************
 * Simulation fxns
 ******************/

/** Start the simulation */
void ParticleSystem::startSimulation(float t)
{
    
	// TODO

	// These values are used by the UI ...
	// -ve bake_end_time indicates that simulation
	// is still progressing, and allows the
	// indicator window above the time slider
	// to correctly show the "baked" region
	// in grey.
	bake_end_time = -1;
	simulate = true;
	dirty = true;

}

/** Stop the simulation */
void ParticleSystem::stopSimulation(float t)
{
    
	// TODO

	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Reset the simulation */
void ParticleSystem::resetSimulation(float t)
{
    
	// TODO

	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Compute forces and update particles **/
void ParticleSystem::computeForcesAndUpdateParticles(float t)
{

	// TODO
	bake_fps = t - currentT;
	currentT = t;
	if (simulate) {
		if (!isBaked(t)) {
			for (vector<Particle>::iterator it = particles.begin(); it != particles.end(); it++) {
				it->nextPos(bake_fps);
			}
			bakeParticles(t);
		}
		else {
			particles = bakeInfo[t];
		}
	}
}


/** Render particles */
void ParticleSystem::drawParticles(float t)
{
	// TODO
	if (simulate) {
		for (vector<Particle>::iterator it = bakeInfo[t].begin(); it != bakeInfo[t].end(); it++) {
			it->draw();
		}
	}
}





/** Adds the current configuration of particles to
  * your data structure for storing baked particles **/
void ParticleSystem::bakeParticles(float t) 
{
	// TODO
	bakeInfo.insert(pair<float, vector<Particle>>(t, particles));
}

/** Clears out your data structure of baked particles */
void ParticleSystem::clearBaked()
{
	// TODO
	bakeInfo.clear();
}

void ParticleSystem::SpawnParticles(Vec3f pos, int num) {
	if (simulate) {
		if (!isBaked(currentT + bake_fps)) {
			for (int i = 0; i < num; i++) {
				Particle p(Vec3f(pos[0], pos[1], pos[2]), 1);

				float F = rand() % 10 / 10.0 + 0.2;
				float theta = rand() % 360 / 57.3;

				float xSpeed = rand() % 10 / 10.0 + 2;
				float ySpeed = cos(theta) * F;
				float zSpeed = sin(theta) * F;

				p.setSpeed(Vec3f(xSpeed, ySpeed, zSpeed));

				for (vector<Force*>::iterator it = forces.begin(); it != forces.end(); it++) {
					p.add_force(*it);
				}

				particles.push_back(p);
			}
		}
	}
}

bool ParticleSystem::isBaked(float time) {
	map<float, vector<Particle>>::iterator it = bakeInfo.find(time);
	return (it != bakeInfo.end());
}


void Particle::add_force(Force* f) {
	forces.push_back(f);
}

void Particle::nextPos(float deltaT) {
	for (vector<Force*>::iterator it = forces.begin(); it != forces.end(); it++)
		(*it)->addForce(speed, deltaT, mass);
	position += speed * deltaT;
}

void Particle::draw() {
	setDiffuseColor(1,0,0);
	glPushMatrix();
	glBegin(GL_POINTS);
	glVertex3f(position[0], position[1], position[2]);
	glEnd();
	glPopMatrix();
}


void Gravity::addForce(Vec3f& speed, float deltaT, float mass) {
	speed += g * deltaT;
}

// the air friction
void Friction::addForce(Vec3f& speed, float deltaT, float mass) {
	float a = F / mass * deltaT;
	for (int i = 0; i < 3; i++){
		if (speed[i] < 0){
			speed[i] += a;
			if (speed[i] > 0)
				speed[i] = 0;
		}
		else{
			speed[i] -= a;
			if (speed[i] < 0)
				speed[i] = 0;
		}
	}
}




