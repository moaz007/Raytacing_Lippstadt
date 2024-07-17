#ifndef __RAYTRACINGMODULE_H
#define __RAYTRACINGMODULE_H

#include <omnetpp.h>

#ifdef WITH_OSGEARTH

class RayTracingModule : public omnetpp::cSimpleModule
{
  private:
    double transmitterLatitude;
    double transmitterLongitude;
    double receiverLatitude;
    double receiverLongitude;
    double transmitterFrequency;
    int maxNumReflections;

  protected:
    virtual void initialize(int stage) override;
    virtual void handleMessage(omnetpp::cMessage *msg) override;
    virtual int numInitStages() const override { return 1; }

  public:
    void traceRay();
};

#endif // WITH_OSGEARTH

#endif // __RAYTRACINGMODULE_H
