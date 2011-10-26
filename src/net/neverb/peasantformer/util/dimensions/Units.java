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

/**
 * Created
 * Date: 2011-10-18
 * Time: 11:18
 */
public class Units extends DimensionUnit {
    protected final static String floatRegex = "[0-9.-]*";
    protected final static String suffixRegex = "[fmpr]?";
    protected final static String unitRegex =
            "(" +
            floatRegex +
            suffixRegex +
            ")";


    public static Units parseUnit(String num) {
        String[] pair = num.replaceAll(
                "(" + floatRegex + ")(" + suffixRegex + ")",
                "$1 $2"
        ).split(" ");
        if (pair.length < 1 || pair.length > 2) {
            return new Units(0);
        }
        float val = Float.parseFloat(pair[0]);
        Character modifier = 'm';
        if (pair.length == 2) {
            modifier = pair[1].charAt(0);
        }
        switch (modifier) {
            default:
                val *= 0;
                break;
            case 'r':
            case 'f':
                /* do nothing - already got raw value */
                break;
            case 'p':
                val *= Ratios.PIXEL_RATIO;
                break;
            case 'm':
                val *= Ratios.METRE_RATIO;
                break;
        }
        return new Units(val);
    }

    public Units(float raw) {
        super(raw);
    }
    public Units(Pixels px) {
        super(px.asFloat() * Ratios.PIXEL_RATIO);
    }
    public Units(Metre mt) {
        super(mt.asFloat() * Ratios.METRE_RATIO);
    }

    public float asRaw() {
        return value;
    }
    public float asPixels() {
        return value / Ratios.PIXEL_RATIO;
    }
    public float asMetre() {
        return value / Ratios.METRE_RATIO;
    }
    public String toString() {
        return "(" + (value /  Ratios.PIXEL_RATIO) + "px " + (value /  Ratios.METRE_RATIO) + "metre)";
    }
}
