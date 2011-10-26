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

package net.neverb.peasantformer.interfaces.objects.compound;

import net.neverb.peasantformer.interfaces.objects.compound.force.GenericForceDef;
import net.neverb.peasantformer.interfaces.objects.compound.joint.GenericJointDef;
import net.neverb.peasantformer.interfaces.objects.compound.object.GenericObjectDef;

import java.util.Collection;
import java.util.HashMap;


/**
 * Author: Alexander Tumin
 * Created: 2011-10-24 18:40
 */
public class GenericCompoundDef {
    protected final String name;

    protected final HashMap<String,GenericForceDef> forces =
            new HashMap<String, GenericForceDef>();
    protected final HashMap<String,GenericJointDef> joints =
            new HashMap<String, GenericJointDef>();
    protected final HashMap<String,GenericObjectDef> objects =
            new HashMap<String, GenericObjectDef>();


    public GenericCompoundDef(String name) {
        this.name = name;
    }

    public Collection<GenericForceDef> getForces() {
        return forces.values();
    }
    public Collection<GenericJointDef> getJoints() {
        return joints.values();
    }
    public Collection<GenericObjectDef> getObjects() {
        return objects.values();
    }

    public String getName() {
        return name;
    }
}
