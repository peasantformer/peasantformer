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

package net.neverb.peasantformer.util.dimensions;

import android.graphics.RectF;

/**
 * Created
 * Date: 2011-10-18
 * Time: 11:33
 */
public class RectU {
    public Units left;
    public Units top;
    public Units right;
    public Units bottom;

    protected final static String delimiterRegex = "[ ]*,[ ]*";
    protected final static String matchRegex =
            "RectU[ ]*\\(" +
            Units.unitRegex +
            delimiterRegex  +
            Units.unitRegex +
            delimiterRegex  +
            Units.unitRegex +
            delimiterRegex  +
            Units.unitRegex +
            "\\)";
    public static RectU parsePointU(String str) {
        if (!str.matches(matchRegex)) {
            return MetreRectU(0,0,0,0);
        }
        String substituted = str.replaceAll(
                matchRegex,
                "$1 $2 $3 $4").toLowerCase();
        String[] tokenized = substituted.split(" ");

        if (tokenized.length != 4) {
            return MetreRectU(0,0,0,0);
        }
        Units u1 = Units.parseUnit(tokenized[0]);
        Units u2 = Units.parseUnit(tokenized[1]);
        Units u3 = Units.parseUnit(tokenized[2]);
        Units u4 = Units.parseUnit(tokenized[3]);

        return new RectU(u1,u2,u3,u4);
    }


    public static RectU MetreRectU(
            float left, float top, float right, float bottom) {
        return new RectU(
                new Units(new Metre(left)), new Units(new Metre(top)),
                new Units(new Metre(right)), new Units(new Metre(bottom))
        );
    }
    public static RectU PixelsRectU(
            float left, float top, float right, float bottom) {
        return new RectU(
                new Units(new Pixels(left)), new Units(new Pixels(top)),
                new Units(new Pixels(right)), new Units(new Pixels(bottom))
        );
    }


    public RectU(Units left, Units top, Units right, Units bottom) {
        this.left = left;
        this.top = top;
        this.right = right;
        this.bottom = bottom;
    }
    public RectU(RectU r) {
        this.left = r.left;
        this.top = r.top;
        this.right = r.right;
        this.bottom = r.bottom;
    }
    public void move(PointU shift) {
        left = new Units(left.asRaw() + shift.x.asRaw());
	    right = new Units(right.asRaw() + shift.x.asRaw());
	    top = new Units(top.asRaw() + shift.y.asRaw());
	    bottom = new Units(bottom.asRaw() + shift.y.asRaw());
    }
    public void unmove(PointU shift) {
        left = new Units(left.asRaw() - shift.x.asRaw());
	    right = new Units(right.asRaw() - shift.x.asRaw());
	    top = new Units(top.asRaw() - shift.y.asRaw());
	    bottom = new Units(bottom.asRaw() - shift.y.asRaw());
    }

    public float width() {
        return asRawRect().width();
    }

    public float height() {
        return asRawRect().height();
    }

    public void stretch(PointU to) {
        this.right = to.x;
        this.top = to.y;
    }

    public float metreWidth() {
        return new Units(width()).asMetre();
    }

    public float metreHeight() {
        return new Units(height()).asMetre();
    }

    public RectF asRawRect() {
        return new RectF(
                left.asRaw(), top.asRaw(),
                right.asRaw(),bottom.asRaw()
                );
    }
    public RectF asPositiveRawRect() {
        RectF r = new RectF(
                left.asRaw(), top.asRaw(),
                right.asRaw(),bottom.asRaw()
                );
        if (r.left < 0) {
            r.right += r.left * -1;
            r.left = 0;
        }
        if (r.top < 0) {
            r.bottom += r.top * -1;
            r.top = 0;
        }
        return r;
    }

    public PointU[] asPointUArray() {
        PointU[] result = new PointU[4];
        result[0] = new PointU(left,bottom);
        result[1] = new PointU(right,bottom);
        result[2] = new PointU(right,top);
        result[3] = new PointU(left,top);


        return result;
    }

    public boolean contains(PointU p) {
        return asRawRect().contains(p.x.asRaw(),p.y.asRaw());
    }
    public boolean intersects(RectU r) {
        RectF a = this.asRawRect();
        RectF b = r.asRawRect();
        return !(a.left > b.right ||
                 a.right < b.left ||
                 a.bottom > b.bottom ||
                 a.top < b.top);
    }

    @Override
    public String toString() {
        return "RectU (" +
                left.asRaw() +
                ", " +
                top.asRaw() +
                ", " +
                right.asRaw() +
                ", " +
                bottom.asRaw() +
                ")";
    }

    public PointU position() {
        return new PointU(
                left,
                bottom
        );
    }

}
