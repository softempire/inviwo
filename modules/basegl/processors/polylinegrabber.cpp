/*********************************************************************************
*
* Inviwo - Interactive Visualization Workshop
*
* Copyright (c) 2013-2018 Inviwo Foundation
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

#include "polylinegrabber.h"

#include <fstream>

namespace inviwo {

	const ProcessorInfo PolylineGrabber::processorInfo_{
		"org.inviwo.PolylineGrabber",  // Class identifier
		"Polyline Grabber",              // Display name
		"Input",          // Category
		CodeState::Experimental,           // Code state
		Tags::CPU,                    // Tags
	};
	const ProcessorInfo PolylineGrabber::getProcessorInfo() const { return processorInfo_; }

	PolylineGrabber::PolylineGrabber()
		: Processor()
		, pt_("pt", "Point to Add")
		, clearPolyline_("clearpolyline", "Clear Points")
		, polyline_(std::make_shared<std::vector<vec3>>())
		, outport_("polylineport")
	{
		outport_.setData(polyline_);
		addPort(outport_);

		clearPolyline_.onChange([this]() { polyline_->clear(); });
		addProperty(clearPolyline_);

		//pt_.setVisible(false);
		pt_.setReadOnly(true);
		pt_.onChange([this]() { addPoint(pt_); });
		addProperty(pt_);

		//TODO transform and use data points

		/*std::ifstream inputStream("C:/Users/ulm_VISCOMSTUD7/Documents/kircher/2009 - Aortic Artery Mouse/2134.dijkstra");
		if (inputStream.is_open()) {
			vec3 pt;
			while (inputStream >> pt.x >> pt.y >> pt.z) {
				addPoint(pt);
			}
		}
		inputStream.close();*/

		//TODO smoothen the normal

		//TODO color points whether they are on slice or above or below

		//TODO implement move and delete points
	}

	void PolylineGrabber::addPoint(const vec3& pt)
	{
		polyline_->push_back(pt);
		invalidate(InvalidationLevel::InvalidOutput);
	}

}  // namespace inviwo