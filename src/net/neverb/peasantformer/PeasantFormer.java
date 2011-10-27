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

package net.neverb.peasantformer;


import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import net.neverb.peasantformer.definitions.GenericWorldDef;
import net.neverb.peasantformer.sceneparser.ConfigLexer;
import net.neverb.peasantformer.sceneparser.ConfigParser;

public class PeasantFormer extends Activity {

    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        ConfigParser parser = new ConfigParser(this);
        GenericWorldDef wdef = parser.parse();
        if (parser.errors.size() > 0) {
            for (String s : parser.errors) {
                Log.d("ABAKA",s);
            }
        } else {
            Log.d("ABAKA","Parser succeded");
        }


        /*
        AbcParser sceneparser = Parboiled.createParser(AbcParser.class);

        ParsingResult<?> result = new RecoveringParseRunner(sceneparser.S()).run(input);

        Log.d(">>",input + " = " + result.parseTreeRoot.getLabel());
        Log.d(">>", ParseTreeUtils.printNodeTree(result));
        if (!result.matched) {
            Log.d(">>", StringUtils.join(result.parseErrors,"---\n"));
        }

        */
        //FactoryMaterial.getInstance().bake(new RegularPeasantMaterialRecipe
//                ());
//        Log.i("BAAAA", RectU.parsePointU(
//                "RectU (1.0m,1.0p,1.0,5)").toString());
    }
    /*
    protected Engine engine;

    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.d("MAIN", "Create");
        setContentView(R.layout.main);
        try {
            engine = new Engine(
                    2,
                    this,
                    Box2DPhysicalWorld.class
//                    PhysPlayground.class,
//                    OpenGLSurface.class

            );
            engine.start();
//            setContentView(engine.getRenderer().getView());

        } catch (Exception e) {
            e.printStackTrace();
        }

    }
    public void onPause() {
        super.onPause();
        Log.d("MAIN", "paused");
        if (engine == null) {
            return;
        }
        engine.pause();

    }
    public void onResume() {
        super.onResume();
        Log.d("MAIN", "unpaused");
        if (engine == null) {
            return;
        }
        engine.resume();
    }
    public void onStop() {
        super.onStop();
        Log.d("MAIN","stopped");
        if (engine == null) {
            return;
        }
        engine.pause();
    }
    public void onDestroy() {
        super.onDestroy();
        Log.d("MAIN", "destroyed");
        if (engine == null) {
            return;
        }
        engine.stop();
    }
    public void onLowMemory() {
        super.onLowMemory();
        Log.d("MAIN","low memeory");
    }
    */
}