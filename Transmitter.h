#ifndef __TRANSMITTER_H
#define __TRANSMITTER_H

#include "omnetpp.h"

#ifdef WITH_OSGEARTH

#include <osgEarth/GeoTransform>
#include <osg/PositionAttitudeTransform>
#include <osgEarthFeatures/Feature>
#include <osgEarth/MapNode>
#include <osgEarthAnnotation/CircleNode>
#include <osgEarthAnnotation/FeatureNode>

class Transmitter : public omnetpp::cSimpleModule
{
  private:
    std::string modelURL;
    double latitude;
    double longitude;
    std::string labelColor; // New configuration parameter for label color
    osg::ref_ptr<osgEarth::GeoTransform> geoTransform;
    osg::ref_ptr<osg::PositionAttitudeTransform> localTransform;
    omnetpp::cMessage *initialEvent; // Event to trigger ray tracing

  protected:
    virtual void initialize(int stage) override;
    virtual void handleMessage(omnetpp::cMessage *msg) override;
    virtual int numInitStages() const override { return 2; }
};

#endif // WITH_OSGEARTH

#endif // __TRANSMITTER_H
