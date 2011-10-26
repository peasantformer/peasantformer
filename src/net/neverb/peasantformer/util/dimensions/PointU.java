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

import org.jbox2d.common.Vec2;

/**
 * Created
 * Date: 2011-10-18
 * Time: 11:31
 */
public class PointU {
    public Units x;
    public Units y;

    protected final static String delimiterRegex = ",";
    protected final static String matchRegex =
            "[ ]*" + "\\[" +  "[ ]*" +
            Units.unitRegex + "[ ]*" +
            delimiterRegex  + "[ ]*" +
            Units.unitRegex + "[ ]*" +
            "\\]"      +      "[ ]*";
    public static PointU parsePointU(String str) {
        if (!str.matches(matchRegex)) {
            return MetrePointU(0,1);
        }
        String substituted = str.replaceAll(matchRegex,"$1 $2").toLowerCase().trim();
        String[] tokenized = substituted.split(" ");

        if (tokenized.length != 2) {
            return MetrePointU(0,0);
        }
        Units u1 = Units.parseUnit(tokenized[0]);
        Units u2 = Units.parseUnit(tokenized[1]);

        return new PointU(u1,u2);
    }


    public void scale(float factor) {
        x = new Units(x.asRaw() * factor);
        y = new Units(y.asRaw() * factor);
    }


    public static PointU Vec2PointU(Vec2 r) {
        return MetrePointU(r.x,r.y);
    }

    public static PointU MetrePointU(float x, float y) {
        return new PointU(
                new Units(new Metre(x)),
                new Units(new Metre(y))
        );
    }
    public static PointU PixelsPointU(float x, float y) {
        return new PointU(
                new Units(new Pixels(x)),
                new Units(new Pixels(y))
        );
    }

    public  PointU(Units x, Units y) {
        this.x = x;
        this.y = y;
    }
    public PointU(Vec2 r) {
        this.x = new Units(new Metre(r.x));
        this.y = new Units(new Metre(r.y));
    }
    public PointU(PointU r) {
        this.x = r.x;
        this.y = r.y;
    }
    public void move(PointU shift) {
        this.x = new Units(x.asRaw() + shift.x.asRaw());
        this.y = new Units(y.asRaw() + shift.y.asRaw());
    }
    public void unmove(PointU shift) {
        this.x = new Units(x.asRaw() - shift.x.asRaw());
        this.y = new Units(y.asRaw() - shift.y.asRaw());
    }

    public Vec2 asVec2() {
        return new Vec2(x.asMetre(),y.asMetre());
    }

    @Override
    public String toString() {
        return "[" + x.asRaw() + ", " + y.asRaw() + "]";
    }

    public void multiply(int i) {
        x = new Units(x.asRaw() * i);
        y = new Units(y.asRaw() * i);
    }

    public void set(PointU port) {
        x = port.x;
        y = port.y;
    }

}
