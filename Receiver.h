#ifndef __RECEIVER_H
#define __RECEIVER_H

#include "omnetpp.h"

#ifdef WITH_OSGEARTH

#include <osgEarth/GeoTransform>
#include <osg/PositionAttitudeTransform>
#include <osgEarthFeatures/Feature>
#include <osgEarth/MapNode>
#include <osgEarthAnnotation/CircleNode>
#include <osgEarthAnnotation/FeatureNode>

class Receiver : public omnetpp::cSimpleModule
{
  private:
    std::string modelURL;
    double latitude;
    double longitude;
    std::string labelColor; // New configuration parameter for label color
    osg::ref_ptr<osgEarth::GeoTransform> geoTransform;
    osg::ref_ptr<osg::PositionAttitudeTransform> localTransform;

  protected:
    virtual void initialize(int stage) override;
    virtual int numInitStages() const override { return 2; }
};

#endif // WITH_OSGEARTH

#endif // __RECEIVER_H
