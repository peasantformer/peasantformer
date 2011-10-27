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

package net.neverb.peasantformer.interfaces.objects.parser.nodes;

import java.util.Collection;
import java.util.HashMap;
import java.util.List;

/**
 * Author: Alexander Tumin
 * Created: 2011-10-26 13:10
 */
public class GroupNode extends RootNode {
    protected HashMap<String,RootNode> childs =
            new HashMap<String, RootNode>();


    public GroupNode(String name, List<RootNode> ch) {
        super(name);
        addNodes(ch);
    }

    public GroupNode(GroupNode node) {
        super(node.getName());
        addNodes(node.getChilds());
    }


    public void addNodes(List<RootNode> ch) {
        if (ch != null) {
            for (RootNode n : ch) {
                childs.put(n.getName(),n);
            }
        }
    }
    public void addNodes(Collection<RootNode> ch) {
        if (ch != null) {
            for (RootNode n : ch) {
                childs.put(n.getName(),n);
            }
        }
    }




    public void addNode(String name, RootNode node) {
        childs.put(name,node);
    }


    public Collection<RootNode> getChilds() {
        return childs.values();
    }
    public RootNode getChild(String name) {
        return childs.get(name);
    }

    @Override
    public Type getType() {
        return Type.GROUP;
    }

    @Override
    public Object getData() {
        return childs;
    }


    @Override
    public String toString() {
        StringBuilder res = new StringBuilder(name);
        res.append("@");
        res.append(getType());
        res.append('[');
        for (RootNode n : childs.values()) {

            res.append(" ").append(n.toString()).append(" ");
        }
        res.append(']');
        return res.toString().replace("  ",", ");
    }
}
