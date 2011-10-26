/*
 Copyright (c) 2011, Alexander <itakingiteasy> Tumin
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
     * Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
     * Neither the name of the organization nor the
       names of its contributors may be used to endorse or promote products
       derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

package net.neverb.peasantformer.util.generators;

import net.neverb.peasantformer.util.dimensions.RectU;

/**
 * Created
 * Date: 2011-10-18
 * Time: 20:44
 */
public class GenVertices {
    public static float[] rectangleClockwise(int row, int col, float rowlen,
                                     float collen, float width, float height) {
            return new float[] {
                    row * rowlen,         col * collen + height,
                    row * rowlen + width, col * collen + height,
                    row * rowlen,         col * collen,
                    row * rowlen + width, col * collen

            };
    }
    public static float[] rectangleCounterClockwise(int row, int col,
                                                    float rowlen, float collen,
                                                    float width, float height) {
            return new float[] {
                    row * rowlen + width, col * collen,
                    row * rowlen,         col * collen,
                    row * rowlen + width, col * collen + height,
                    row * rowlen,         col * collen + height

            };
    }

    public static float[] rectangleClockwise(float left, float top,
                                             float right,float bottom) {
        return new float[] {
                left,  bottom,
                right, bottom,
                left,  top,
                right, top
        };
    }
    public static float[] rectangleCounterClockwise(float left, float top,
                                                    float right, float bottom) {
        return new float[] {
                right, top,
                left,  top,
                right, bottom,
                left,  bottom
        };
    }


    public static float[] rectangleClockwise(RectU geometry) {
        return new float[] {
                geometry.left.asPixels(),  geometry.bottom.asPixels(),
                geometry.right.asPixels(), geometry.bottom.asPixels(),
                geometry.left.asPixels(),  geometry.top.asPixels(),
                geometry.right.asPixels(), geometry.top.asPixels()
        };
    }
    public static float[] rectangleCounterClockwise(RectU geometry) {
        return new float[] {
                geometry.right.asPixels(), geometry.top.asPixels(),
                geometry.left.asPixels(),  geometry.top.asPixels(),
                geometry.right.asPixels(), geometry.bottom.asPixels(),
                geometry.left.asPixels(),  geometry.bottom.asPixels()

        };
    }
    public static float[] circleClockwise(float radius, int vertices) {
	    float[] geo = new float[vertices];
        for (int i = 0; (i+1) < vertices; i+=2) {
            geo[i]   = (float)(Math.sin(i * Math.PI / 180) * radius);
            geo[i+1] = (float)(Math.cos(i * Math.PI / 180) * radius);
        }
        return geo;
    }

}
