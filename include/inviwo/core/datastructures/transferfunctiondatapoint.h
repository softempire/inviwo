/** \
*
*/

#ifndef IVW_TRANSFERFUNCTIONDATAPOINT_H
#define IVW_TRANSFERFUNCTIONDATAPOINT_H
#include <stdlib.h>
#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/image/imagedisk.h>
#include <inviwo/core/datastructures/image/imageram.h>
#include <inviwo/core/ports/imageport.h>

namespace inviwo {

    class IVW_CORE_API TransferFunctionDataPoint {

    public:
        TransferFunctionDataPoint();
        TransferFunctionDataPoint(vec2* pos);
        TransferFunctionDataPoint(vec2* pos, vec4* rgba);
        virtual ~TransferFunctionDataPoint();

        const vec2* getPos();
        void setPos(vec2* pos);
        const vec4* getRgba();
        void setRgba(vec4* rgba);
        void setRgb(const vec3* rgb);
        void setA(const float alpha);

    private:
		vec2* pos_;
        vec4* rgba_;
    };

} // namespace
#endif // IVW_TRANSFERFUNCTIONDATAPOINT_H
