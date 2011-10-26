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

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ANDCONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICTLIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAYOUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

package net.neverb.peasantformer.interfaces.objects.parser.nodes.primitive;

import android.graphics.Point;
import net.neverb.peasantformer.interfaces.objects.parser.nodes.RootNode;
import net.neverb.peasantformer.util.dimensions.PointU;

import java.util.Collections;
import java.util.LinkedList;

/**
 * Author: Alexander Tumin
 * Created: 2011-10-25 12:17
 */
public class PointArrayNode extends RootNode {
    protected LinkedList<LinkedList<PointU>> points =
            new LinkedList<LinkedList<PointU>>();

    public PointArrayNode(String name, PointU[][] pts) {
        super(name);
        addGroup(pts);
    }


    public void addGroup(PointU[][] pts) {
        for (PointU[] ps : pts) {
            LinkedList<PointU> prs = new LinkedList<PointU>();
            Collections.addAll(prs, ps);
            points.add(prs);
        }
    }

    public LinkedList<LinkedList<PointU>> getPoints() {
        return points;
    }


    @Override
    public Type getType() {
        return Type.POINTARRAY;
    }
}
