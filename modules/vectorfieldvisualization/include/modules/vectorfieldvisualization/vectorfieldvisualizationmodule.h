/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2015-2019 Inviwo Foundation
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

#ifndef IVW_VECTORFIELDVISUALIZATIONMODULE_H
#define IVW_VECTORFIELDVISUALIZATIONMODULE_H

#include <modules/vectorfieldvisualization/vectorfieldvisualizationmoduledefine.h>
#include <inviwo/core/common/inviwomodule.h>
#include <inviwo/core/io/serialization/versionconverter.h>

namespace inviwo {

class IVW_MODULE_VECTORFIELDVISUALIZATION_API VectorFieldVisualizationModule : public InviwoModule {

public:
    VectorFieldVisualizationModule(InviwoApplication* app);

    virtual int getVersion() const override;
    virtual std::unique_ptr<VersionConverter> getConverter(int version) const override;

private:
    class Converter : public VersionConverter {
        typedef bool (Converter::*updateType)(TxElement*);

    public:
        Converter(int version);
        virtual bool convert(TxElement* root) override;

        bool traverseNodes(TxElement* node, updateType update);
        bool updateAllowLooping(TxElement* node);

    private:
        bool integralLineTracerMetaDataProperty(TxElement* root);
        int version_;
    };
};

}  // namespace inviwo

#endif  // IVW_VECTORFIELDVISUALIZATIONMODULE_H
