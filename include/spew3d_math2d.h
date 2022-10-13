/* Copyright (c) 2020-2022, ellie/@ell1e & Spew3D Team (see AUTHORS.md).

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

Alternatively, at your option, this file is offered under the Apache 2
license, see accompanied LICENSE.md.
*/

#ifndef SPEW3D_MATH2D_H_
#define SPEW3D_MATH2D_H_

typedef struct spew3d_point spew3d_point;
struct spew3d_point {
    double x, y;
};


#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

static inline void spew3d_math2d_rotate(
        spew3d_point *p, double degree
        ) {
    /// Rotate a given point around its origin by the given degree.
    /// Positive angle gives CW (clockwise) rotation.
    /// X is right, Y is down.
    degree = (degree / 180.0) * M_PI;
    double newy = (p->y) * cos(degree) + (p->x) * sin(degree);
    double newx = (p->x) * cos(degree) - (p->y) * sin(degree);
    p->x = newx;
    p->y = newy;
}


static inline void spew3d_math2d_rotatecenter(
        spew3d_point *p, double degree,
        spew3d_point center
        ) {
    p->x -= center.x;
    p->y -= center.y;
    spew3d_math2d_rotate(p, degree);
    p->x += center.x;
    p->y += center.y;
}


static inline double spew3d_math2d_angle(
        spew3d_point *p
        ) {
    /// Return the angle of a point's origin to the point.
    /// Angles: (1.0, 0.0) returns 0 degrees angle,
    /// CW rotation increases angle. X is right, Y is down,
    /// (0.0, 1.0) returns 90 degrees angle.
    return ((atan2(p->y, p->x) / M_PI) * 180.0);
}

#endif  // SPEW3D_MATH2D_H_

