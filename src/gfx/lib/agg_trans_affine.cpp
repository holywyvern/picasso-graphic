//----------------------------------------------------------------------------
// Anti-Grain Geometry (AGG) - Version 2.5
// A high quality rendering engine for C++
// Copyright (C) 2002-2006 Maxim Shemanarev
// Contact: mcseem@antigrain.com
//          mcseemagg@yahoo.com
//          http://antigrain.com
// 
// AGG is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// AGG is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with AGG; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, 
// MA 02110-1301, USA.
//----------------------------------------------------------------------------

#include "agg_trans_affine.h"



namespace agg
{

    //------------------------------------------------------------------------
    const trans_affine& trans_affine::parl_to_parl(const float* src, 
                                                   const float* dst)
    {
        sx  = src[2] - src[0];
        shy = src[3] - src[1];
        shx = src[4] - src[0];
        sy  = src[5] - src[1];
        tx  = src[0];
        ty  = src[1];
        invert();
        multiply(trans_affine(dst[2] - dst[0], dst[3] - dst[1], 
                              dst[4] - dst[0], dst[5] - dst[1],
                              dst[0], dst[1]));
        return *this;
    }

    //------------------------------------------------------------------------
    const trans_affine& trans_affine::rect_to_parl(float x1, float y1, 
                                                   float x2, float y2, 
                                                   const float* parl)
    {
        float src[6];
        src[0] = x1; src[1] = y1;
        src[2] = x2; src[3] = y1;
        src[4] = x2; src[5] = y2;
        parl_to_parl(src, parl);
        return *this;
    }

    //------------------------------------------------------------------------
    const trans_affine& trans_affine::parl_to_rect(const float* parl, 
                                                   float x1, float y1, 
                                                   float x2, float y2)
    {
        float dst[6];
        dst[0] = x1; dst[1] = y1;
        dst[2] = x2; dst[3] = y1;
        dst[4] = x2; dst[5] = y2;
        parl_to_parl(parl, dst);
        return *this;
    }

    //------------------------------------------------------------------------
    const trans_affine& trans_affine::multiply(const trans_affine& m)
    {
        float t0 = sx  * m.sx + shy * m.shx;
        float t2 = shx * m.sx + sy  * m.shx;
        float t4 = tx  * m.sx + ty  * m.shx + m.tx;
        shy = sx  * m.shy + shy * m.sy;
        sy  = shx * m.shy + sy  * m.sy;
        ty  = tx  * m.shy + ty  * m.sy + m.ty;
        sx  = t0;
        shx = t2;
        tx  = t4;
        return *this;
    }


    //------------------------------------------------------------------------
    const trans_affine& trans_affine::invert()
    {
        float d  = determinant_reciprocal();

        float t0  =  sy  * d;
               sy  =  sx  * d;
               shy = -shy * d;
               shx = -shx * d;

        float t4 = -tx * t0  - ty * shx;
               ty = -tx * shy - ty * sy;

        sx = t0;
        tx = t4;
        return *this;
    }


   //------------------------------------------------------------------------
    const trans_affine& trans_affine::flip_x()
    {
        sx  = -sx;
        shy = -shy;
        tx  = -tx;
        return *this;
    }

    //------------------------------------------------------------------------
    const trans_affine& trans_affine::flip_y()
    {
        shx = -shx;
        sy  = -sy;
        ty  = -ty;
        return *this;
    }

    //------------------------------------------------------------------------
    const trans_affine& trans_affine::reset()
    {
        sx  = sy  = 1.0; 
        shy = shx = tx = ty = 0.0;
        return *this;
    }

    //------------------------------------------------------------------------
    bool trans_affine::is_identity(float epsilon) const
    {
        return is_equal_eps(sx,  1.0f, epsilon) &&
               is_equal_eps(shy, 0.0f, epsilon) &&
               is_equal_eps(shx, 0.0f, epsilon) && 
               is_equal_eps(sy,  1.0f, epsilon) &&
               is_equal_eps(tx,  0.0f, epsilon) &&
               is_equal_eps(ty,  0.0f, epsilon);
    }

    //------------------------------------------------------------------------
    bool trans_affine::is_valid(float epsilon) const
    {
        return fabs(sx) > epsilon && fabs(sy) > epsilon;
    }

    //------------------------------------------------------------------------
    bool trans_affine::is_equal(const trans_affine& m, float epsilon) const
    {
        return is_equal_eps(sx,  m.sx,  epsilon) &&
               is_equal_eps(shy, m.shy, epsilon) &&
               is_equal_eps(shx, m.shx, epsilon) && 
               is_equal_eps(sy,  m.sy,  epsilon) &&
               is_equal_eps(tx,  m.tx,  epsilon) &&
               is_equal_eps(ty,  m.ty,  epsilon);
    }

    //------------------------------------------------------------------------
    float trans_affine::rotation() const
    {
        float x1 = 0.0;
        float y1 = 0.0;
        float x2 = 1.0;
        float y2 = 0.0;
        transform(&x1, &y1);
        transform(&x2, &y2);
        return (float)atan2(y2-y1, x2-x1);
    }

    //------------------------------------------------------------------------
    void trans_affine::translation(float* dx, float* dy) const
    {
        *dx = tx;
        *dy = ty;
    }

    //------------------------------------------------------------------------
    void trans_affine::scaling(float* x, float* y) const
    {
        float x1 = 0.0;
        float y1 = 0.0;
        float x2 = 1.0;
        float y2 = 1.0;
        trans_affine t(*this);
        t *= trans_affine_rotation(-rotation());
        t.transform(&x1, &y1);
        t.transform(&x2, &y2);
        *x = x2 - x1;
        *y = y2 - y1;
    }

   void trans_affine::shearing(float* x, float* y) const
   {
   }

}
