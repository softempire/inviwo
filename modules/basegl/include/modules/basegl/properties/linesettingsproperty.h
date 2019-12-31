/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2019 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *********************************************************************************/

#include <modules/basegl/datastructures/linesettingsinterface.h>
#include <inviwo/core/properties/compositeproperty.h>
#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/properties/ordinalproperty.h>
#include <inviwo/core/properties/stipplingproperty.h>

namespace inviwo {
class IVW_MODULE_BASEGL_API LineSettingsProperty:
    public LineSettingsInterface,
    public CompositeProperty {
public:
    LineSettingsProperty(const std::string& identifier, const std::string& displayName,
                             InvalidationLevel invalidationLevel = InvalidationLevel::InvalidResources,
                             PropertySemantics semantics = PropertySemantics::Default);
    virtual ~LineSettingsProperty() = default;
        
    // Inherited from LineSettingsInterface
    // Line width in pixels
    virtual float getWidth() const;
    virtual float getAntialiasingWidth() const;
    
    virtual float getMiterLimit() const;
    virtual bool getRoundCaps() const;
    virtual bool getPseudoLighting() const;
    virtual bool getRoundDepthProfile() const;
    
    virtual const StipplingSettingsInterface& getStippling() const;
        
    FloatProperty lineWidth_;
    FloatProperty antialiasing_;
    FloatProperty miterLimit_;
    BoolProperty roundCaps_;

    BoolProperty pseudoLighting_;
    BoolProperty roundDepthProfile_;

    StipplingProperty stippling_;
    
};

}  // namespace inviwo