#include "RayTracingModule.h"
#include "OsgEarthScene.h"

#ifdef WITH_OSGEARTH

#include <osgUtil/LineSegmentIntersector>
#include <osgUtil/IntersectionVisitor>
#include <osgEarth/MapNode>
#include <osgEarth/GeoTransform>

using namespace omnetpp;
using namespace osgEarth;
using namespace osgUtil;

Define_Module(RayTracingModule);

void RayTracingModule::initialize(int stage)
{
    if (stage == 0)
    {
        transmitterLatitude = par("txLatitude");
        transmitterLongitude = par("txLongitude");
        receiverLatitude = par("rxLatitude");
        receiverLongitude = par("rxLongitude");
        transmitterFrequency = par("transmitterFrequency");
        maxNumReflections = par("maxNumReflections");

        auto mapNode = MapNode::findMapNode(OsgEarthScene::getInstance()->getScene());
        if (!mapNode)
        {
            throw cRuntimeError("MapNode not found.");
        }
    }
}

void RayTracingModule::handleMessage(cMessage *msg)
{
    // No message handling needed here
}

void RayTracingModule::traceRay()
{
    osg::Vec3d start(transmitterLongitude, transmitterLatitude, 0);
    osg::Vec3d end(receiverLongitude, receiverLatitude, 0);

    EV << "Starting ray tracing from " << transmitterLatitude << ", " << transmitterLongitude << " to " << receiverLatitude << ", " << receiverLongitude << "\n";
    EV << "Intersecting from (" << start.x() << ", " << start.y() << ", " << start.z() << ") to (" << end.x() << ", " << end.y() << ", " << end.z() << ")\n";

    auto mapNode = MapNode::findMapNode(OsgEarthScene::getInstance()->getScene());
    if (!mapNode)
    {
        throw cRuntimeError("MapNode not found.");
    }

    osg::ref_ptr<LineSegmentIntersector> intersector = new LineSegmentIntersector(start, end);
    osg::ref_ptr<IntersectionVisitor> iv = new IntersectionVisitor(intersector);
    mapNode->accept(*iv);

    if (intersector->containsIntersections())
    {
        auto result = intersector->getFirstIntersection();
        EV << "Intersection found at (" << result.getWorldIntersectPoint().x() << ", " << result.getWorldIntersectPoint().y() << ", " << result.getWorldIntersectPoint().z() << ")\n";
    }
    else
    {
        EV << "No intersection found.\n";
    }
}

#endif // WITH_OSGEARTH
