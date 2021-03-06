/*
 * Copyright 2020 Bryson Banks, David Campbell, and Jeffrey Nelson.  All rights reserved.
 *
 * BarnesHutSimulation: Body.h
 */

#ifndef _BODY_DEFINED
#define _BODY_DEFINED

#include <iostream>
#include <cmath>
#include <tuple>
#include <fstream>

typedef std::tuple<double, double, double> vector_3d;

constexpr int X = 0;  // for indexing the x value in a vector_3d
constexpr int Y = 1;  // for indexing the y value in a vector_3d
constexpr int Z = 2;  // for indexing the z value in a vector_3d

constexpr double xScale = 1.0;  // scaling factor for x plane
constexpr double yScale = 1.0;  // scaling factor for y plane
constexpr double zScale = 1.0;  // scaling factor for z plane

constexpr double G = 0.00000000006674;  // gravitational constant

/* Generate a new 3-dimensional zero vector of type vector_3d */
inline vector_3d zero_vect() {
    return (vector_3d)std::make_tuple(0.0, 0.0, 0.0);
}

class Body {

public:
    int id;         // id of body
    double mass;    // mass in kg
    vector_3d pos;  // center of mass position vector: <x, y, z>
    vector_3d acc;  // acceleration vector: <a_x, a_y, a_z>
    vector_3d vel;  // velocity vector: <v_x, v_y, v_z>

    Body();
    Body(int id, double mass, const vector_3d& pos);
    Body(int id, double mass, const vector_3d& pos, const vector_3d& acc, const vector_3d& vel);

    /* Override "<<" operator for printing body details to I/O output stream */
    friend std::ostream& operator<<(std::ostream& out, const Body& b);

    /* Calculate the distance between this body and body b */
    double distance(const Body& b);

    /* Calculate the gravitational force vector on this body produced by body b */
    vector_3d force(const Body& b);

    /* Apply force vector on this body by updating acceleration vector */
    void apply(const vector_3d f);

    /* Simulate movement of this body for t seconds given initial acceleration and velocity */
    void move(double t);

    /* Logs this body to a file */
    void logBody(std::ofstream& f);

};

/* Return a new body representing the center of mass for the given array of bodies */
inline Body center_of_mass(Body bodies[], int n) {
    // x_cm = ((m_1 * x_1) + (m_2 * x_2) + ... ) / (m_1 + m_2 + ... )
    double totalMass = 0;
    vector_3d cmPos = zero_vect();
    for (int i = 0; i < n; i++) {
        std::get<X>(cmPos) += (bodies[i].mass * std::get<X>(bodies[i].pos));
        std::get<Y>(cmPos) += (bodies[i].mass * std::get<Y>(bodies[i].pos));
        std::get<Z>(cmPos) += (bodies[i].mass * std::get<Z>(bodies[i].pos));
        totalMass += bodies[i].mass;
    }
    std::get<X>(cmPos) /= totalMass;
    std::get<Y>(cmPos) /= totalMass;
    std::get<Z>(cmPos) /= totalMass;
    Body b = Body(0, totalMass, cmPos, zero_vect(), zero_vect());
    return b;
}

/* From an input file, ingests the next Body from the file (dynamically allocated) */
inline Body getBody(std::ifstream& f) {
    int id;
    double mass;
    double posx, posy, posz;
    double accx, accy, accz;
    double velx, vely, velz;
    // read in values from input file stream
    f >> id >> mass;
    f >> posx >> posy >> posz;
    f >> accx >> accy >> accz;
    f >> velx >> vely >> velz;
    // generate and return new body with parsed values
    vector_3d pos = std::make_tuple(posx, posy, posz);
    vector_3d acc = std::make_tuple(accx, accy, accz);
    vector_3d vel = std::make_tuple(velx, vely, velz);
    return Body(id, mass, pos, acc, vel);
}

#endif // _BODY_DEFINED
