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

package net.neverb.peasantformer.sceneparser;

import android.content.Context;
import android.util.Config;
import android.util.Log;
import net.neverb.peasantformer.definitions.GenericWorldDef;
import net.neverb.peasantformer.sceneparser.nodes.GroupNode;
import net.neverb.peasantformer.sceneparser.nodes.RootNode;
import net.neverb.peasantformer.util.dimensions.PointU;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.ThreadPoolExecutor;

/**
 * Author: Alexander Tumin
 * Created: 2011-10-27 15:44
 */
public class ConfigParser {
    protected abstract class ActionElem {

    }

    public class ConfigParserException extends RuntimeException {
        public ConfigParserException() {
            super();
        }

    }
    public RootNode rootnode = null;
    public LinkedList<String> errors =
            new LinkedList<String>();

    protected HashMap<String,Runnable> objectParsers =
            new HashMap<String, Runnable>();

    protected HashMap<String,Runnable> groupParsers  =
            new HashMap<String, Runnable>();

    protected HashMap<String,Runnable> forceParsers  =
            new HashMap<String, Runnable>();

    protected HashMap<String,Runnable> jointParsers  =
            new HashMap<String, Runnable>();



    protected void init() {

    }

    public ConfigParser(String s) {
        ConfigLexer lexer = new ConfigLexer(s,errors);
        rootnode = lexer.lex();
        init();
    }
    public ConfigParser(Context c) {
        ConfigLexer lexer = new ConfigLexer(c,errors);
        rootnode = lexer.lex();
        init();
    }


    protected RootNode extractNode(GroupNode group, String nodename, RootNode.Type t) {
        return extractNode(group,nodename,t,true);
    }
    protected RootNode extractNode(GroupNode group, String nodename, RootNode.Type t,boolean strict) {
        if (group == null) {
            return null;
        }
        try {
            RootNode node = group.getChild(nodename);
            if (strict) {
                if (node.getType() != t) {
                    errors.add(
                            "Parser: Type of " +
                                    nodename +
                                    " @ " +
                                    group.getName() +
                                    " is " + node.getType() +
                                    ". Expected: " +
                                    t.toString());
                    return null;
                }

            }
            return node;
        } catch (GroupNode.NoSuchNodeException e) {
            if (strict) {
                e.printStackTrace();
                errors.add(
                        "Parser: Failed to get node " +
                                nodename +
                                " @ " +
                                "group.getName()" +
                                " (Missing node)");
            }
            return null;
        }
    }

    public GenericWorldDef parse() {
        GenericWorldDef worlddef;
        if (errors.size() > 0 || rootnode == null) {
            errors.add("Parser: Lexer failed with given file");
            return null;
        }
        if (rootnode.getType() != RootNode.Type.GROUP) {
            errors.add("Parser: Root node is not a group");
        }
        if (errors.size() > 0) {
            return null;
        }
        GroupNode gd = (GroupNode) rootnode;
        try {
            String name = (String) extractNode(gd,"name", RootNode.Type.STRING).getData();
            PointU gravity = (PointU) extractNode(gd,"gravity", RootNode.Type.POINT).getData();

            worlddef = new GenericWorldDef(name);
            worlddef.gravity = gravity;


            GroupNode objects = (GroupNode) extractNode(gd,"Objects", RootNode.Type.GROUP,false);

            if (objects != null) {
                for (RootNode child : objects.getChilds()) {

                }
            }

            GroupNode forces = (GroupNode) extractNode(gd,"Forces", RootNode.Type.GROUP,false);

            GroupNode joints = (GroupNode) extractNode(gd,"Joints", RootNode.Type.GROUP,false);

            GroupNode groups = (GroupNode) extractNode(gd,"Groups", RootNode.Type.GROUP,false);


            if (objects == null && (forces != null || joints != null || groups != null)) {
                errors.add("You have no objects, but have some joints or forces...");
                throw new ConfigParserException();
            }


            return worlddef;
        } catch (ConfigParserException e) {
            e.printStackTrace();
            return null;
        }
    }
}
