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

package net.neverb.peasantformer.interfaces.objects.parser.mouse;

import mouse.runtime.SemanticsBase;
import net.neverb.peasantformer.interfaces.objects.parser.nodes.RootNode;
import net.neverb.peasantformer.interfaces.objects.parser.nodes.primitive.*;
import net.neverb.peasantformer.util.dimensions.PointU;
import net.neverb.peasantformer.util.dimensions.Units;

import java.util.LinkedList;

/**
 * Author: Alexander Tumin
 * Created: 2011-10-25 17:06
 */
public class MouseParserSymantics extends SemanticsBase {
    protected enum HelperDataType {
        NUMBER,
        POINT,
        STRING,
        ARRAY,
        GROUP
    }
    protected abstract class HelperDataHolder {
        public abstract HelperDataType getType();
    }
    protected class HelperDataHolderNumber extends HelperDataHolder {
        public final Units data;

        public HelperDataHolderNumber(Units data) {
            this.data = data;
        }
        @Override
        public HelperDataType getType() {
            return HelperDataType.NUMBER;
        }
    }
    protected class HelperDataHolderString extends HelperDataHolder {
        public final String data;

        public HelperDataHolderString(String data) {
            this.data = data;
        }

        @Override
        public HelperDataType getType() {
            return HelperDataType.STRING;
        }
    }
    protected class HelperDataHolderPoint extends HelperDataHolder {
        public final PointU data;

        public HelperDataHolderPoint(PointU data) {
            this.data = data;
        }

        @Override
        public HelperDataType getType() {
            return HelperDataType.POINT;
        }
    }
    protected class HelperDataHolderArray extends HelperDataHolder {
        protected final PointU[][] data;

        public HelperDataHolderArray(PointU[][] data) {
            this.data = data;
        }

        @Override
        public HelperDataType getType() {
            return HelperDataType.ARRAY;
        }
    }

    protected class HelperDataHolderAnonymousGroup extends HelperDataHolder {
        protected final LinkedList<RootNode> data =
                new LinkedList<RootNode>();

        @Override
        public HelperDataType getType() {
            return HelperDataType.GROUP;
        }
    }

    void string() {
        HelperDataHolderString string = new HelperDataHolderString(
                rhs(1).text().replaceAll("\\\\\"","\"")
        );
        lhs().put(string);
    }

    void number() {
        System.out.println(rhs(0).text());
        HelperDataHolderNumber number = new HelperDataHolderNumber(
                Units.parseUnit(rhs(0).text())
        );
        lhs().put(number);
    }

    void point() {
        Units u1 = Units.parseUnit(rhs(1).text());
        Units u2 = Units.parseUnit(rhs(3).text());
        PointU p = new PointU(u1,u2);
        HelperDataHolderPoint point = new HelperDataHolderPoint(p);
        lhs().put(point);
    }


    void point1d() {
        PointU[] points = new PointU[(rhsSize()-1)/2];
        int n = 0;
        for (int i = 1; i < rhsSize()-1; i+=2) {
            points[n++] = ((HelperDataHolderPoint)rhs(i).get()).data;
        }
        lhs().put(points);
    }

    void point2d() {
        PointU[][] points = new PointU[(rhsSize()-1)/2][];
        int n = 0;
        for (int i = 1; i < rhsSize()-1; i+=2) {
            points[n++] = (PointU[]) rhs(i).get();
        }
        HelperDataHolderArray array = new HelperDataHolderArray(points);
        lhs().put(array);

    }

    void property() {
        String name = rhs(0).text().trim();
        HelperDataHolder holder = (HelperDataHolder) rhs(2).get();

        switch (holder.getType()) {
            case NUMBER:
                HelperDataHolderNumber number = (HelperDataHolderNumber) holder;
                lhs().put(new NumberNode(name,number.data));
                break;
            case POINT:
                HelperDataHolderPoint point = (HelperDataHolderPoint) holder;
                lhs().put(new PointNode(name,point.data));
                break;
            case STRING:
                HelperDataHolderString string = (HelperDataHolderString) holder;
                lhs().put(new StringNode(name,string.data));
                break;
            case ARRAY:
                HelperDataHolderArray array = (HelperDataHolderArray) holder;
                lhs().put(new PointArrayNode(name,array.data));
                break;
            case GROUP:
                /* do nothing */
                break;
        }


    }

    void propertylist() {
        HelperDataHolderAnonymousGroup group = new HelperDataHolderAnonymousGroup();
        for (int i = 0; i < rhsSize(); i++) {
            group.data.add((RootNode) rhs(i).get());
        }
        lhs().put(group);

    }

    void group() {
        String name = rhs(0).text().trim();
        RootNode data = new GroupNode(name, ((HelperDataHolderAnonymousGroup)rhs(2).get()).data);
        lhs().put(data);
    }

    void config() {
        for (int i = 0; i < rhsSize(); i++) {
            System.out.println("group: " + rhs(i).text().trim() + " | " + rhs(i).get());
        }
    }
}
