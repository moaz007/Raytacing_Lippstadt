#include "Transmitter.h"
#include "RayTracingModule.h"

#ifdef WITH_OSGEARTH

#include "OsgEarthScene.h"
#include "omnetpp/osgutil.h"

#include <osg/Node>
#include <osg/PositionAttitudeTransform>
#include <osg/Material>
#include <osgEarth/Capabilities>
#include <osgEarth/Version>
#include <osgEarthAnnotation/LabelNode>
#include <osgEarthSymbology/Geometry>
#include <osgEarthFeatures/Feature>

using namespace omnetpp;
using namespace osgEarth;

Define_Module(Transmitter);

void Transmitter::initialize(int stage)
{
    if (stage == 0) {
        modelURL = par("modelURL").stringValue();
        latitude = par("latitude");
        longitude = par("longitude");
        labelColor = par("labelColor").stringValue();
        initialEvent = new omnetpp::cMessage("initialEvent");
    }
    else if (stage == 1) {
        auto scene = OsgEarthScene::getInstance()->getScene();
        auto mapNode = osgEarth::MapNode::findMapNode(scene);

        geoTransform = new osgEarth::GeoTransform();
        localTransform = new osg::PositionAttitudeTransform();
        geoTransform->addChild(localTransform);

        auto modelNode = osgDB::readNodeFile(modelURL);
        if (!modelNode)
            throw cRuntimeError("Model file \"%s\" not found", modelURL.c_str());

        // Enable lighting and shaders
        modelNode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON);

        localTransform->addChild(modelNode);
        auto geoSRS = mapNode->getMapSRS()->getGeographicSRS();
        geoTransform->setPosition(osgEarth::GeoPoint(geoSRS, longitude, latitude, 0.0));

        mapNode->addChild(geoTransform);

        Style labelStyle;
        labelStyle.getOrCreate<TextSymbol>()->alignment() = TextSymbol::ALIGN_CENTER_TOP;
        labelStyle.getOrCreate<TextSymbol>()->declutter() = true;
        labelStyle.getOrCreate<TextSymbol>()->pixelOffset() = osg::Vec2s(0, 50); // Adjusted to bring label closer
        labelStyle.getOrCreate<TextSymbol>()->fill()->color() = osgEarth::Color(labelColor);

        auto labelNode = new osgEarth::Annotation::LabelNode("Transmitter", labelStyle);
        localTransform->addChild(labelNode);

        // Set the model color to bright red
        auto color = osg::Vec4(1.0, 0.0, 0.0, 1.0); // Bright red
        auto material = new osg::Material();
        material->setAmbient(osg::Material::FRONT_AND_BACK, color * 0.2); // Slightly dimmer ambient color
        material->setDiffuse(osg::Material::FRONT_AND_BACK, color); // Bright diffuse color
        material->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(1.0, 1.0, 1.0, 1.0)); // White specular highlights
        material->setShininess(osg::Material::FRONT_AND_BACK, 64.0); // Shininess
        modelNode->getOrCreateStateSet()->setAttribute(material, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
        modelNode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON);

        auto objectNode = new cObjectOsgNode(this);
        objectNode->addChild(modelNode);
        localTransform->addChild(objectNode);

        // Schedule the initial event
        scheduleAt(simTime() + 1, initialEvent);
    }
}

void Transmitter::handleMessage(omnetpp::cMessage *msg)
{
    if (msg == initialEvent) {
        // Find the ray tracing module and start ray tracing
        auto rayTracer = check_and_cast<RayTracingModule *>(getParentModule()->getSubmodule("rayTracer"));
        if (rayTracer) {
            rayTracer->traceRay();
        }
        scheduleAt(simTime() + 1, initialEvent); // Schedule the next event
    } else {
        delete msg;
    }
}

#endif // WITH_OSGEARTH
