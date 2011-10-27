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
import android.content.res.AssetManager;
import android.util.Log;
import mouse.runtime.Source;
import mouse.runtime.SourceString;
import net.neverb.peasantformer.sceneparser.mouse.MouseParser;
import net.neverb.peasantformer.sceneparser.nodes.RootNode;

import java.io.IOException;
import java.io.InputStream;
import java.util.LinkedList;

/**
 * Author: Alexander Tumin
 * Created: 2011-10-25 17:16
 */
public class ConfigLexer {
    protected MouseParser parser;
    protected LinkedList<String> errors;

    protected Source src;
    public ConfigLexer(Context c, LinkedList<String> errors) {
        this.errors = errors;
        AssetManager assetManager = c.getAssets();
        StringBuilder file = new StringBuilder();
        try {
            InputStream abcs  = assetManager.open("inputData");
            int ch;
            while ((ch = abcs.read()) != -1) {
                file.append((char)ch);
            }

        } catch (IOException e) {
            errors.add("Lexer: I/O error :/");
            e.printStackTrace();
        }

        parser = new MouseParser();
        src = new SourceString(file.toString());
    }
    public ConfigLexer(String c, LinkedList<String> errors) {
        parser = new MouseParser();
        src = new SourceString(c);
    }


    public RootNode lex() {
        if (!src.created()) return null;
        parser.semantics().setErrors(errors);
        boolean ok = parser.parse(src);
        if (!ok) {
            return null;
        }
        return parser.semantics().getResult();
    }
}
